#include "Obj.h"
#include "anim/AnimMng.h"
#include "_Debug/_DebugConOut.h"

USING_NS_CC;

Obj::Obj(Actor& target):
	target_(target)
{
	animationFrame_ = 0.0f;
	isAnimEnd_ = false;
	isHitTarget_ = false;
}

Obj::~Obj()
{
}

void Obj::update(float delta)
{
}

bool Obj::InitAnim(void)
{
	// ｱﾆﾒｰｼｮﾝの登録
	AnimRegistrator();
	return true;
}

bool Obj::HitToTarget(void)
{
	// ﾀｰｹﾞｯﾄのﾎﾟｼﾞｼｮﾝ
	const auto targetPos = target_.getPosition();
	// ﾀｰｹﾞｯﾄのｻｲｽﾞ
	const auto targetDamageCol = target_.GetDamageCol();
	const auto targetAttackCol = target_.GetAttackCol();
	// ﾀｰｹﾞｯﾄと自身(ｱﾀｯｸｵﾌﾞｼﾞｪｸﾄ)との距離
	const Vec2 distance{ targetDamageCol.origin.x - getPosition().x,targetDamageCol.origin.y - getPosition().y };
	const Vec2 distanceAttack{ targetAttackCol.origin.x - getPosition().x,targetAttackCol.origin.y - getPosition().y };
	// ﾀｰｹﾞｯﾄのｻｲｽﾞ/2 + 自身のｻｲｽﾞ/2
	const Vec2 diffAttack{ (targetAttackCol.size.width / 2.0f) + (getContentSize().width / 2.0f),((targetAttackCol.size.height / 2.0f) + 5.0f) + (getContentSize().height / 2.0f) };
	const Vec2 diff{ (targetDamageCol.size.width / 2.0f) + (getContentSize().width / 2.0f),((targetDamageCol.size.height/ 2.0f) + 5.0f) + (getContentSize().height / 2.0f) };
	// 当たっている
	if (abs(distanceAttack.x) <= diffAttack.x && abs(distanceAttack.y) <= diffAttack.y)
	{
		target_.SetIsHitAttack(true);
		// 当たったﾌﾗｸﾞをtrueに
		isHitTarget_ = true;
		return true;
	}
	// 当たっている
	if (abs(distance.x) <= diff.x && abs(distance.y) <= diff.y)
	{
		target_.SetIsHitAttack(true);
		// 当たったﾌﾗｸﾞをtrueに
		isHitTarget_ = true;
		return true;
	}
	// 当たっていない
	return false;
}

void Obj::UpdateAnimation(float delta)
{
	// アニメーションカウントを毎フレームdelta値を加算
	animationFrame_ += delta;
	// あるアニメーションの終了までの継続時間の格納
	const auto duration = lpAnimMng.GetAnimationCache(currentAnimation_)->getDuration();
	// アニメーションカウントが継続時間を超えていれば
	if (animationFrame_ >= duration)
	{
		// ループフラグがtrueの場合はループ再生
		if (lpAnimMng.GetIsLoop(currentAnimation_))
		{
			animationFrame_ = 0.0f;
		}
		else
		{
			// falseの場合は1回の再生
			// 1アニメーションが終了したフラグを立てる
			isAnimEnd_ = true;
		}
	}
}

void Obj::ChangeAnimation(std::string animName)
{
	// 今の動きを止める
	this->stopAllActions();

	// 変更したいアニメーションが現在のアニメーションと同じならば何もしない。
	if (currentAnimation_ == animName)
	{
		return;
	}
	// 現在のアニメーションを変更先のアニメーション名に変更
	currentAnimation_ = animName;
	animationFrame_ = 0.0f;

	// アニメーション終了フラグをfalseに
	isAnimEnd_ = false;
	if (lpAnimMng.GetIsLoop(currentAnimation_))
	{
		auto action = RepeatForever::create(Animate::create(lpAnimMng.GetAnimationCache(currentAnimation_)));
		this->runAction(action);
	}
	else
	{
		this->runAction(Animate::create(lpAnimMng.GetAnimationCache(currentAnimation_)));
	}
}