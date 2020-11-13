#include "Actor.h"
#include "Scene/GameScene.h"

USING_NS_CC;

//Actor::Actor()
//{
//}

Actor::Actor()
{
	deleteFlag_ = false;
	isAnimEnd_ = false;
	animationFrame_ = 0.0f;
}

Actor::~Actor()
{
}

void Actor::Update(void)
{
	Action();
}

// アニメーションの更新関数(もうちょっと改良する)
void Actor::UpdateAnimation(float delta)
{
	// アニメーションカウントを毎フレームdelta値を加算
	animationFrame_ += delta;
	// あるアニメーションの終了までの継続時間の格納
	auto duration = lpAnimMng.GetAnimationCache(type_, currentAnimation_)->getDuration();
	// アニメーションカウントが継続時間を超えていれば
	if (animationFrame_ >= duration)
	{
		// ループフラグがtrueの場合はループ再生
		if (lpAnimMng.GetIsLoop(type_, currentAnimation_))
		{
			auto action = RepeatForever::create(Animate::create(lpAnimMng.GetAnimationCache(type_, currentAnimation_)));
			this->runAction(action);
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

