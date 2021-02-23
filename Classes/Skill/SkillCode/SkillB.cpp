#include <cmath>
#include "Generator/Geometry.h"
#include "Skill/SkillMng.h"
#include "SkillB.h"

USING_NS_CC;

SkillB::SkillB(SkillBase* ptr)
{
	// add�ł��Ȃ�
	pos_ = ptr->GetPlayerPos();
	tpos_ = ptr->GetTargetPos();
	effectData_.size = Size(192*0.6,192*0.6);
	// ��ڲ԰�̕����̎擾
	auto direction = ptr->GetPlayerDirection();
	auto flip = false;
	if (direction == Direction::Right)
	{
		flip = true;
	}
	if (direction == Direction::Left)
	{
		flip = false;
	}

	auto Pos = Vec2(pos_.x * pos_.x, pos_.y * pos_.y);
	auto Tpos = Vec2(tpos_.x * tpos_.x, tpos_.y * tpos_.y);
	auto tvec = Tpos - Pos;
	auto vec = tpos_ - pos_;
	auto nvec = Vec2(1, 0);
	auto nvec2 = tvec / hypot(vec.x, vec.y);
	auto cos = lpGeometry.Dot(nvec, nvec2);
	auto sin = lpGeometry.Cross(nvec, nvec2);
	dir_ = Vec2(cos, sin);
	//dir_ = dir_.getNormalized() * 2;
	// �̪�Ă��쐬���āA���g�̴̪�ĉ摜�Ɋi�[
	fx_ = lpEffectMng.PickUp("enemySpawn", Vec2{ 0.0f,0.0f },pos_,Vec2(0,0), 19, 0.08f,flip,false,true);
	fx_.sprite_->setScale(0.6f);
	//lpEffectMng.InitializePickedUpEffect("enemySpawn", pos_, dir, 19, 0.08f, flip, true);
	// �̪�Ă̍Đ�
	//lpEffectMng.PlayWithLoop(fx_, "enemySpawn");

	time_ = 0.0f;

	param.removeFlg = false;
	param.activation = true;
	param.name = "enemySpawn";
	param.ct = 180.0f;
}

SkillB::~SkillB()
{
}

void SkillB::UpDate(float delta)
{
	if (!param.activation)
	{
		if (param.ct > 0)
		{
			param.ct--;
		}
		else
		{
			param.ct = 0;
			param.removeFlg = true;
		}
	}

	time_ += delta;
	dir_ = dir_.getNormalized() * 7;
	fx_.sprite_->setPosition(fx_.sprite_->getPosition() + dir_);
	if (param.activation)
	{
		// ��Βl���Ƃ�A�G�ɏՓ˂������͎��Ԍo�߂ŏ�����悤�ɂ��Ă���
		if ((abs(tpos_.x - fx_.sprite_->getPosition().x) <= 60 && abs(tpos_.y - fx_.sprite_->getPosition().y) <= 60) || time_ >= 19 * 0.08)
		{
			// active��Ԃ�false�ɂ���visible��s���ɂ���
			//fx_.isActive_ = false;
			//fx_.sprite_->setVisible(false);
			// SkillBase��removeFromParent�̏����𖞂������߂ɐ؂�ւ���
			param.activation = false;
		}
	}

	// ���̂Ƃ���̪�Ă���è�ޏ�Ԃ��Ɠ�����
	//if (fx_.isActive_)
	//{
	//	effectData_.origin = fx_.sprite_->getPosition();
	//}

	/*���̒���Skill�̌��ʂ⓮����L�q���Ă�������*/
	// �����Ŵ̪�Ă��߼޼�݂̈ړ������
	// ��Ұ��݂��I���ƴ̪�Ă̱�è�ޏ�Ԃ�false�ɂ��Ă���

	bool flag = true;
	/*if (lpEffectMng.AnimEndChecker(fx_, delta))
	{
		param.activation = false;
		flag = lpSkillMng.SkillInActivate("skill_01.txt");
	}*/

}

void SkillB::Init()
{
}

bool SkillB::GetActive(void)
{
	return false;
}

cocos2d::Vec2 SkillB::GetPlayerPos(void)
{
	return cocos2d::Vec2();
}

cocos2d::Vec2 SkillB::GetTargetPos(void)
{
	return cocos2d::Vec2();
}

cocos2d::Rect SkillB::GetEffectData(void)
{
	return effectData_;
}

SkillBase* SkillB::CreateSkillB(SkillBase* ptr)
{
	SkillBase* pRet = new(std::nothrow)SkillB(ptr);
	if (pRet)
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
