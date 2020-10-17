#include "GameScene.h"
#include "PL_HPgauge.h"

USING_NS_CC;

cocos2d::Sprite* PL_HPgauge::createPL_HPgauge()
{
	PL_HPgauge* pRet = new(std::nothrow)PL_HPgauge();
	if (pRet && pRet->Init())
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

bool PL_HPgauge::Init(void)
{
	nowLife_ = 100;
	initLife_ = nowLife_;
	setColor(Color3B(0,255,0));
	return true;
}

void PL_HPgauge::update(float delta)
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
