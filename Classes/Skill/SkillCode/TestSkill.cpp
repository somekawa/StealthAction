#include "Skill/SkillMng.h"
#include "TestSkill.h"

USING_NS_CC;

TestSkill::TestSkill(SkillBase* ptr)
{
	// addできない
	ptr->AddActiveSkill(this);
	pos_ = ptr->GetPlayerPos();
	// ｴﾌｪｸﾄを作成して、自身のｴﾌｪｸﾄ画像に格納
	fx_ = lpEffectMng.createEffect("enemySpawn", 19, 0.08f, Vec2{ 0.0f,0.0f }, pos_,true);
	// ｴﾌｪｸﾄの再生
	lpEffectMng.PlayWithOnce(fx_, "enemySpawn");
}

TestSkill::~TestSkill()
{
}

void TestSkill::UpDate(float delta)
{
	// 今のところｴﾌｪｸﾄがｱｸﾃｨﾌﾞ状態だと動かす
	if (fx_.isActive_)
	{
		// ｽﾋﾟｰﾄﾞがおかしいので後で直す
		// この問題はtestskill自体を消したら直る
		// ｴﾌｪｸﾄを動かす(動かさないｴﾌｪｸﾄの場合は書く必要なし)
		lpEffectMng.Move(fx_.sprite_, Vec2(1, 0));
	}
	/*この中にSkillの効果や動作を記述してください*/
	// ここでｴﾌｪｸﾄのﾎﾟｼﾞｼｮﾝの移動をやる
	// ｱﾆﾒｰｼｮﾝが終わるとｴﾌｪｸﾄのｱｸﾃｨﾌﾞ状態をfalseにしている
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