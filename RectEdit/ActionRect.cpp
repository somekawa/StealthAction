#include <DxLib.h>

#include "ActionRect.h"

ActionRect::ActionRect(Vector2I begin, Vector2I end, Vector2I offset, char type, int frame)
{
	beginPos_ = begin;
	endPos_ = end;
	data_ = RectData(begin-offset, end-offset, end - begin, type, frame);
	switch (type)
	{
	case 0:
		boxColor_ = 0xff0000;
		break;
	case 1:
		boxColor_ = 0x0000ff;
		break;
	default:
		break;
	}
}

ActionRect::~ActionRect()
{
}

void ActionRect::Set(Vector2I begin,
					 Vector2I end,
					 RectType type, 
					 int frame,
					 std::string actionName)
{

}

void ActionRect::Draw(void)
{
	DrawBox(beginPos_.x,beginPos_.y,endPos_.x,endPos_.y, boxColor_, false);
}
