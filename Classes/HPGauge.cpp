// 担当場所
#include "HPGauge.h"
#include "obj/Actor.h"

USING_NS_CC;

HPGauge::HPGauge()
{
}

HPGauge::~HPGauge()
{
}

HPGauge::HPGauge(Actor& target, char moveTag) :targetActor_(&target)
{
	moveFlag_ = moveTag;
	initLife_ = static_cast<float>(targetActor_->GetHp());
	nowLife_ = initLife_;
	if (target.getName() == "player1")
	{
		setColor(Color3B::GREEN);
	}
	else
	{
		setColor(Color3B::RED);
	}
}

HPGauge* HPGauge::createHPGauge(Actor& target, char moveTag)
{
	HPGauge* pRet = new(std::nothrow)HPGauge(target,moveTag);
	if (pRet && pRet->init())
	{
		// 必ずInit後に画像登録
		// 登録
		SpriteFrameCache::getInstance()->addSpriteFrame(Sprite::create("image/HPbar.png")->getSpriteFrame(), "HPgauge");
		// 画像描画
		pRet->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("HPgauge"));
		// 呼び出し
		pRet->autorelease();
	}
	else
	{
		delete pRet;
		pRet = nullptr;
	}
	return pRet;
}

void HPGauge::update(float delta)
{
	// getnameがgamesceneでない場合、何もしないreturn処理
	if (Director::getInstance()->getRunningScene()->getName() != "GameScene")
	{
		return;
	}

	float newScale;

	if (nowLife_ <= 0.0f)
	{
		// 0を下回らないように
		newScale = 0;
	}
	else
	{
		// nowLife_の値の大きさをみて、縮小する
		newScale = nowLife_ / initLife_;
	}

	if (targetActor_->getName() == "player1")
	{
		this->setScale(newScale, 1.0);
		// プレイヤーのHPは右から左に削れたほうがいいため、アンカーポイントを変更している
		if (this->getAnchorPoint() != Vec2(0.0f, 0.5f))
		{
			this->setAnchorPoint(Vec2(0.0f, 0.5f));
		}
	}
	else
	{
		// 敵のゲージはプレイヤーと同じサイズだと見づらくなるため、縮小している
		this->setScale(newScale / 2, 1.0 / 2);
	}

	// 敵のゲージの座標移動処理
	if (moveFlag_ == 1)
	{
		setPosition(Vec2(targetActor_->getPosition().x - 30.0f, targetActor_->getPosition().y + 100.0f));
	}
	// 敵死亡時に敵のゲージを削除する
	if (getTag() == 0 || !targetActor_->AliveCheck())
	{
		removeFromParentAndCleanup(true);
	}
}

void HPGauge::SetHP(float hp)
{
	nowLife_ = hp;
	// 上限を超えないように
	if (nowLife_ > 100.0f)
	{
		nowLife_ = 100.0f;
	}
}

float HPGauge::GetHP(void)
{
	return nowLife_;
}