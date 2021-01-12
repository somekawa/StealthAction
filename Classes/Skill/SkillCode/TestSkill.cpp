#include "Effect/EffectManager.h"
#include "Skill/SkillMng.h"
#include "TestSkill.h"

USING_NS_CC;

TestSkill::TestSkill(SkillBase* ptr)
{
	// add�ł��Ȃ�
	ptr->AddActiveSkill(this);
	pos_ = ptr->GetPlayerPos();
	lpEffectMng.AddEffect("enemySpawn", 19, 0.08f, Vec2{ 0,0});
	// lpEffect.add�݂����Ȃ��Ƃ����
}

TestSkill::~TestSkill()
{
}

void TestSkill::UpDate(void)
{
	/*���̒���Skill�̌��ʂ⓮����L�q���Ă�������*/
	// �����Ŵ̪�Ă��߼޼�݂̈ړ������
	bool flag = true;
	if (lpEffectMng.GetAnimEnd())
	{
		flag = lpSkillMng.SkillInActivate("skill_01.txt");
	}
}

void TestSkill::Init()
{
}

bool TestSkill::GetActive(void)
{
	return false;
}

cocos2d::Vec2 TestSkill::GetPlayerPos(void)
{
	return cocos2d::Vec2();
}

cocos2d::Vec2 TestSkill::GetTargetPos(void)
{
	return cocos2d::Vec2();
}