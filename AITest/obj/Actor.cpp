#include "Actor.h"
#include "Scene/GameScene.h"
#include "Gravity.h"
#include "_Debug/_DebugConOut.h"

USING_NS_CC;

//Actor::Actor()
//{
//}

Actor::Actor()
{
	deleteFlag_ = false;
	isAnimEnd_ = false;
	onFloor_ = false;

	animationFrame_ = 0.0f;
	// �������g���Q�Ƃœn���A����
	gravity_ = std::make_unique<Gravity>(*this);
}

Actor::~Actor()
{
}

void Actor::Update(void)
{

	Action();
}



std::string Actor::GetAction(void)
{
	return currentAnimation_;
}

void Actor::SetAction(std::string actName)
{
	currentAnimation_ = actName;
}

void Actor::SetPos(Vector2I pos)
{
	pos_ = pos;
}

void Actor::SetDirection(Direction dir)
{
	direction_ = dir;
}

const int& Actor::GetAnimationFrameInt(void)
{
	// ���t���[�����Z�����l(animationFrame)��1�t���[���ɗv���鎞��(delayPerUnit)������
	// delayPerUnit�Ŋ���ƌ��݂̃t���[���l��int�Ŏ擾�\
	auto delay = lpAnimMng.GetAnimationCache(type_, currentAnimation_)->getDelayPerUnit();
	auto val = animationFrame_;
	return val;
}

bool Actor::CheckObjHit(void)
{
	auto director = Director::getInstance();
	if ((TMXLayer*)director->getRunningScene()->getChildByTag((int)zOlder::BG)->getChildByName("MapData") == nullptr)
	{
		return false;
	}
	const int chipSize = 48;
	auto CollisionData = (TMXLayer*)director->getRunningScene()->getChildByTag((int)zOlder::BG)->getChildByName("MapData")->getChildByName("col");
	auto ColSize = CollisionData->getLayerSize();
	// �����ׂ̗�����
	auto NextPos = [&]() {
		Vector2I next;
		switch (direction_)
		{
		case Direction::Right:
			next = { pos_.x + 1,pos_.y };
			break;
		case Direction::Left:
			next = { pos_.x + 1,pos_.y };
			break;
		case Direction::Max:
			break;
		default:
			break;
		}
		return Vector2I(next.x / chipSize, next.y / chipSize);
	};

	if (CollisionData->getTileGIDAt(Vec2(NextPos().x, NextPos().y)) != 0)
	{
		//onFloor_ = true;
		return true;
	}

	return false;
}

bool Actor::CheckOnFloor(void)
{
	auto director = Director::getInstance();
	if ((TMXLayer*)director->getRunningScene()->getChildByTag((int)zOlder::BG)->getChildByName("MapData") == nullptr)
	{
		return false;
	}
	const int chipSize = 48;
	auto CollisionData = (TMXLayer*)director->getRunningScene()->getChildByTag((int)zOlder::BG)->getChildByName("MapData")->getChildByName("col");
	auto ColSize = CollisionData->getLayerSize();

	auto chipPos = Vector2I(pos_.x / chipSize, ((pos_.y) / chipSize) -1);

	if (CollisionData->getTileGIDAt(Vec2(chipPos.x,chipPos.y)) != 0)
	{
		onFloor_ = true;
	}
	else
	{
		onFloor_ = false;
	}

	return onFloor_;
}
