#pragma once
#include "MapObj.h"
#include <memory>

class ClearObj : public MapObj
{
public:
	ClearObj(cocos2d::Vec2 pos);
	~ClearObj();
	static ClearObj* CreateClearObj(cocos2d::Vec2 pos);
private:
	int frame = 0;
};

