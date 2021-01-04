#include "EffectManager.h"

USING_NS_CC;

EffectManager::EffectManager(Layer& layer)
{
	layer.addChild(this);
}

EffectManager::~EffectManager()
{
}

void EffectManager::update(float delta)
{
	/*if (animate_->isDone())
	{
		isAnimEnd_ = true;
	}*/
}

void EffectManager::Play(const EffectType& eType, cocos2d::Vec2 pos)
{
	pos_ = pos;
	type_ = eType;
	animFrame_ = 0.0f;
	isAnimEnd_ = false;
	animate_ = Animate::create(effectAnimation_[eType]);
	auto action = Repeat::create(Animate::create(effectAnimation_[eType]), 1);
	runAction(action);
}

void EffectManager::Load(EffectType eType,int frame,float duration)
{
	std::string effectPath = "";
	std::string effectName = "";
	switch (eType)
	{
	case EffectType::PlayerSpawn:
		effectPath = "";
		effectName = "playerSpawn";
		break;
	case EffectType::EnemySpawn:
		effectPath = "effect/enemySpawn";

		effectName = "enemySpawn";
		break;
	case EffectType::PlayerAttack1:
		effectPath = "";

		effectName = "playerAttack1";
		break;
	case EffectType::PlayerAttack2:
		effectPath = "";

		effectName = "playerAttack2";
		break;
	case EffectType::Max:
		break;
	default:
		break;
	}
	// �A�j���[�V�����L���b�V���̓V���O���g��
	AnimationCache* animationCache = AnimationCache::getInstance();

	//�X�v���C�g�V�[�g�̏���
	auto cache = SpriteFrameCache::getInstance();

	// �p�X�w��
	cache->addSpriteFramesWithFile(effectPath + ".plist");

	// �A�j���[�V�����摜�ǉ�
	Animation* animation = Animation::create();

	for (int i = 0; i < frame; i++)
	{
		auto string = effectName + "%d.png";		// plist�̒�������p�X����Ȃ�
		auto str = StringUtils::format(string.c_str(), i);
		SpriteFrame* sprite = cache->getSpriteFrameByName(str);

		animation->addSpriteFrame(sprite);
	}

	// �A�j���[�V�����̊Ԋu
	animation->setDelayPerUnit(duration);

	// �A�j���[�V�����I����ɍŏ��ɖ߂����ǂ���
	animation->setRestoreOriginalFrame(true);

	// �o�����A�j���[�V�������L���b�V���ɓo�^
	animationCache->addAnimation(animation,effectName);
	//animationCache_->addAnimation(animation, actorName + "_" + animName);
	// effectAnimation_�ɓo�^
	effectAnimation_.try_emplace(eType, animation);
}
