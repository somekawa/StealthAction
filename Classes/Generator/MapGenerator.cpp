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
        double result = dist_(engine_);
        double r = 100 * dist_(engine_);
        auto  t = 2 * M_PI * result;
        auto pos_area1 = (cocos2d::Vec2(400 + r * cos(t), 500 + r * sin(t)));
        auto pos = (cocos2d::Vec2(600 + r * cos(t), 500 + r * sin(t)));
        auto pos_area2 = (cocos2d::Vec2(600 + r * cos(t), 700 + r * sin(t)));
        auto pos_area3 = (cocos2d::Vec2(600 + r * cos(t), 300 + r * sin(t)));
        auto pos_area4 = (cocos2d::Vec2(800 + r * cos(t), 500 + r * sin(t)));
        auto size = (cocos2d::Size(4.0f * 16, 4.0f * 8));
        auto name = "floor" + cocos2d::StringUtils::toString(n);

        auto area = (n / (FLOOR / 5));
        if (area == 0)
        {
            if (n == 0)
            {
                data_.emplace_back(Room_Data{0,false,cocos2d::Vec2(600, 500),size,cocos2d::Vec2(600, 500),name,cocos2d::Color4F(1, 1, 1, 0.5),cocos2d::Color4F(1,0, 0,1) });
                continue;
            }
            data_.emplace_back(Room_Data{0,false,pos,size,cocos2d::Vec2(450,500),name, cocos2d::Color4F(1, 0, 0, 0.5), cocos2d::Color4F(0.8,0.8,0.8, 0.5) });
        }
        else if (area == 1)
        {
            data_.emplace_back(Room_Data{1,true,pos_area1,size,cocos2d::Vec2(450,500),name, cocos2d::Color4F(0, 0, 1,1), cocos2d::Color4F(0.8,0.8,0.8,0.5)});
        }
        else if (area == 2)
        {
           data_.emplace_back(Room_Data{2,true,pos_area2,size,cocos2d::Vec2(600,600),name,cocos2d::Color4F(0, 1,0,1), cocos2d::Color4F(0.8,0.8,0.8,0.5)});
        }
        else if (area == 3)
        {
           data_.emplace_back(Room_Data{3,true,pos_area3,size,cocos2d::Vec2(600,400),name, cocos2d::Color4F(0.0,1.0,1.0,1), cocos2d::Color4F(0.8,0.8,0.8,0.5) });
        }
        else if (area == 4)
        {
           data_.emplace_back(Room_Data{4,true,pos_area4,size,cocos2d::Vec2(750,500),name, cocos2d::Color4F(1.0, 0.0,1.0,1), cocos2d::Color4F(0.8,0.8,0.8,0.5)});
        }
    }
    return true;
}

bool MapGenerator::DistributedRoom(void)
{
    auto vec = cocos2d::Vec2(0, 0);
    auto vec1 = cocos2d::Vec2(0, 0);
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

                auto square_center_n = cocos2d::Vec2(data_[n].size.width / 2, data_[n].size.height / 2);
                auto square_center_m = cocos2d::Vec2(data_[m].size.width / 2, data_[m].size.height / 2);
                auto tmp = std::abs(data_[n].pos.x - data_[m].pos.x);
                auto tmp2 = std::abs(data_[n].pos.y - data_[m].pos.y);
                auto tmp3 = (square_center_n + square_center_m).x;
                auto tmp4 = (square_center_n + square_center_m).y;

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
    delaunay_->CreateTriangle(cocos2d::Vec2(0, 0), cocos2d::Vec2(600, 500));
    for (int i = 0; i < FLOOR; i++)
    {
        delaunay_->SubdivisionTriangle((data_[i].pos + data_[i].size / 2) - cocos2d::Vec2(600, 400));
        vertex_.push_back((data_[i].pos + data_[i].size / 2) - cocos2d::Vec2(600, 400));
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
