#include <random>
#include <cmath>
#include "Geometry.h"
#include "Delaunay.h"
#include "MST.h"
#include <iostream>
USING_NS_CC;
using namespace std;

constexpr auto FLOOR = 150;

MST::MST(std::vector<Edge_Status> data, std::vector<cocos2d::Vec2> vertex, int floor_cnt)
{
	std::uniform_int_distribution<>::param_type param_i(0, floor_cnt);
	engine.seed(seed_gen());
	dist.param(param_i);
	edge_data = data;
	vertex_list = vertex;
}

MST::~MST()
{
}

void MST::Choice_Node()
{
	/*VとAを空集合とする．*/
	Vertex_List A = vertex_list;
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
	while (vertex_list.size() > V.size())
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
	std::vector<Node_Status>nodeList;

	int a = 0;
	std::vector<int>alist;
	for (auto& v : vertex_list)
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
						nodeA.pair_node.push_back(edge.pair_vertex[0]);
					}
					else
					{
						nodeA.pair_node.push_back(edge.pair_vertex[1]);
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
		nodeList.push_back(nodeA);
		alist.push_back(a);
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
		for (int i = 0; i < vertex_list.size(); i++)
		{
			for (int j = i + 1; j < vertex_list.size(); j++)
			{
				
				Edge_List uvEdge = { vertex_list[i], vertex_list[j] };
				if ((edge.pair_vertex[0] == uvEdge[0] && edge.pair_vertex[1] == uvEdge[1])
					|| (edge.pair_vertex[0] == uvEdge[1] && edge.pair_vertex[1] == uvEdge[0]))
				{
					
					if (alist[i]  < 3 && alist[j] < 3 && dist(engine) <= 1)
					{
						alist[i]++;
						alist[j]++;
						//edge.used = true;
						node.emplace_back(edge.pair_vertex);
					}
					break;
				}
			}
		}
	}
}

std::vector<std::array<cocos2d::Vec2, 2>> MST::GetNode()
{
	return node;
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
