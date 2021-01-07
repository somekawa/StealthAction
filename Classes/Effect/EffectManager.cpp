#include "EffectManager.h"

USING_NS_CC;

EffectManager::EffectManager(Layer& layer)
{
	// GameScene��EffectLayer�ɂԂ牺����
	layer.addChild(this);
}

EffectManager::~EffectManager()
{
}

void EffectManager::update(float delta)
{
	//if (animate_->isDone())
	//{
	//	isAnimEnd_ = true;
	//}
}

void EffectManager::Play(const EffectType& eType, cocos2d::Vec2 pos)
{
	// �߼޼�ݾ��
	pos_ = pos;
	setPosition(pos);
	// ���߾��
	type_ = eType;
	// ��Ұ��ݺϐ����ĕϐ�������
	animFrame_ = 0.0f;
	// ��Ұ��݂��I�����������׸ޏ�����
	isAnimEnd_ = false;
	// ���̴݂̪�ı�Ұ����ް��̊i�[
	animate_ = Animate::create(effectAnimation_[eType]);
	// ���݂̱�Ұ����ް���runAction������
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
		// �̪�Ă�����path��
		effectPath = "";
		// �̪�ČŗL�̖��O
		effectName = "playerSpawn";
		break;
	case EffectType::EnemySpawn:
		// �̪�Ă�����path��
		effectPath = "effect/enemySpawn";
		// �̪�ČŗL�̖��O
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
	// effectAnimation_�ɓo�^
	effectAnimation_.try_emplace(eType, animation);
}
