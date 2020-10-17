#include "GameScene.h"
#include "PL_HPgauge.h"

USING_NS_CC;

cocos2d::Sprite* PL_HPgauge::createPL_HPgauge()
{
	PL_HPgauge* pRet = new(std::nothrow)PL_HPgauge();
	if (pRet && pRet->Init())
	{
		// •K‚¸InitŒã‚É‰æ‘œ“o˜^
		// “o˜^
		SpriteFrameCache::getInstance()->addSpriteFrame(Sprite::create("image/HPbar.png")->getSpriteFrame(), "HPgauge");
		// ‰æ‘œ•`‰æ
		pRet->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("HPgauge"));
		// ŒÄ‚Ño‚µ
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
	// getname‚ªgamescene‚Å‚È‚¢ê‡A‰½‚à‚µ‚È‚¢returnˆ—
	if (Director::getInstance()->getRunningScene()->getName() != "GameScene")
	{
		return;
	}
	float newScale;

	if (nowLife_ <= 0.0f)
	{
		// 0‚ð‰º‰ñ‚ç‚È‚¢‚æ‚¤‚É
		newScale = 0;
	}
	else
	{
		// nowLife_‚Ì’l‚Ì‘å‚«‚³‚ð‚Ý‚ÄAk¬‚·‚é
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
