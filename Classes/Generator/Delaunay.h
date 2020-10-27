#pragma once
#include <array>
#include <vector>
#include <map>
#include "cocos2d.h"

class Geometory;

using Triangle = std::array<cocos2d::Vec2, 3>;

struct Edge_Status;
struct Triangle_Status
{
	Triangle vertex;
	float radius;
	cocos2d::Vec2 center;
};

using Edge_List = std::array<cocos2d::Vec2,2>;
using Triangle_Data = std::vector<Triangle_Status>;


class Delaunay
{
public:
	Delaunay();
	~Delaunay();

	const Triangle_Status& Create_Triangle(cocos2d::Vec2 pos,cocos2d::Vec2 size);

	const Triangle_Data Get_Triangle_Data(void);
	const void Subdivision_Triangle(cocos2d::Vec2 point);
	const std::vector<Edge_Status> Triangle_To_Edge(void);
	void FinishDelaunay(void);
private:
	const Triangle_Status Circumscribed_circle_Center(Triangle_Status triangle);
	//�O�p�`���ɒ��_���ǉ����ꂽ��
	Triangle_Data Check_Duplicative(Triangle_Data& data,Triangle_Data* duplicative_data,Triangle_Status status, cocos2d::Vec2 point);
	Triangle_Status triangle;	//���_3�ŎO�p�`��ۑ�
	Triangle_Status firstTriangle;	//�ŏ��̎O�p�`��ۑ�
	Triangle_Data triangle_data; //�ł����O�p�`��ۑ�
};
struct Triangle_Status;
bool operator==(const Triangle_Status& t, const Triangle_Status& t2);
bool ORCheck(const Triangle_Status& t, const Triangle_Status& t2);
