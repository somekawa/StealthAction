#include <cmath>
#include "Generator/Geometry.h"
#include "Skill/SkillMng.h"
#include "TestSkill.h"

USING_NS_CC;

TestSkill::TestSkill(SkillBase* ptr)
{
	// addできない
	pos_ = ptr->GetPlayerPos();
	tpos_ = ptr->GetTargetPos();
	effectData_.size = Size(192,192);
	// ﾌﾟﾚｲﾔｰの方向の取得
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
	// ｴﾌｪｸﾄを作成して、自身のｴﾌｪｸﾄ画像に格納
	fx_ = lpEffectMng.createEffect("enemySpawn", 19, 0.08f, Vec2{ 0.0f,0.0f }, pos_,true,true);
	// ｴﾌｪｸﾄの再生
	lpEffectMng.PlayWithLoop(fx_, "enemySpawn");
}

TestSkill::~TestSkill()
{
}

void TestSkill::UpDate(float delta)
{
	auto Pos = Vec2(pos_.x * pos_.x, pos_.y * pos_.y);
	auto Tpos = Vec2(tpos_.x * tpos_.x, tpos_.y * tpos_.y);
	auto tvec = Tpos - Pos;
	auto vec = tpos_ - pos_;
	auto nvec = Vec2(1,0);
	auto nvec2 = tvec / hypot(vec.x, vec.y);
	auto cos = lpGeometry.Dot(nvec, nvec2);
	auto sin = lpGeometry.Cross(nvec, nvec2);
	auto dir = Vec2(cos,sin);
	dir = dir.getNormalized() * 2;
	// 今のところｴﾌｪｸﾄがｱｸﾃｨﾌﾞ状態だと動かす
	if (fx_.isActive_)
	{
		// ｽﾋﾟｰﾄﾞがおかしいので後で直す
		// この問題はtestskill自体を消したら直る
		// ｴﾌｪｸﾄを動かす(動かさないｴﾌｪｸﾄの場合は書く必要なし)
		lpEffectMng.Move(fx_.sprite_, dir);
		effectData_.origin = fx_.sprite_->getPosition();
	}
	/*この中にSkillの効果や動作を記述してください*/
	// ここでｴﾌｪｸﾄのﾎﾟｼﾞｼｮﾝの移動をやる
	// ｱﾆﾒｰｼｮﾝが終わるとｴﾌｪｸﾄのｱｸﾃｨﾌﾞ状態をfalseにしている
	;

	bool flag = true;
	if (lpEffectMng.AnimEndChecker(fx_, delta))
	{
		param.activation = false;
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

cocos2d::Rect TestSkill::GetEffectData(void)
{
	return effectData_;
}

SkillBase* TestSkill::CreateTestSkill(SkillBase* ptr)
{
	SkillBase* pRet = new(std::nothrow)TestSkill(ptr);
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
