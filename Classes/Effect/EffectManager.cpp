#include "EffectManager.h"

USING_NS_CC;

EffectManager::~EffectManager()
{
}

// effectManager�̓V���O���g����
void EffectManager::Update(const Layer& layer)
{

}

void EffectManager::AddEffect(std::string effectName, int frame, float duration, Vec2 offset, Layer& layer, bool isMove)
{
	// �o�^����Ă��Ȃ���΁A��Ұ��݂̓o�^������
	if(effectAnimation_.find(effectName) == effectAnimation_.end())
	{
		// �̪�Ẳ摜�߽
		std::string effectPath = "effect/" + effectName;

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
		animationCache->addAnimation(animation, effectName);

		// �̪�Ė��̱�Ұ����ް��̕ۑ�
		effectAnimation_.emplace(effectName, animationCache->getAnimation(effectName));
		// �̪�Ė��̵̾�Ēl�̕ۑ�
		offset_.emplace(effectName, offset);
	}
	// ڲ԰�ɴ̪�Ă̽��ײĂ�ǉ�
	layer.addChild(effectSprites_[effectName].create());
	// �����邩�ǂ�������ސݒ�
	effectSprites_[effectName].setTag(isMove);
	// �̪�Ă��߼޼�ݾ��
	effectSprites_[effectName].setPosition({ 100.0f,200.0f });
	// ýčĐ�
	effectSprites_[effectName].runAction(Animate::create(effectAnimation_[effectName]));

	// �Đ�����ؽĂɊi�[���Ă���
	//playList_.emplace_back(( effectAnimation_[effectName],isMove ));
}

// �̪�Ă��Đ�����offset�������Ŏ��R�Ɍ��߂鎖���\�Ȃ悤��
//void EffectManager::Play(const EffectType& eType, cocos2d::Vec2 pos)
//{
//	// �߼޼�ݾ��
//	// autoRelease()�������\������
//	pos_ = pos;
//	setPosition(pos);
//	// ���߾��
//	type_ = eType;
//	// ��Ұ��ݺϐ����ĕϐ�������
//	animFrame_ = 0.0f;
//	// ��Ұ��݂��I�����������׸ޏ�����
//	isAnimEnd_ = false;
//	// ���̴݂̪�ı�Ұ����ް��̊i�[
//	animate_ = Animate::create(effectAnimation_[eType]);
//	// ���݂̱�Ұ����ް���runAction������
//	auto action = Repeat::create(Animate::create(effectAnimation_[eType]), 1);
//	runAction(action);
//}

//void EffectManager::Load(EffectType eType,int frame,float duration)
//{
//	std::string effectPath = "";
//	std::string effectName = "";
//	switch (eType)
//	{
//	case EffectType::PlayerSpawn:
//		// �̪�Ă�����path��
//		effectPath = "";
//		// �̪�ČŗL�̖��O
//		effectName = "playerSpawn";
//		break;
//	case EffectType::EnemySpawn:
//		// �̪�Ă�����path��
//		effectPath = "effect/enemySpawn";
//		// �̪�ČŗL�̖��O
//		effectName = "enemySpawn";
//		break;
//	case EffectType::PlayerAttack1:
//		effectPath = "";
//
//		effectName = "playerAttack1";
//		break;
//	case EffectType::PlayerAttack2:
//		effectPath = "";
//
//		effectName = "playerAttack2";
//		break;
//	case EffectType::Max:
//		break;
//	default:
//		break;
//	}
//	// �A�j���[�V�����L���b�V���̓V���O���g��
//	AnimationCache* animationCache = AnimationCache::getInstance();
//
//	//�X�v���C�g�V�[�g�̏���
//	auto cache = SpriteFrameCache::getInstance();
//
//	// �p�X�w��
//	cache->addSpriteFramesWithFile(effectPath + ".plist");
//
//	// �A�j���[�V�����摜�ǉ�
//	Animation* animation = Animation::create();
//
//	for (int i = 0; i < frame; i++)
//	{
//		auto string = effectName + "%d.png";		// plist�̒�������p�X����Ȃ�
//		auto str = StringUtils::format(string.c_str(), i);
//		SpriteFrame* sprite = cache->getSpriteFrameByName(str);
//
//		animation->addSpriteFrame(sprite);
//	}
//
//	// �A�j���[�V�����̊Ԋu
//	animation->setDelayPerUnit(duration);
//
//	// �A�j���[�V�����I����ɍŏ��ɖ߂����ǂ���
//	animation->setRestoreOriginalFrame(true);
//
//	// �o�����A�j���[�V�������L���b�V���ɓo�^
//	animationCache->addAnimation(animation,effectName);
//	// effectAnimation_�ɓo�^
//	effectAnimation_.try_emplace(eType, animation);
//}
