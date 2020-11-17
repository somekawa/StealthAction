#pragma once
#include <vector>
#include <array>
#include <map>
#include "Direction/Direction.h"
#include "cocos2d.h"

using Edge_List = std::array<cocos2d::Vec2,2>;
using Vertex_List = std::vector<cocos2d::Vec2>;
using Lock_List = std::vector<std::pair<cocos2d::Vec2, bool>>;

struct Node_Status
{
	cocos2d::Vec2 key;
	bool lock;				//����node�ƂȂ����Ă��邩
	Lock_List pair_node;	//�q�����Ă���node�̍��W
	std::list<MapDirection> dirList;
};

struct Edge_Status
{
	float distance;
	Edge_List pair_vertex;	//�q�����Ă���node�̍��W
	bool used;
};

class Delaunay;
//Prim's Minimum Spanning Tree
//�ŏ��X�p�j���O�c���[ (�v�����@)
class MST
{
public:
	MST(std::vector<Edge_Status> data, Vertex_List vertex,std::vector<int> areaData,int floor_cnt);
	~MST();

	void Choice_Node();
	std::vector<Node_Status> GetNode();
	std::vector<Edge_Status> edge_data;		//edge����ۑ�
private:
	Vertex_List vertexList_;
	std::vector<int> areaData_;
	std::vector<Edge_List> node;
	std::vector<Node_Status> nodeList_;

	Delaunay* delaunay_;

	std::random_device seedGen_;
	std::default_random_engine engine;
	std::uniform_int_distribution<> dist;
};

bool operator == (const Edge_List& edge, const Edge_List& edge1);

