#include "Collision/Collider.h"

USING_NS_CC;

Collider::Collider(Sprite& target):
	target_(target)
{
	pos_  = target_.getPosition();
	size_ = target_.getContentSize();
	this->setPosition(pos_);
}

Collider::~Collider()
{
}

Node* Collider::Create(Sprite& target)
{
	Collider* pRet = new(std::nothrow) Collider(target);
	if (pRet && pRet->init())
	{
		pRet->autorelease();
	}
	else
	{
		delete pRet;
		pRet = nullptr;
	}
	return pRet;
}