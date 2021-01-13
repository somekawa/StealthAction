#include "Effect/EffectManager.h"
#include "Skill/SkillMng.h"
#include "TestSkill.h"

USING_NS_CC;

TestSkill::TestSkill(SkillBase* ptr)
{
	// addできない
	ptr->AddActiveSkill(this);
	pos_ = ptr->GetPlayerPos();
	// ｴﾌｪｸﾄを作成して、自身のｴﾌｪｸﾄ画像に格納
	effectSprite_ = lpEffectMng.createEffect("enemySpawn", 19, 0.08f, Vec2{ 0.0f,0.0f }, pos_);
	// ｴﾌｪｸﾄの再生
	lpEffectMng.Play(effectSprite_, "enemySpawn");
}

TestSkill::~TestSkill()
{
}

void TestSkill::UpDate(void)
{
	// ｽﾋﾟｰﾄﾞがおかしいので後で直す
	// ｴﾌｪｸﾄを動かす(動かさないｴﾌｪｸﾄの場合は書く必要なし)
	lpEffectMng.Move(effectSprite_, Vec2(1, 0));
	/*この中にSkillの効果や動作を記述してください*/
	// ここでｴﾌｪｸﾄのﾎﾟｼﾞｼｮﾝの移動をやる
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