#include "Effect/EffectManager.h"
#include "Skill/SkillMng.h"
#include "TestSkill.h"

USING_NS_CC;

TestSkill::TestSkill(SkillBase* ptr)
{
	// add�ł��Ȃ�
	ptr->AddActiveSkill(this);
	pos_ = ptr->GetPlayerPos();
	// �̪�Ă��쐬���āA���g�̴̪�ĉ摜�Ɋi�[
	effectSprite_ = lpEffectMng.createEffect("enemySpawn", 19, 0.08f, Vec2{ 0.0f,0.0f }, pos_);
	// �̪�Ă̍Đ�
	lpEffectMng.Play(effectSprite_, "enemySpawn");
}

TestSkill::~TestSkill()
{
}

void TestSkill::UpDate(void)
{
	// ��߰�ނ����������̂Ō�Œ���
	// �̪�Ă𓮂���(�������Ȃ��̪�Ă̏ꍇ�͏����K�v�Ȃ�)
	lpEffectMng.Move(effectSprite_, Vec2(1, 0));
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