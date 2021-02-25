#include "ClearObj.h"

ClearObj::ClearObj(cocos2d::Vec2 pos)
{
	own_ = Sprite::create("image/Sprites/map/gate.png");
	own_->setPosition(pos);
	own_->setAnchorPoint({ 0, 0 });
	own_->setScaleX(1.0f);
	own_->setScaleY(2.0f);
	this->addChild(own_);
	this->setName("clear");
	this->setVisible(false);
	isEnable_ = false;
}

ClearObj::~ClearObj()
{
}

ClearObj* ClearObj::CreateClearObj(cocos2d::Vec2 pos)
{
	ClearObj* pRet = new(std::nothrow) ClearObj(pos);
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

