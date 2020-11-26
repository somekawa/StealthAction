#pragma once
#include <vector>
#include <array>
#include <map>
#include "Direction/Direction.h"
#include "cocos2d.h"

using Edge_List = std::array<cocos2d::Vec2,2>;
using Vertex_List = std::vector<cocos2d::Vec2>;
//using Lock_List = std::pair<cocos2d::Vec2, bool>;

struct NodeChild
{
	int childId;
	cocos2d::Vec2 pair_node;	//繋がっているnodeの座標
	bool lock;	// 移動できるか
	MapDirection dir;
};

struct Node_Status
{
	int id;
	cocos2d::Vec2 key;	
	std::vector<NodeChild>childData;
};


struct Edge_Status
{
	float distance;
	Edge_List pair_vertex;	//繋がっているnodeの座標
	bool used;
};

class Delaunay;
//Prim's Minimum Spanning Tree
//最小スパニングツリー (プリム法)
class MST
{
public:
	MST(std::vector<Edge_Status> data, Vertex_List vertex,std::vector<int> areaData,int floor_cnt);
	~MST();

	void Choice_Node();
	Edge_List FindShortest(cocos2d::Vec2& v, cocos2d::Vec2& usedvert, float& min_distance, Edge_List& min_edge);
	std::vector<Node_Status> GetNode();
	std::vector<Edge_Status> edge_data;		//edge情報を保存
private:

	Vertex_List vertexList_;
	std::vector<int> areaData_;
	std::vector<Edge_List> minEdgeList_;
	std::vector<Node_Status> nodeList_;

	Delaunay* delaunay_;

	std::random_device seedGen_;
	std::default_random_engine engine;
	std::uniform_int_distribution<> dist;
};

bool operator == (const Edge_List& edge, const Edge_List& edge1);

