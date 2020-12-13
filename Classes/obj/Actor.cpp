#include "Actor.h"
#include "Scene/GameScene.h"
#include "Gravity.h"
#include "_Debug/_DebugConOut.h"
#include "ActionRect.h"

USING_NS_CC;

//Actor::Actor()
//{
//}

Actor::Actor(int hp):hp_(hp)
{
	deleteFlag_ = false;
	isAnimEnd_ = false;
	onFloor_ = false;
	isHitWall_ = false;
	// �U�����Ă��邩�̃t���O�̏�����
	isAttacking_ = false;
	// �_���[�W����������t���O�̏�����
	onDamaged_ = false;
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

void Actor::CheckMapObjHit(float delta)
{
	auto director = Director::getInstance();
	auto a = director->getRunningScene()->getChildByTag((int)zOlder::BG);
	if ((TMXLayer*)director->getRunningScene()->getChildByTag((int)zOlder::BG)->getChildByName("MapData") == nullptr)
	{
		return;
	}
	auto position = this->getPosition();
	const int chipSize = 48;
	auto CollisionData = (TMXLayer*)director->getRunningScene()->getChildByTag((int)zOlder::BG)->getChildByName("MapData")->getChildByName("col");
	// �R���W�������C���[�̏c��
	auto ColSize = CollisionData->getLayerSize();

	auto colSize = Vec2::ZERO;

	for (auto col : currentCol_)
	{
		if (col->GetData().type_ == 1)
		{
			colSize = Vec2{ (float)col->GetData().size_.x,(float)col->GetData().size_.y };
		}
	}
	this->setAnchorPoint({ 0.5f,0.0f });
	// �����ׂ̗��ǂ������łȂ���
	auto checkID = [&](Vec2 point) {
		auto gridPos = Vec2{ point } / chipSize;
		auto bottom = Vec2(gridPos.x, ColSize.height - gridPos.y);
		auto posGid = CollisionData->getTileGIDAt(bottom);
		if (posGid != 0)
		{
			// �␳���K�v�ȂƂ���true
			return true;
		}
		else
		{
			return false;
		}
		return false;
	};
	// �����̃|�W�V����
	auto bottomPos = Vec2{ position.x,position.y - (0.3f * (delta * 50)) };
	// �����̉��ׂ̃|�W�V����
	// speed_��dir�ɂ����-�ɂȂ�����+�ɂȂ����肷��̂ŕϐ������p
	auto nextPos = Vec2{ position.x + speed_.x,position.y };
	// �����̃|�W�V��������ɏ��ڒP�ʂɒ���
	auto nextYGrid = Vec2{ bottomPos.x,bottomPos.y } / chipSize;
	// ���ׂ̃|�W�V��������ɏ��ڒP�ʂɒ���
	auto nextXGrid = Vec2{ nextPos.x,nextPos.y } / chipSize;

	auto gridYPos = Vec2(nextYGrid.x, ColSize.height - nextYGrid.y);
	auto gridYGid = CollisionData->getTileGIDAt(nextYGrid);

	auto gridXPos = Vec2(nextXGrid.x, ColSize.height - nextXGrid.y);
	auto gridXGid = CollisionData->getTileGIDAt(nextXGrid);
	// �͈͊Ocheck
	if (gridXPos.x > ColSize.width || gridXPos.x < 0 ||
		gridXPos.y > ColSize.height || gridXPos.y < 0)
	{
		return;
	}
	// �͈͊Ocheck
	if (gridYPos.x > ColSize.width || gridYPos.x < 0 ||
		gridYPos.y > ColSize.height || gridYPos.y < 0)
	{
		return;
	}
	if (gridXGid != 0)
	{
		isHitWall_ = true;
	}
	else
	{
		isHitWall_ = false;
	}
	if (gridYGid != 0)
	{
		onFloor_ = true;
	}
	else
	{
		onFloor_ = false;
	}
}

void Actor::SetIsAttacking(bool flg)
{
	isAttacking_ = flg;
}

bool Actor::CheckHitAttack(const AttackRect& attackRect)
{
	if (currentCol_.size() <= 0)
	{
		return false;
	}

	// �U����`�̂Ƃ�
	if (currentCol_[0]->GetData().type_ == 0)
	{
		auto damageColData = currentCol_[0]->GetData();
		auto damageColPos = Vec2{ (float)damageColData.begin_.x + ((float)damageColData.size_.x / 2),
							 (float)damageColData.begin_.y + ((float)damageColData.size_.y / 2) };
		if (attackRect.pos_.x + (attackRect.size_.width / 2) - (getPosition().x + (float)damageColPos.x) <= 0 &&
			attackRect.pos_.y + (attackRect.size_.height / 2) - (getPosition().y + (float)damageColPos.y) <= 0)
		{
			return true;
		}
	}
	return false;
}
