#include "Geometry.h"

const float& Geometry::Dot(cocos2d::Vec2 vec, cocos2d::Vec2 vec2)
{
	return (vec.x * vec2.x) + (vec.y * vec2.y);
}

const float& Geometry::Cross(const cocos2d::Vec2& point, const cocos2d::Vec2& vertex1, const cocos2d::Vec2& vertex2)
{	//			 pointへのxベクトル		 vertex1へのyベクトル	  vertexへのxベクトル		 pointへのyベクトル
	//return (point.x - vertex2.x) * (vertex1.y - vertex2.y) - (vertex1.x - vertex2.x) * (point.y - vertex2.y);
	//↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
	auto vec = point - vertex2;
	auto vec2 = vertex1 - vertex2;
	return (vec.x * vec2.y) - (vec2.x * vec.y);
}const float& Geometry::Cross(const cocos2d::Vec2& vec1, const cocos2d::Vec2& vec2)
{
	auto closs = (vec1.x * vec2.y) - (vec1.y * vec2.x);
	return closs;
}

const float& Geometry::Radius_Calculator(const cocos2d::Vec2& size)
{
	auto radius = sqrtf(pow(size.x, 2) + pow(size.y, 2)) / 2;
	return radius;
}

const float& Geometry::Distance_Calculator(const cocos2d::Vec2& vertex, const cocos2d::Vec2& vertex1)
{
	auto x = vertex.x - vertex1.x;
	auto y = vertex.y - vertex1.y;

	auto xx = x * x;
	auto yy = y * y;

	auto distance = xx + yy;
	return distance;
}

bool Geometry::CircleSegment(const cocos2d::Vec2& circle_center, const float& radius, const cocos2d::Vec2& line_start, const cocos2d::Vec2& line_end)
{
	//ベクトルを用意する
	auto start_to_center = circle_center - line_start;
	auto start_to_end	 = line_end		 - line_start;
	auto end_to_center	 = circle_center - line_end;
	//線分と円の中心の最短の長さを求める
	auto normal_start_to_end = start_to_end / hypot(start_to_end.x, start_to_end.y);
	auto distance = (start_to_center.x * normal_start_to_end.y) - (normal_start_to_end.x * start_to_center.y);
	//長さと円の長さの比較をする
	if (std::fabsf(distance) < radius)
	{
		//線分内に円があるかを調べる
		auto dot = Dot(start_to_center, start_to_end);
		auto dot2 = Dot(end_to_center, start_to_end);
		if (dot * dot2 <= 0.0f)
		{
			return true;
		}
		else if (Distance_Calculator(circle_center, line_start) < radius
				|| Distance_Calculator(circle_center,line_end) < radius)
		{
			return true;
		}
	}
	else
	{
		return false;
	}
}
