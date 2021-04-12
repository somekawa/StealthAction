// �S���ꏊ
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

	if (targetActor_->getName() == "player1")
	{
		this->setScale(newScale, 1.0);
		// �v���C���[��HP�͉E���獶�ɍ�ꂽ�ق����������߁A�A���J�[�|�C���g��ύX���Ă���
		if (this->getAnchorPoint() != Vec2(0.0f, 0.5f))
		{
			this->setAnchorPoint(Vec2(0.0f, 0.5f));
		}
	}
	else
	{
		// �G�̃Q�[�W�̓v���C���[�Ɠ����T�C�Y���ƌ��Â炭�Ȃ邽�߁A�k�����Ă���
		this->setScale(newScale / 2, 1.0 / 2);
	}

	// �G�̃Q�[�W�̍��W�ړ�����
	if (moveFlag_ == 1)
	{
		setPosition(Vec2(targetActor_->getPosition().x - 30.0f, targetActor_->getPosition().y + 100.0f));
	}
	// �G���S���ɓG�̃Q�[�W���폜����
	if (getTag() == 0 || !targetActor_->AliveCheck())
	{
		removeFromParentAndCleanup(true);
	}
}

void HPGauge::SetHP(float hp)
{
	nowLife_ = hp;
	// ����𒴂��Ȃ��悤��
	if (nowLife_ > 100.0f)
	{
		nowLife_ = 100.0f;
	}
}

float HPGauge::GetHP(void)
{
	return nowLife_;
}