#pragma once
#include <vector>
#include <array>
#include <map>
#include <memory>
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

	void CreateMST();
	
	// 最も短いエッジを返す
	Edge_List FindShortest(cocos2d::Vec2& v, cocos2d::Vec2& usedvert, float& min_distance, Edge_List& min_edge);
	std::vector<Node_Status> GetNode();
	std::vector<Edge_Status> GetEdgeData();
	std::vector<Edge_Status> edgeData;		//edge情報を保存
private:
	// プリム法を用いてMSTを作成
	void MakeMSTforPrim();
	// edge_dataとminEdgeListの重複判定
	// 重複していたら使用済みにする
	void UsedIfDuplicate();
	// 親から繋がっている子を作成する
	void CreateLinkNode();
	//一度削除したノードの一部復元
	void RevertPartofEdge();
	//Mapの管理クラスに渡す前の下処理
	void NextFancPrepation(void);
	Vertex_List vertexList_;
	std::vector<int> areaData_;
	std::vector<Edge_List> minEdgeList_;
	std::vector<Node_Status> nodeList_;
	std::random_device seedGen_;
	std::default_random_engine engine;
	std::uniform_int_distribution<> dist;
};

bool operator == (const Edge_List& edge, const Edge_List& edge1);

