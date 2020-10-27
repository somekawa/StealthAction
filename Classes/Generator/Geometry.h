#pragma once
#include "cocos2d.h"

#define lpGeometry Geometry::GetInstance()

class Geometry
{
public:
	static Geometry& GetInstance(void)
	{
		static Geometry s_Instace;
		return s_Instace;
	}
	const float& Dot(cocos2d::Vec2 vec, cocos2d::Vec2 vec2);
	//ŠOÏ
	const float& Cross(const cocos2d::Vec2& point, const cocos2d::Vec2& vertex1, const cocos2d::Vec2& vertex2);
	//”¼Œa‚ğZo
	const float& Radius_Calculator(const cocos2d::Vec2& size);
	//distance‚ğ2æ‚µ‚½’l‚ğZo
	const float& Distance_Calculator(const cocos2d::Vec2& vertex, const cocos2d::Vec2& vertex1);

	bool CircleSegment(const cocos2d::Vec2& circle_center, const float& radius, const cocos2d::Vec2& line_start, const cocos2d::Vec2& line_end);
};

