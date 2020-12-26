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
	auto targetPos = target_.getPosition();
	// ﾀｰｹﾞｯﾄのｻｲｽﾞ
	auto targetSize = target_.getContentSize();
	// ﾀｰｹﾞｯﾄと自身(ｱﾀｯｸｵﾌﾞｼﾞｪｸﾄ)との距離
	auto distance = Vec2{ targetPos.x - getPosition().x,targetPos.y - getPosition().y };
	// ﾀｰｹﾞｯﾄのｻｲｽﾞ/2 + 自身のｻｲｽﾞ/2
	auto diff = Vec2{ (targetSize.width / 2) + (getContentSize().width / 2),(targetSize.height / 2) + (getContentSize().height / 2) };
	// 当たっている
	if (abs(distance.x) <= diff.x && abs(distance.y) <= diff.y)
	{
		// 当たったﾌﾗｸﾞをtrueに
		isHitTarget_ = true;
		return true;
	}
	// 当たっていない
	return false;
}

void Obj::UpdateAnimation(float delta)
{
	/*if (currentAnimation_ != previousAnimation_)
{
	animationFrame_ = lpAnimMng.GetFrameNum(type_,currentAnimation_);
}*/
// アニメーションカウントを毎フレームdelta値を加算
	animationFrame_ += delta;
	//TRACE("%s", currentAnimation_);
	//TRACE("currentAnimation:%s,animFrame:%f\n", currentAnimation_.c_str(), animationFrame_);
	if (currentAnimation_ == "impact")
	{
		int i = 0;
	}
	// あるアニメーションの終了までの継続時間の格納
	auto duration = lpAnimMng.GetAnimationCache(type_, currentAnimation_)->getDuration();
	// アニメーションカウントが継続時間を超えていれば
	if (animationFrame_ >= duration)
	{
		// ループフラグがtrueの場合はループ再生
		if (lpAnimMng.GetIsLoop(type_, currentAnimation_))
		{
			//auto action = RepeatForever::create(Animate::create(lpAnimMng.GetAnimationCache(type_, currentAnimation_)));
			//this->runAction(action);
			animationFrame_ = 0.0f;
			//animationFrame_ = 0.0f;
		}
		else
		{
			// falseの場合は1回の再生
			// 1アニメーションが終了したフラグを立てる
			isAnimEnd_ = true;
			//animationFrame_ = 0.0f;
		}
	}
	TRACE("animEndFlag:%d\n", isAnimEnd_);
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
	// アニメーションのフレーム数を初期値に戻す
	//animationFrame_ = 0.0f;
	if (lpAnimMng.GetIsLoop(type_, currentAnimation_))
	{
		auto action = RepeatForever::create(Animate::create(lpAnimMng.GetAnimationCache(type_, currentAnimation_)));
		this->runAction(action);
	}
	else
	{
		this->runAction(Animate::create(lpAnimMng.GetAnimationCache(type_, currentAnimation_)));
	}
}
