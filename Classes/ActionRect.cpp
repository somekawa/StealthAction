#include <cocos2d.h>

#include "ActionRect.h"

USING_NS_CC;

ActionRect::ActionRect(Vector2I begin, Vector2I end, char type, int frame)
{
	beginPos_ = begin;
	endPos_ = end;
	data_ = RectData(begin,end, end - begin, type, frame);

	pos_ = { data_.size_.x / 2,data_.size_.y / 2 };
	switch (type)
	{
	case 0:
		boxColor_ = Color4F::RED;
		break;
	case 1:
		boxColor_ = Color4F::BLUE;
		break;
	default:
		break;
	}
}

ActionRect::~ActionRect()
{
}

DrawNode* ActionRect::CreateCollider(void)
{
	return ActionRect::create();
}

void ActionRect::Set(Vector2I begin,
	Vector2I end,
	char type,
	int frame,
	std::string actionName)
{

}
