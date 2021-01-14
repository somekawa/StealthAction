#include "Skill/SkillMng.h"
#include "TestSkill.h"

USING_NS_CC;

TestSkill::TestSkill(SkillBase* ptr)
{
	// add�ł��Ȃ�
	ptr->AddActiveSkill(this);
	pos_ = ptr->GetPlayerPos();
	// �̪�Ă��쐬���āA���g�̴̪�ĉ摜�Ɋi�[
	fx_ = lpEffectMng.createEffect("enemySpawn", 19, 0.08f, Vec2{ 0.0f,0.0f }, pos_,true);
	// �̪�Ă̍Đ�
	lpEffectMng.PlayWithOnce(fx_, "enemySpawn");
}

TestSkill::~TestSkill()
{
}

void TestSkill::UpDate(float delta)
{
	// ���̂Ƃ���̪�Ă���è�ޏ�Ԃ��Ɠ�����
	if (fx_.isActive_)
	{
		// ��߰�ނ����������̂Ō�Œ���
		// ���̖���testskill���̂��������璼��
		// �̪�Ă𓮂���(�������Ȃ��̪�Ă̏ꍇ�͏����K�v�Ȃ�)
		lpEffectMng.Move(fx_.sprite_, Vec2(1, 0));
	}
	/*���̒���Skill�̌��ʂ⓮����L�q���Ă�������*/
	// �����Ŵ̪�Ă��߼޼�݂̈ړ������
	// ��Ұ��݂��I���ƴ̪�Ă̱�è�ޏ�Ԃ�false�ɂ��Ă���
	lpEffectMng.AnimEndChecker(fx_, delta);

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