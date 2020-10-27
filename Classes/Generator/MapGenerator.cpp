#include <random>
#include "cocos2d.h"
#include "Delaunay.h"
#include "MST.h"
#include "MapGenerator.h"

MapGenerator::MapGenerator()
{
    // メルセンヌ・ツイスター法による擬似乱数生成器を、
    // ハードウェア乱数をシードにして初期化
    _engine.seed(_seed_gen());
    std::uniform_real_distribution<>::param_type param(0, 1);
    std::uniform_int_distribution<>::param_type param_i(8, 16);
    std::uniform_int_distribution<>::param_type param_n(4, 7);
    _dist.param(param);
    _dist_i.param(param_i);
    _dist_n.param(param_n);

    _delaunay = new Delaunay();
}

MapGenerator::~MapGenerator()
{
    if (_delaunay != nullptr)
    {
        delete _delaunay;
        _delaunay = nullptr;
    }if (_mst != nullptr)
    {
        delete _mst;
        _mst = nullptr;
    }
}

void MapGenerator::Call()
{
    Create_Room();
    DistributedRoom();
    Create_Delaunay();
    Create_MST();
}

const std::vector<Room_Data>& MapGenerator::GetRoomData(void)
{
    return _data;
}

const Triangle_Data MapGenerator::GetDelaunayData(void)
{
    return _delaunay->Get_Triangle_Data();
}

const std::vector<std::array<cocos2d::Vec2, 2>> MapGenerator::GetMSTNode(void)
{
    return _mst->GetNode();
}

bool MapGenerator::Create_Room(void)
{
    for (std::size_t n = 0; n < FLOOR; n++)
    {
        double result = _dist(_engine);
        double r = 100 * _dist(_engine);
        auto  t = 2 * M_PI * result;
        auto pos = (cocos2d::Vec2(450 + r * cos(t), 500 + r * sin(t)));
        auto pos_area2 = (cocos2d::Vec2(750 + r * cos(t), 500 + r * sin(t)));
        auto pos_area3 = (cocos2d::Vec2(600 + r * cos(t), 600 + r * sin(t)));
        auto pos_area4 = (cocos2d::Vec2(600 + r * cos(t), 400 + r * sin(t)));
        auto size = (cocos2d::Size(4.0f * _dist_i(_engine), 4.0f * _dist_n(_engine)));
        auto name = "floor" + cocos2d::StringUtils::toString(n);
        if (n < FLOOR / 4)
        {
            if (n == 0)
            {
                _data.emplace_back(Room_Data{ cocos2d::Vec2(600, 500),size,cocos2d::Vec2(600, 500),name,cocos2d::Color4F(1, 0, 1, 0.5),cocos2d::Color4F(0.89, 0.19, 0.05, 0.5)});
                //draw->setTag(0);
                continue;
            }
            _data.emplace_back(Room_Data{ pos,size,cocos2d::Vec2(450,500),name, cocos2d::Color4F(0, 0, 1, 0.5), cocos2d::Color4F(0.38, 0.73, 1, 0.5)});
        }
        else if (n < FLOOR / 2)
        {
            _data.emplace_back(Room_Data{ pos_area3,size,cocos2d::Vec2(600,600),name,cocos2d::Color4F(1, 0, 1, 0.5), cocos2d::Color4F(0.05, 0.19, 0.89, 0.5)});
        }
        else if (n > (FLOOR / 2) + (FLOOR / 4))
        {
            _data.emplace_back(Room_Data{ pos_area4,size,cocos2d::Vec2(600,400),name, cocos2d::Color4F(1, 0, 1, 0.5), cocos2d::Color4F(0, 1, 0, 0.5) });
        }
        else
        {
            _data.emplace_back(Room_Data{pos_area2,size,cocos2d::Vec2(750,500),name, cocos2d::Color4F(1, 0, 1, 0.5), cocos2d::Color4F(0.59, 0.7, 0, 0.5)});
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

                auto square_center_n = cocos2d::Vec2(_data[n].size.width / 2, _data[n].size.height / 2);
                auto square_center_m = cocos2d::Vec2(_data[m].size.width / 2, _data[m].size.height / 2);
                auto tmp = std::abs(_data[n].pos.x - _data[m].pos.x);
                auto tmp2 = std::abs(_data[n].pos.y - _data[m].pos.y);
                auto tmp3 = (square_center_n + square_center_m).x;
                auto tmp4 = (square_center_n + square_center_m).y;

                if ((square_center_n + square_center_m).x > std::abs((_data[n].pos.x + square_center_n.x)- (_data[m].pos.x + square_center_m.x)) &&
                    (square_center_n + square_center_m).y > std::abs((_data[n].pos.y + square_center_n.y) - (_data[m].pos.y + square_center_m.y)))
                {
                    vec = (_data[n].pos - _data[m].pos);
                    vec *= -1;
                    vec.normalize();
                    _data[m].pos += vec;

                    vec1 = (_data[m].pos - _data[n].pos);
                    vec1 *= -1;
                    vec1.normalize();
                    _data[n].pos += vec1;
                    //tmp_pos_n = cocos2d::Vec2(std::round(tmp_pos_n.x), std::round(tmp_pos_n.y));
                    flag = true;
                }
            }
        }
    } 

    return true;
}

bool MapGenerator::Create_Delaunay(void)
{
    _delaunay->Create_Triangle(cocos2d::Vec2(0, 0), cocos2d::Vec2(600, 500));
    for (int i = 0; i < FLOOR; i++)
    {
        _delaunay->Subdivision_Triangle((_data[i].pos + _data[i].size / 2) - cocos2d::Vec2(600, 400));
        _vertex.push_back((_data[i].pos + _data[i].size / 2) - cocos2d::Vec2(600, 400));
    }
    _delaunay->FinishDelaunay();
    return true;
}

bool MapGenerator::Create_MST(void)
{
    _mst = new MST(_delaunay->Triangle_To_Edge(),_vertex, 0);
    _mst->Choice_Node();
    return false;
}
