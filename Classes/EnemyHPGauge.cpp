#include "EnemyHPGauge.h"
#include "obj/Enemy/Enemy.h"

USING_NS_CC;

EnemyHPGauge::EnemyHPGauge(ActorType type,Enemy& target):
	target_(target)
{
	retain();
	enemyType_ = type;
	switch (type)
	{
	case ActorType::Player:
		break;
	case ActorType::Imp:
		break;
	case ActorType::Assassin:
		enemyName_ = "assassin";
		break;
	case ActorType::TwistedCultist:
		enemyName_ = "twistedCultist";
		break;
	case ActorType::Cultist:
		enemyName_ = "cultist";
		break;
	case ActorType::Fireball:
		break;
	case ActorType::Max:
		break;
	default:
		break;
	}
	setPosition(target_.getPosition());
	setTag(101);
	setName(target_.GetMyName() + "_" + std::to_string(target_.GetID()) + "_HP");
	// ‰æ‘œ•`‰æ
	setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("EnemyHPGauge"));

}

EnemyHPGauge::~EnemyHPGauge()
{
}

void EnemyHPGauge::update(float delta)
{
	if (!target_.AliveCheck() || getTag() == 0)
	{
		removeFromParentAndCleanup(true);
		return;
	}
	// getname‚ªgamescene‚Å‚È‚¢ê‡A‰½‚à‚µ‚È‚¢returnˆ—
	if (Director::getInstance()->getRunningScene()->getName() != "GameScene")
	{
		return;
	}

	setPosition({target_.getPosition().x - 100.0f,target_.getPosition().y + 100.0f});
	if (currentHP_ <= 0.0f)
	{
		// 0‚ð‰º‰ñ‚ç‚È‚¢‚æ‚¤‚É
		curScale_ = 0;
	}
	else
	{
		// nowHP_‚Ì’l‚Ì‘å‚«‚³‚ð‚Ý‚ÄAk¬‚·‚é
		curScale_ = currentHP_ / initHP_;
	}

	this->setScale(curScale_, 1.0);
}

bool EnemyHPGauge::LoadSprite(void)
{
	// “o˜^
	SpriteFrameCache::getInstance()->addSpriteFrame(Sprite::create("image/EnemyHPbar.png")->getSpriteFrame(), "EnemyHPGauge");
	return true;
}

cocos2d::Sprite* EnemyHPGauge::CreateEnemyHPGauge(ActorType type,Enemy& target)
{
	EnemyHPGauge* pRet = new(std::nothrow)EnemyHPGauge(type,target);
	if (pRet && pRet->Init())
	{
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

bool EnemyHPGauge::Init(void)
{
	switch (enemyType_)
	{
	case ActorType::Player:
		break;
	case ActorType::Imp:
		break;
	case ActorType::Assassin:
		currentHP_ = 50.0f;
		setColor(Color3B(255, 0, 0));
		break;
	case ActorType::TwistedCultist:
		currentHP_ = 50.0f;
		setColor(Color3B(0, 255, 0));

		break;
	case ActorType::Cultist:
		currentHP_ = 30.0f;
		setColor(Color3B(0, 0, 255));

		break;
	case ActorType::Fireball:
		break;
	case ActorType::Max:
		break;
	default:
		break;
	}
	initHP_ = currentHP_;
	return true;
}

void EnemyHPGauge::SetHP(float hp)
{
	currentHP_ = hp;
}
