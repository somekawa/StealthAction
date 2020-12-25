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
	edgeData = data;
	vertexList_ = vertex;
	areaData_ = areaData;
}

MST::~MST()
{
}

void MST::MakeMSTforPrim()
{
	/*V��A����W���Ƃ���D*/
	Vertex_List A = vertexList_;
	Vertex_List V;
	//�O���t����C�ӂ̒��_���ЂƂI�сCV�ɉ�����D
	std::move(A.begin(), A.begin() + 1, std::back_inserter(V));
	A.erase(A.begin());
	Edge_List min_edge;
	float min_distance = FLT_MAX;

	//V���O���t�̂��ׂĂ̒��_���܂ނ܂ŁC�ȉ����J��Ԃ��D
	while (vertexList_.size() > V.size())
	{
		for (auto& usedvert : V)
		{
			for (auto& unUsedvert : A)
			{
				FindShortest(unUsedvert, usedvert, min_distance, min_edge);
			}
		}
		// min_edge�̒��g��{unUsedvert,usedvert}������0�Ԗ�
		auto& minVert = min_edge[0];

		//������v��V�ɉ�����D
		V.emplace_back(minVert);
		//������A����v���폜
		auto& vertItr = remove_if(A.begin(), A.end(), [minVert](const Vec2& vert)
			{
				return vert == minVert;
			});
		if (vertItr == A.end())
		{
			continue;
		}
		
		A.erase(vertItr, A.end());

		//�ŏI�I�ɃO���t(V, E)���ŏ��S��؂ƂȂ�D
		minEdgeList_.push_back(min_edge);

		min_distance = FLT_MAX;
	}
}

void MST::CreateMST()
{
	MakeMSTforPrim();
	UsedIfDuplicate();
	CreateLinkNode();	
	RevertPartofEdge();
	NextFancPrepation();
}

void MST::UsedIfDuplicate()
{
	// edge_data��minEdgeList�̏d������
	// �d�����Ă�����g�p�ς݂ɂ���
	for (auto& edge : edgeData)
	{
		for (auto& minEdge : minEdgeList_)
		{
			if (edge.pair_vertex == minEdge)
			{
				edge.used = true;
				break;
			}
		}
	}
}

void MST::CreateLinkNode()
{
	int id = 0;
	// �m�[�h���X�g�Ƃ��������N���Ă��邩�̃��X�g�̍쐬
	for (auto& v : vertexList_)
	{
		Node_Status tmpNode;
		tmpNode.key = v;
		tmpNode.id = id;
		for (auto& edge : edgeData)
		{
			if (!edge.used)
			{
				continue;
			}
			if (edge.pair_vertex[0] == v || edge.pair_vertex[1] == v)
			{

				if (edge.pair_vertex[0] != v)
				{
					tmpNode.childData.push_back({ id, edge.pair_vertex[0], true, MapDirection::Max });
				}
				else
				{
					tmpNode.childData.push_back({ id, edge.pair_vertex[1], true, MapDirection::Max });
				}
			}
		}
		//������o�O���o���ꍇ�������Ȃ���
		if (tmpNode.childData.size() >= 5)
		{
			cocos2d::log("�����Ă���");
			minEdgeList_.clear();
			minEdgeList_.reserve(0);
			nodeList_.clear();
			engine.seed(0);
			CreateMST();

		}
		id++;
		nodeList_.push_back(tmpNode);

	}
}

void MST::RevertPartofEdge()
{
	//std::shuffle(edge_data.begin(), edge_data.end(), engine);
	std::uniform_int_distribution<>::param_type param_i2(0, 10);
	dist.param(param_i2);

	// �G�b�W��ǉ���
	for (auto edge : edgeData)
	{		
		if (edge.used)
		{
			
			continue;
		}
		for (int i = 0; i < nodeList_.size(); i++)
		{
			if (nodeList_[i].key != edge.pair_vertex[0] && nodeList_[i].key != edge.pair_vertex[1])
			{
				continue;
			}
			for (int j = i + 1; j < nodeList_.size(); j++)
			{
				Edge_List uvEdge = { nodeList_[i].key, nodeList_[j].key };
				if (!(edge.pair_vertex == uvEdge)|| nodeList_[i].childData.size() > 4 
					|| nodeList_[j].childData.size() > 4 || dist(engine) > 1)
				{
					continue;
				}
				for (auto& status : nodeList_)
				{
					if (status.key == edge.pair_vertex[0])
					{
						status.childData.push_back({ 0, edge.pair_vertex[1], true, MapDirection::Max });

					}
					if (status.key == edge.pair_vertex[1])
					{
						status.childData.push_back({ 0, edge.pair_vertex[0], true, MapDirection::Max });
					}
				}
				break;				
			}
		}
	}
}

void MST::NextFancPrepation(void)
{
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
			float angle = rad * (180 /M_PI);
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
				if (pair.pair_node == vertexList_[j] && areaData_[i] == areaData_[j])
				{
					pair.lock = false;
				}
			}
		}
	}
}

Edge_List MST::FindShortest(cocos2d::Vec2& unUsedvert, cocos2d::Vec2& usedvert, float& min_distance, Edge_List& min_edge)
{
	auto distance = std::sqrt(lpGeometry.Distance_Calculator(unUsedvert, usedvert));
	Edge_List tmpEdge = { unUsedvert, usedvert };
	if (min_distance > distance)
	{
		for (auto edge : edgeData)
		{
			//V�Ɋ܂܂�钸�_u�Ɗ܂܂�Ȃ����_v�����ԏd�݂��ŏ��̕�(u, v)���O���t����I�сAE�ɉ�����D
			if (edge.pair_vertex == tmpEdge)
			{
				min_distance = distance;
				min_edge = tmpEdge;
				return min_edge;
			}
		}
	}
	return min_edge;
}

std::vector<Node_Status> MST::GetNode()
{
	return nodeList_;
}

bool operator==(const Edge_List& edge, const Edge_List& edge1)
{
	if ((edge[0] == edge1[0]) && (edge[1] == edge1[1])
		||(edge[1] == edge1[0])&&(edge[0] == edge1[1]))
	{
		return true;
	}
	else
	{
		return false;
	}
}
