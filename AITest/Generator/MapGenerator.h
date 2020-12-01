#pragma once
#include <vector>
#include <list>
#include <string>
#include "Direction/Direction.h"
constexpr auto FLOOR = 10;
enum class Map
{

};
struct Room_Data
{
    int area;
    bool lock;
    cocos2d::Vec2 pos;
    cocos2d::Size size;
    cocos2d::Vec2 offset;
    //debug�p
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
    MapGenerator();
    ~MapGenerator();

    void Call();

    const std::vector<Room_Data>& GetRoomData(void);
    const Triangle_Data GetDelaunayData(void);
    const std::vector<Node_Status> GetMSTNode(void);
    const std::list<MapDirection> FloorDir(void);
private:
    //�G���A���Ƃɐݒ肳��Ă��钆�S�_�̎��͂�Map�̂��ƂƂȂ镔�����쐬.
    bool Create_Room(void);
    //�d�Ȃ��Ă��镔����"BoidAlgorithm"��p���ĕ��U������.
    bool DistributedRoom(void);
    bool Create_Delaunay(void);
    bool Create_MST(void);
    std::vector<Room_Data> data_;
    Vertex_List vertex_;
    std::vector<int> areaData_;

    std::random_device seed_gen_;
    std::default_random_engine engine_;
    std::uniform_real_distribution<> dist_;
    std::uniform_int_distribution<> _dist_i;
    std::uniform_int_distribution<> _dist_n;

    Triangle_Data triangle_data;
    Delaunay* _delaunay = nullptr;
    MST* _mst = nullptr;
};