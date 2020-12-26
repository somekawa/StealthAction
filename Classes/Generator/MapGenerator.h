#pragma once
#include <vector>
#include <list>
#include <string>
#include <mutex>
#include "Direction/Direction.h"
constexpr auto FLOOR = 50;
//enum class Map
//{
//
//};
struct Room_Data
{
    int area;
    bool lock;
    cocos2d::Vec2 pos;
    cocos2d::Size size;
    cocos2d::Vec2 offset;
    //debug用
    std::string name;
    cocos2d::Color4F line_color;
    cocos2d::Color4F fill_color;
};

struct Triangle_Status;
using Triangle_Data = std::vector<Triangle_Status>;
using Vertex_List = std::vector<cocos2d::Vec2>;

struct Node_Status;
class Delaunay;
class MST;
class MapGenerator
{
public:
    MapGenerator(std::uint_fast32_t seed = 0);
    ~MapGenerator();

    void Call();

    const std::vector<Room_Data>& GetRoomData(void);
    const Triangle_Data GetDelaunayData(void);
    const std::vector<Node_Status> GetMSTNode(void);
    const std::list<MapDirection> FloorDir(void);
private:
    //エリアごとに設定されている中心点の周囲にMapのもととなる部屋を作成.
    bool CreateRoom(void);
    //重なっている部屋を"BoidAlgorithm"を用いて分散させる.
    bool DistributedRoom(void);
    bool CreateDelaunay(void);
    bool CreateRoot(void);
    std::vector<Room_Data> data_;
    Vertex_List vertex_;
    std::vector<int> areaData_;

    std::random_device seedGen_;
    std::default_random_engine engine_;
    std::uniform_real_distribution<> dist_;
    std::uniform_int_distribution<> _dist_i;
    std::uniform_int_distribution<> _dist_n;

    Triangle_Data triangleData_;
    std::unique_ptr <Delaunay> delaunay_ = nullptr;
    std::unique_ptr<MST> mst_ = nullptr;
};