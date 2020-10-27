#include <random>
#include <cmath>
#include "Geometry.h"
#include "Delaunay.h"
#include "MST.h"
/*
V��E����W���Ƃ���D
�O���t����C�ӂ̒��_���ЂƂI�сCV�ɉ�����D
V���O���t�̂��ׂĂ̒��_���܂ނ܂ŁC�ȉ����J��Ԃ��D
V�Ɋ܂܂�钸�_u�Ɗ܂܂�Ȃ����_v�����ԏd�݂��ŏ��̕�(u,v)���O���t����I�сCE�ɉ�����D������v��V�ɉ�����D
�ŏI�I�ɃO���t(V,E)���ŏ��S��؂ƂȂ�D*/
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
	while (vertex_list.size() > V.size())
	{
		
		for (auto u : V)
		{
			for (auto v = A.begin(); v < A.end(); ++v)
			{
				auto distance = std::sqrt(lpGeometry.Distance_Calculator(*v, u));
				for (auto edge : edge_data)
				{
					if (min_distance > distance)
					{
						Edge_List uvEdge = { u, *v };
						if ((edge.pair_vertex[0] == uvEdge[0] && edge.pair_vertex[1] == uvEdge[1])
							|| (edge.pair_vertex[0] == uvEdge[1] && edge.pair_vertex[1] == uvEdge[0]))
						{
							min_distance = distance;
							min_edge = Edge_List{ u, *v };
							itr = v;
						}
						else
						{

							//continue;
						}
					}
				}
				choice_u = u;
			}

			//std::move(itr, itr + 1, std::back_inserter(V));
			if (itr._Myproxy != nullptr)
			{
				V.emplace_back(*itr);
				std::array<cocos2d::Vec2, 2> tmp{ choice_u, *itr };
				A.erase(itr);
				
				node.emplace_back(tmp);
			}
		}
		min_distance = FLT_MAX;
	}

	std::uniform_int_distribution<>::param_type param_i2(0, 15);
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
	}
}

std::vector<std::array<cocos2d::Vec2, 2>> MST::GetNode()
{
	return node;
}
