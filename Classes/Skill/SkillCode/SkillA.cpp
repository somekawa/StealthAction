#include <cmath>
#include "Generator/Geometry.h"
#include "Skill/SkillMng.h"
#include "SkillA.h"

USING_NS_CC;

SkillA::SkillA(SkillBase* ptr)
{
	pos_ = ptr->GetPlayerPos();
	effectData_.size = Size(180, 180);
	// ��ڲ԰�̕����̎擾
	auto direction = ptr->GetPlayerDirection();
	Vec2 offset = {0.0f,0.0f};
	float speed = 0.0f;
	bool flip = false;
	if (direction == Direction::Right)
	{
		flip = false;
		offset = Vec2(50.0f, 20.0f);
		speed = 5.0f;
	}
	if (direction == Direction::Left)
	{
		flip = true;
		offset = Vec2(-50.0f, 20.0f);
		speed = -5.0f;
	}

	// �̪�Ă��쐬���āA���g�̴̪�ĉ摜�Ɋi�[
	fx_ = lpEffectMng.PickUp("magic", Vec2{ 0.0f,0.0f }, pos_+offset, Vec2(speed,0), 6, 0.08f, flip, true);
}

SkillA::~SkillA()
{
}

void SkillA::UpDate(float delta)
{

	// ���̂Ƃ���̪�Ă���è�ޏ�Ԃ��Ɠ�����
	if (fx_.isActive_)
	{
		effectData_.origin = fx_.sprite_->getPosition();
	}

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

void SkillA::Init()
{
}

bool SkillA::GetActive(void)
{
	return false;
}

cocos2d::Vec2 SkillA::GetPlayerPos(void)
{
	return cocos2d::Vec2();
}

cocos2d::Vec2 SkillA::GetTargetPos(void)
{
	return cocos2d::Vec2();
}

cocos2d::Rect SkillA::GetEffectData(void)
{
	return effectData_;
}

SkillBase* SkillA::CreateSkillA(SkillBase* ptr)
{
	SkillBase* pRet = new(std::nothrow)SkillA(ptr);
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
