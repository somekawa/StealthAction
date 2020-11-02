#include <random>
#include <cmath>
#include "Geometry.h"
#include "Delaunay.h"
#include "MST.h"
/*
VとEを空集合とする．
グラフから任意の頂点をひとつ選び，Vに加える．
Vがグラフのすべての頂点を含むまで，以下を繰り返す．
Vに含まれる頂点uと含まれない頂点vを結ぶ重みが最小の辺(u,v)をグラフから選び，Eに加える．そしてvをVに加える．
最終的にグラフ(V,E)が最小全域木となる．*/
MST::MST(std::vector<Edge_Status> data, std::vector<cocos2d::Vec2> vertex, int floor_cnt)
{
	std::uniform_int_distribution<>::param_type param_i(0, floor_cnt);
	engine.seed(seed_gen());
	dist.param(param_i);
	edge_data = data;
	vertex_list = vertex;

	start = dist(engine);
}

MST::~MST()
{
}

void MST::Choice_Node()
{
	Vertex_List A = vertex_list;
	Vertex_List V;
	std::move(A.begin(), A.begin() + 1, std::back_inserter(V));
	A.erase(A.begin());
	Node_List::iterator itr;
	cocos2d::Vec2 choice_u;
	Edge_List min_edge;
	float min_distance = FLT_MAX;
	int size = vertex_list.size();

	auto s = vertex_list[0];
	std::vector<Edge_Status> distance_list;
	for (auto itr = vertex_list.begin(); itr != vertex_list.end() - 1; itr++)
	{
		auto itr2 = std::next(itr);
		distance_list.emplace_back(Edge_Status{ std::sqrt(lpGeometry.Distance_Calculator(*itr, *itr2)),Edge_List{*itr, *itr2} });
	}

	auto end_itr = distance_list.end();
	std::vector<cocos2d::Vec2> used_vertex;
	used_vertex.emplace_back(vertex_list[0]);
	while (end_itr != distance_list.begin())
	{
		auto mincost = std::min_element(distance_list.begin(), end_itr,
			[](const auto& a, const auto& b) {return (a).distance < (b).distance; });

		for (auto d : distance_list)
		{
			if (s != d.pair_vertex[0] && s != d.pair_vertex[1])
			{
				continue;
			}
			if ((mincost->distance == d.distance)/* && (*mincost).pair_vertex == d.pair_vertex*/)
			{
				node.emplace_back(d.pair_vertex);
			}
		}

		end_itr = std::remove_if(distance_list.begin(), end_itr,
			[=](const auto& a) {return a.distance == (*mincost).distance; });

		//distance_list.erase(a, distance_list.end());

		//for (auto u : V)
		//{
		//	for (auto v = A.begin(); v < A.end(); ++v)
		//	{
		//		//auto distance = std::sqrt(lpGeometry.Distance_Calculator(*v, u));

		//		for (auto edge : edge_data)
		//		{
		//			if (min_distance > distance)
		//			{
		//				Edge_List uvEdge = { u, *v };
		//				if ((edge.pair_vertex[0] == uvEdge[0] && edge.pair_vertex[1] == uvEdge[1])
		//					|| (edge.pair_vertex[0] == uvEdge[1] && edge.pair_vertex[1] == uvEdge[0]))
		//				{
		//					min_distance = distance;
		//					min_edge = Edge_List{ u, *v };
		//					itr = v;
		//				}
		//				else
		//				{

		//					//continue;
		//				}
		//			}
		//		}
		//		choice_u = u;
		//	}

		//	//std::move(itr, itr + 1, std::back_inserter(V));
		//	if (&itr != nullptr)
		//	{
		//		auto i = (&itr);
		//		//V.emplace_back(&(*itr));
		//		std::array<cocos2d::Vec2, 2> tmp;
		//		
		//		tmp = { choice_u, cocos2d::Vec2(&itr->x, &itr->y) };
		//		A.erase(itr);
		//		
		//		node.emplace_back(tmp);
		//	}
		//}
		//min_distance = FLT_MAX;
	}

	/*std::uniform_int_distribution<>::param_type param_i2(0, 15);
	dist.param(param_i2);
	for (auto edge2 : node)
	{
		auto used = false;
		for (auto edge : edge_data)
		{
			if ((edge2 != edge.pair_vertex))
			{
				if (used)
				{
					break;
				}

				if ((dist(engine) <= 1))
				{
					used = true;
					node.emplace_back(edge.pair_vertex);
				}
			}
		}
		if (used)
		{
			continue;
		}
	}*/
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
