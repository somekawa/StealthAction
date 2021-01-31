#include "HPGauge.h"
#include "obj/Actor.h"

USING_NS_CC;

HPGauge::HPGauge(Actor& target,char moveTag):targetActor_(target)
{
	moveFlag_ = moveTag;
	initLife_ = targetActor_.GetHp();
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

cocos2d::Sprite* HPGauge::createHPGauge(Actor& target, char moveTag)
{
	HPGauge* pRet = new(std::nothrow)HPGauge(target,moveTag);
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

bool HPGauge::Init(void)
{

	return true;
}

void HPGauge::update(float delta)
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
	if (moveFlag_ == 1)
	{
		setPosition(Vec2(targetActor_.getPosition().x - 30.0f, targetActor_.getPosition().y + 100.0f));
	}

	if (getTag() == 0 || !targetActor_.AliveCheck())
	{
		removeFromParentAndCleanup(true);
	}
}

void HPGauge::SetHP(float hp)
{
	nowLife_ = hp;
}

float HPGauge::GetHP(void)
{
	return nowLife_;
}

HPGauge::~HPGauge()
{

}
