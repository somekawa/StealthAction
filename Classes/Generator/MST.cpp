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
	/*V��A����W���Ƃ���D*/
	Vertex_List A = vertexList_;
	Vertex_List V;
	//�O���t����C�ӂ̒��_���ЂƂI�сCV�ɉ�����D
	std::move(A.begin(), A.begin() + 1, std::back_inserter(V));
	A.erase(A.begin());
	//Node_List::iterator& itr = vertex_list.end();
	cocos2d::Vec2 itr;
	cocos2d::Vec2 choice_u;
	Edge_List min_edge;
	float min_distance = FLT_MAX;


	//V���O���t�̂��ׂĂ̒��_���܂ނ܂ŁC�ȉ����J��Ԃ��D
	while (vertexList_.size() > V.size())
	{
		for (auto u : V)
		{
			for (auto v : A)
			{
				auto distance = std::sqrt(lpGeometry.Distance_Calculator(v, u));
				if (min_distance > distance)
				{
					for (auto edge : edge_data)
					{
						Edge_List uvEdge = { u, v };
						//V�Ɋ܂܂�钸�_u�Ɗ܂܂�Ȃ����_v�����ԏd�݂��ŏ��̕�(u, v)���O���t����I�сAE�ɉ�����D
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
		//������v��V�ɉ�����D
		V.emplace_back(itr);
		A.erase(a, A.end());

		//�ŏI�I�ɃO���t(V, E)���ŏ��S��؂ƂȂ�D
		node.push_back(min_edge);

		min_distance = FLT_MAX;
	}


	for (auto& edge : edge_data)
	{
		//edge_data�̏d������
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
	// �m�[�h���X�g�Ƃ��������N���Ă��邩�̃��X�g�̍쐬
	for (auto& v : vertexList_)
	{
		Node_Status nodeA;
		nodeA.id = i;
		nodeA.key = v;
		for (auto& edge : edge_data)
		{
			if (!edge.used)
			{
				continue;
			}
			if ((edge.pair_vertex[0] == v || edge.pair_vertex[1] == v))
			{
				
				if (edge.pair_vertex[0] != v)
				{
					nodeA.childData.push_back({nodeA.id, edge.pair_vertex[0], true, MapDirection::Max });
				}
				else
				{
					nodeA.childData.push_back({nodeA.id, edge.pair_vertex[1], true, MapDirection::Max });
				}
				
			}
		}
		if (nodeA.childData.size() >= 5)
		{
			cocos2d::log("�����Ă���");
			node.clear();
			node.reserve(0);
			engine.seed(0);
			Choice_Node();

		}
		
		nodeList_.push_back(nodeA);
		i++;
	}
	
	//std::shuffle(edge_data.begin(), edge_data.end(), engine);
	std::uniform_int_distribution<>::param_type param_i2(0, 10);
	dist.param(param_i2);

	// �G�b�W��ǉ�����
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

					if (nodeList_[i].childData.size() < 3 && nodeList_[i].childData.size() < 3 && dist(engine) <= 1)
					{
						for (auto& status : nodeList_)
						{
							if (status.key == edge.pair_vertex[0] )
							{
								status.childData.push_back({0, edge.pair_vertex[1], true, MapDirection::Max });

							}
							if (status.key == edge.pair_vertex[1])
							{
								status.childData.push_back({0, edge.pair_vertex[0], true, MapDirection::Max });
							}							
						}
						node.emplace_back(edge.pair_vertex);
					}
					break;
				}
			}
		}
	}

	// �q���Ɏ���ID���Z�b�g
	for (auto& status : nodeList_)
	{
		for (auto& child : status.childData)
		{
			auto itr = std::find_if(nodeList_.begin(), nodeList_.end(),
				[child](Node_Status& node)
				{
					return node.key == child.pair_node;
				});

			if (itr != nodeList_.end())
			{
				child.childId = itr->id;
				break;
			}
		}
	}
	int idx = 0;
	for (int i = 0; i < nodeList_.size(); i++)
	{
		auto dir = MapDirection::Max;

		for (auto& pair : nodeList_[i].childData)
		{
			//���S�Ɍ������x�N�g��
			// ����u������
			auto vec = pair.pair_node - nodeList_[i].key;
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
			
			pair.dir = dir;

			// �G���A���b�N
			for (int j = 0; j < vertexList_.size(); j++)
			{
				if (pair.pair_node == vertexList_[j] && areaData_[i] != areaData_[j])
				{
					pair.lock = false;
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
