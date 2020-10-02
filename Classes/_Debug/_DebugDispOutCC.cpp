#ifdef _DEBUG
#include <cocos2d.h>
#include <windows.h>
#include "_DebugDispOutCC.h"

USING_NS_CC;
#define _DEBUG_ZODER 999
std::unique_ptr<_DebugDispOutCC, _DebugDispOutCC::_DebugDispOutDeleter> _DebugDispOutCC::s_Instance(new _DebugDispOutCC);
bool _DebugDispOutCC::DrawRect(std::string spName, Vec2 pos, cocos2d::Vec2 posLU, cocos2d::Vec2 posRD, Color4F col)
{
	auto director = Director::getInstance();
	auto node = ((DrawNode*)director->getRunningScene()->getChildByName(spName));
	if (node == nullptr)
	{
		node = DrawNode::create();
		//node->drawPoint(Vec2(-15, -70), 1.0f, Color4F(1.0f, 1.0f, 1.0f, 1.0f));
		node->drawRect(posLU, posRD, col);
		node->setGlobalZOrder(_DbgScreen);
		node->setName(spName);
		director->getRunningScene()->addChild(node);
	}
	node->setPosition(pos);
	return false;
}
_DebugDispOutCC::_DebugDispOutCC()
{
	_DbgScreen = 1000;
}

_DebugDispOutCC::~_DebugDispOutCC()
{
}


#endif	// _DEBUG
