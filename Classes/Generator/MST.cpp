#include <random>
#include <cmath>
#include "Geometry.h"
#include "Delaunay.h"
#include "MST.h"
#include <iostream>
USING_NS_CC;
using namespace std;

constexpr auto FLOOR = 150;

MST::MST(std::vector<Edge_Status> data, Vertex_List vertex, std::vector<int>areaData, int floor_cnt)
{
	std::uniform_int_distribution<>::param_type param_i(0, floor_cnt);
	engine.seed(/*seedGen_()*/0);
	dist.param(param_i);
	edge_data = data;
	vertexList_ = vertex;
	areaData_ = areaData;
}

MST::~MST()
{
}

void MST::Choice_Node()
{
	/*VとAを空集合とする．*/
	Vertex_List A = vertexList_;
	Vertex_List V;
	//グラフから任意の頂点をひとつ選び，Vに加える．
	std::move(A.begin(), A.begin() + 1, std::back_inserter(V));
	A.erase(A.begin());
	//Node_List::iterator& itr = vertex_list.end();
	cocos2d::Vec2 itr;
	cocos2d::Vec2 choice_u;
	Edge_List min_edge;
	float min_distance = FLT_MAX;


	//Vがグラフのすべての頂点を含むまで，以下を繰り返す．
	while (vertexList_.size() > V.size())
	{
		for (auto u : V)
		{
			for (auto v : A)
			{
				auto distance = std::sqrt(lpGeometry.Distance_Calculator(v, u));
				for (auto edge : edge_data)
				{
					if (min_distance > distance)
					{
						Edge_List uvEdge = { u, v };
						//Vに含まれる頂点uと含まれない頂点vを結ぶ重みが最小の辺(u, v)をグラフから選び、Eに加える．
						if ((edge.pair_vertex[0] == uvEdge[0] && edge.pair_vertex[1] == uvEdge[1])
							|| (edge.pair_vertex[0] == uvEdge[1] && edge.pair_vertex[1] == uvEdge[0]))
						{
							min_distance = distance;
							min_edge = uvEdge;
							itr = v;
						}
					}
				}
			}
		}
		Vertex_List::iterator a = remove_if(A.begin(), A.end(), [itr](const Vec2& a)
			{
				return a == itr;
			});
		if (a == A.end())
		{
			continue;
		}
		//そしてvをVに加える．
		V.emplace_back(itr);
		A.erase(a, A.end());

		//最終的にグラフ(V, E)が最小全域木となる．
		node.push_back(min_edge);

		min_distance = FLT_MAX;
	}


	for (auto& edge : edge_data)
	{
		//edge_dataの重複判定
		for (auto& n : node)
		{
			if ((edge.pair_vertex[0] == n[0] && edge.pair_vertex[1] == n[1])
				|| (edge.pair_vertex[0] == n[1] && edge.pair_vertex[1] == n[0]))
			{
				edge.used = true;
				break;
			}
		}
	}

	int i = 0;
	int a = 0;
	std::vector<int>alist;
	for (auto& v : vertexList_)
	{
		a = 0;
		Node_Status nodeA;
		nodeA.key = v;
		for (auto& edge : edge_data)
		{
			if ((edge.pair_vertex[0] == v || edge.pair_vertex[1] == v))
			{
				if (edge.used)
				{
					a++;
					if (edge.pair_vertex[0] != v)
					{
						nodeA.pair_node.push_back(std::make_pair(edge.pair_vertex[0], true));
					}
					else
					{
						nodeA.pair_node.push_back(std::make_pair(edge.pair_vertex[1], true));
					}
				}
			}
		}
		if (a >= 5)
		{
			cocos2d::log("超えている");
			node.clear();
			node.reserve(0);
			engine.seed(0);
			Choice_Node();

		}
		nodeList_.push_back(nodeA);
		alist.push_back(a);
		i++;
	}
	//std::shuffle(edge_data.begin(), edge_data.end(), engine);
	std::uniform_int_distribution<>::param_type param_i2(0, 10);
	dist.param(param_i2);
	for (auto edge : edge_data)
	{
		if (edge.used)
		{
			continue;
		}
		for (int i = 0; i < vertexList_.size(); i++)
		{
			for (int j = i+1; j < vertexList_.size(); j++)
			{

				Edge_List uvEdge = { vertexList_[i], vertexList_[j] };
				if ((edge.pair_vertex[0] == uvEdge[0] && edge.pair_vertex[1] == uvEdge[1])
					|| (edge.pair_vertex[0] == uvEdge[1] && edge.pair_vertex[1] == uvEdge[0]))
				{

					if (alist[i] < 3 && alist[j] < 3 && dist(engine) <= 1)
					{
						alist[i]++;
						alist[j]++;
						//edge.used = true;
						for (auto& status : nodeList_)
						{
							if (status.key == edge.pair_vertex[0] )
							{
								status.pair_node.push_back(std::make_pair(edge.pair_vertex[1], true));

							}
							if (status.key == edge.pair_vertex[1])
							{
								status.pair_node.push_back(std::make_pair(edge.pair_vertex[0], true));
							}
						}
						node.emplace_back(edge.pair_vertex);
					}
					break;
				}
			}
		}
	}
	int idx = 0;
	for (int i = 0; i < nodeList_.size(); i++)
	{
		auto dir = MapDirection::Max;

		for (auto& pair : nodeList_[i].pair_node)
		{
			//中心に向かうベクトル
			auto vec = pair.first - nodeList_[i].key;
			auto nvec1 = Vec2(1, 0)/*nodeList_[i].key / hypot(nodeList_[i].key.x, nodeList_[i].key.y)*/;
			auto nvec2 = vec / hypot(vec.x, vec.y);
			auto cos = lpGeometry.Dot(nvec1, nvec2);
			auto sin = lpGeometry.Cross(nvec1, nvec2);
			auto rad = atan2(sin, cos);
			float angle = rad * (180 / std::_Pi);
			if (angle < 0)
			{
				angle = angle + 360;
			}
			/*0~7*/
			dir = static_cast<MapDirection>(static_cast<int>((angle - 1) / 45));
			
			nodeList_[i].dirList.push_back(dir);

			for (int j = 0; j < vertexList_.size(); j++)
			{
				if (pair.first == vertexList_[j] && areaData_[i] != areaData_[j])
				{
					pair.second = false;
				}
			}
		}
	}
}

std::vector<Node_Status> MST::GetNode()
{
	return nodeList_;
}

bool operator==(const Edge_List& edge, const Edge_List& edge1)
{
	if ((edge[0] == edge1[0]) || (edge[1] == edge1[1])
		||(edge[1] == edge1[0])||(edge[0] == edge1[1]))
	{
		return true;
	}
	else
	{
		return false;
	}
}
