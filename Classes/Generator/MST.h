#pragma once
#include <vector>
#include <array>
#include <map>
#include "cocos2d.h"

using Link_Map = std::map<cocos2d::Vec2, std::vector<cocos2d::Vec2>>;
using Edge_List = std::array<cocos2d::Vec2,2>;
using Node_List = std::vector<cocos2d::Vec2>;
using Vertex_List = std::vector<cocos2d::Vec2>;

struct Node_Status
{
	cocos2d::Vec2 key;
	bool link;				//����node�ƂȂ����Ă��邩
	Node_List pair_node;	//�q�����Ă���node�̍��W
};

struct Edge_Status
{
	float distance;
	Edge_List pair_vertex;	//�q�����Ă���node�̍��W
};

class Delaunay;
//Prim's Minimum Spanning Tree
//�ŏ��X�p�j���O�c���[ (�v�����@)
class MST
{
public:
	MST(std::vector<Edge_Status> data, std::vector<cocos2d::Vec2> vertex,int floor_cnt);
	~MST();

	void Choice_Node();
	std::vector<std::array<cocos2d::Vec2, 2>> GetNode();
	std::vector<Edge_Status> edge_data;		//edge����ۑ�
private:
	Link_Map link_vertex;
	Vertex_List vertex_list;
	std::vector<std::array<cocos2d::Vec2,2>> node;
	//std::map<cocos2d::Vec2,Node_Status> node;
	std::vector<Edge_Status> edge;

	int start;
	Delaunay* delaunay;

	std::random_device seed_gen;
	std::default_random_engine engine;
	std::uniform_int_distribution<> dist;
};

bool operator == (const Edge_List& edge, const Edge_List& edge1);

