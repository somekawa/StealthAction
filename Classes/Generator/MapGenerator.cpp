#include <random>
#include "cocos2d.h"
#include "Delaunay.h"
#include "MST.h"
#include "MapGenerator.h"

MapGenerator::MapGenerator(std::uint_fast32_t seed)
{
    // メルセンヌ・ツイスター法による擬似乱数生成器を、
    // ハードウェア乱数をシードにして初期化
    engine_.seed(seed);
    std::uniform_real_distribution<>::param_type param(0, 1);
    std::uniform_int_distribution<>::param_type param_i(8, 16);
    std::uniform_int_distribution<>::param_type param_n(4, 7);
    dist_.param(param);
    _dist_i.param(param_i);
    _dist_n.param(param_n);

    delaunay_ = std::make_unique<Delaunay>();
}

MapGenerator::~MapGenerator()
{
}

void MapGenerator::Call()
{
    CreateRoom();
    DistributedRoom();
    CreateDelaunay();
    CreateRoot();
}

const std::vector<Room_Data>& MapGenerator::GetRoomData(void)
{
    return data_;
}

const Triangle_Data MapGenerator::GetDelaunayData(void)
{
    return delaunay_->GetTriangleData();
}

const std::vector<Node_Status> MapGenerator::GetMSTNode(void)
{
    return mst_->GetNode();
}

const std::vector<Edge_Status> MapGenerator::GetEdgeData(void)
{
    return mst_->GetEdgeData();
}

const std::list<MapDirection> MapGenerator::FloorDir(void)
{
    return std::list<MapDirection>();
}

bool MapGenerator::CreateRoom(void)
{
    for (std::size_t n = 0; n < FLOOR; n++)
    {
        const float result = dist_(engine_);
		const float r = 100.0f * dist_(engine_);
		const float t = 2.0f * static_cast<float>(M_PI) * result;
		const cocos2d::Vec2 pos_area1 = (cocos2d::Vec2(400.0f + r * cos(t), 500.0f + r * sin(t)));
		const cocos2d::Vec2 pos = (cocos2d::Vec2(600.0f + r * cos(t), 500.0f + r * sin(t)));
		const cocos2d::Vec2 pos_area2 = (cocos2d::Vec2(600.0f + r * cos(t), 700.0f + r * sin(t)));
		const cocos2d::Vec2 pos_area3 = (cocos2d::Vec2(600.0f + r * cos(t), 300.0f + r * sin(t)));
		const cocos2d::Vec2 pos_area4 = (cocos2d::Vec2(800.0f + r * cos(t), 500.0f + r * sin(t)));
		const cocos2d::Size size = (cocos2d::Size(4.0f * 16.0f, 4.0f * 8.0f));
		const std::string name = "floor" + cocos2d::StringUtils::toString(n);

        const unsigned int area = (n / (FLOOR / 5));
        if (area == 0)
        {
            if (n == 0)
            {
                data_.emplace_back(Room_Data{0,false,cocos2d::Vec2(600.0f, 500.0f),size,cocos2d::Vec2(600.0f, 500.0f),name,cocos2d::Color4F(1.0f, 1.0f, 1.0f, 0.5f),cocos2d::Color4F(1.0f,0.0f, 0.0f,1.0f) });
                continue;
            }
            data_.emplace_back(Room_Data{0,false,pos,size,cocos2d::Vec2(450.0f,500.0f),name, cocos2d::Color4F(1.0f, 0.0f, 0.0f, 0.5f), cocos2d::Color4F(0.8f,0.8f,0.8f, 0.5f) });
        }
        else if (area == 1)
        {
            data_.emplace_back(Room_Data{1,true,pos_area1,size,cocos2d::Vec2(450.0f,500.0f),name, cocos2d::Color4F(0.0f, 0.0f, 1.0f,1.0f), cocos2d::Color4F(0.8f,0.8f,0.8f,0.5f)});
        }
        else if (area == 2)
        {
           data_.emplace_back(Room_Data{2,true,pos_area2,size,cocos2d::Vec2(600.0f,600.0f),name,cocos2d::Color4F(0.0f, 1.0f,0.0f,1.0f), cocos2d::Color4F(0.8f,0.8f,0.8f,0.5f)});
        }
        else if (area == 3)
        {
           data_.emplace_back(Room_Data{3,true,pos_area3,size,cocos2d::Vec2(600.0f,400.0f),name, cocos2d::Color4F(0.0f,1.0f,1.0f,1.0f), cocos2d::Color4F(0.8f,0.8f,0.8f,0.5f) });
        }
        else if (area == 4)
        {
           data_.emplace_back(Room_Data{4,true,pos_area4,size,cocos2d::Vec2(750.0f,500.0f),name, cocos2d::Color4F(1.0f, 0.0f,1.0f,1.0f), cocos2d::Color4F(0.8f,0.8f,0.8f,0.5f)});
        }
		else
		{
			// 何も処理を行わない
		}
    }
    return true;
}

bool MapGenerator::DistributedRoom(void)
{
	cocos2d::Vec2 vec = cocos2d::Vec2(0.0f, 0.0f);
	cocos2d::Vec2 vec1 = cocos2d::Vec2(0.0f, 0.0f);
    bool flag = true;     //呼ばれるたびに加算し、分割を開始するかの判定に使用する
    
    while (flag)
    {
        flag = false;
        //floorの移動処理
        for (int n = 0; n < FLOOR; n++)
        {
            for (int m = 0; m < FLOOR; m++)
            {
                if (n == m)
                {
                    continue;
                }

				const cocos2d::Vec2 square_center_n = cocos2d::Vec2(data_[n].size.width / 2, data_[n].size.height / 2);
				const cocos2d::Vec2 square_center_m = cocos2d::Vec2(data_[m].size.width / 2, data_[m].size.height / 2);
				const float tmp  = std::abs(data_[n].pos.x - data_[m].pos.x);
				const float tmp2 = std::abs(data_[n].pos.y - data_[m].pos.y);
				const float tmp3 = (square_center_n + square_center_m).x;
				const float tmp4 = (square_center_n + square_center_m).y;

                if ((square_center_n + square_center_m).x > std::abs((data_[n].pos.x + square_center_n.x)- (data_[m].pos.x + square_center_m.x)) &&
                    (square_center_n + square_center_m).y > std::abs((data_[n].pos.y + square_center_n.y) - (data_[m].pos.y + square_center_m.y)))
                {
                    vec = (data_[n].pos - data_[m].pos);
                    vec *= -1;
                    vec.normalize();
                    data_[m].pos += vec;

                    vec1 = (data_[m].pos - data_[n].pos);
                    vec1 *= -1;
                    vec1.normalize();
                    data_[n].pos += vec1;
                    //tmp_pos_n = cocos2d::Vec2(std::round(tmp_pos_n.x), std::round(tmp_pos_n.y));
                    flag = true;
                }
            }
        }
    } 

    return true;
}

bool MapGenerator::CreateDelaunay(void)
{
    delaunay_->CreateTriangle(cocos2d::Vec2(0.0f, 0.0f), cocos2d::Vec2(600.0f, 500.0f));
    for (int i = 0; i < FLOOR; i++)
    {
        delaunay_->SubdivisionTriangle((data_[i].pos + data_[i].size / 2) - cocos2d::Vec2(600.0f, 400.0f));
        vertex_.push_back((data_[i].pos + data_[i].size / 2) - cocos2d::Vec2(600.0f, 400.0f));
        areaData_.push_back(data_[i].area);
    }
    delaunay_->FinishDelaunay();
    return true;
}

bool MapGenerator::CreateRoot(void)
{
    mst_ = std::make_unique<MST>(delaunay_->TriangletoEdge(),vertex_,areaData_, 0);
    mst_->CreateMST();
    return true;
}