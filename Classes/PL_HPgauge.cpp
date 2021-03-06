#include "Scene/GameScene.h"
#include "PL_HPgauge.h"

USING_NS_CC;

cocos2d::Sprite* PL_HPgauge::createPL_HPgauge()
{
	PL_HPgauge* pRet = new(std::nothrow)PL_HPgauge();
	if (pRet && pRet->Init())
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

bool PL_HPgauge::Init(void)
{
	nowLife_ = 100;
	initLife_ = nowLife_;
	setColor(Color3B(0,255,0));
	return true;
}

void PL_HPgauge::update(float delta)
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

	this->setScale(newScale, 1.0);
}

void PL_HPgauge::SetHP(float hp)
{
	nowLife_ = hp;
}

float PL_HPgauge::GetHP(void)
{
	return nowLife_;
}

PL_HPgauge::~PL_HPgauge()
{
}
