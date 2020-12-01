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
	// 自分自身を参照で渡し、生成
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
	// 毎フレーム加算される値(animationFrame)に1フレームに要する時間(delayPerUnit)を引き
	// delayPerUnitで割ると現在のフレーム値がintで取得可能
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
	// 自分の隣を見る
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
