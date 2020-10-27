#pragma once
#include <vector>
#include <string>
constexpr auto FLOOR = 150;

struct Room_Data
{
    cocos2d::Vec2 pos;
    cocos2d::Size size;
    cocos2d::Vec2 offset;
    //debug�p
    std::string name;
    cocos2d::Color4F line_color;
    cocos2d::Color4F fill_color;
};
class Delaunay;
class MapGenerator
{
public:
    MapGenerator();
    ~MapGenerator();

    void Call();

    const std::vector<Room_Data>& GetRoomData(void);
    const Triangle_Data GetDelaunayData(void);
    const std::vector<std::array<cocos2d::Vec2, 2>> GetMSTNode(void);
private:
    //�G���A���Ƃɐݒ肳��Ă��钆�S�_�̎��͂�Map�̂��ƂƂȂ镔�����쐬.
    bool Create_Room(void);
    //�d�Ȃ��Ă��镔����"BoidAlgorithm"��p���ĕ��U������.
    bool DistributedRoom(void);
    bool Create_Delaunay(void);
    bool Create_MST(void);
    std::vector<Room_Data> _data;
    std::vector<cocos2d::Vec2> _vertex;

    std::random_device _seed_gen;
    std::default_random_engine _engine;
    std::uniform_real_distribution<> _dist;
    std::uniform_int_distribution<> _dist_i;
    std::uniform_int_distribution<> _dist_n;

    Triangle_Data triangle_data;
    Delaunay* _delaunay = nullptr;
    MST* _mst = nullptr;
};