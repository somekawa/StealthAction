#include "HPGauge.h"
#include "obj/Actor.h"

USING_NS_CC;

HPGauge::HPGauge()
{
}

HPGauge::HPGauge(Actor& target,char moveTag):targetActor_(&target)
{
	moveFlag_ = moveTag;
	initLife_ = targetActor_->GetHp();
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

HPGauge::~HPGauge()
{
}

HPGauge* HPGauge::createHPGauge(Actor& target, char moveTag)
{
	HPGauge* pRet = new(std::nothrow)HPGauge(target,moveTag);
	if (pRet && pRet->init())
	{
		// �K��Init��ɉ摜�o�^
		// �o�^
		SpriteFrameCache::getInstance()->addSpriteFrame(Sprite::create("image/HPbar.png")->getSpriteFrame(), "HPgauge");
		// �摜�`��
		pRet->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("HPgauge"));
		// �Ăяo��
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
	// getname��gamescene�łȂ��ꍇ�A�������Ȃ�return����
	if (Director::getInstance()->getRunningScene()->getName() != "GameScene")
	{
		return;
	}

	// �v���C���[��HP�͉E���獶�ɍ�ꂽ�ق��������Ǝv�������߁A�A���J�[�|�C���g��ύX���Ă��܂�
	if (targetActor_->getName() == "player1" && this->getAnchorPoint() != Vec2(0.0f, 0.5f))
	{
		this->setAnchorPoint(Vec2(0.0f, 0.5f));
	}

	float newScale;

	if (nowLife_ <= 0.0f)
	{
		// 0�������Ȃ��悤��
		newScale = 0;
	}
	else
	{
		// nowLife_�̒l�̑傫�����݂āA�k������
		newScale = nowLife_ / initLife_;
	}

	this->setScale(newScale, 1.0);
	if (moveFlag_ == 1)
	{
		setPosition(Vec2(targetActor_->getPosition().x - 30.0f, targetActor_->getPosition().y + 100.0f));
	}

	if (getTag() == 0 || !targetActor_->AliveCheck())
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

