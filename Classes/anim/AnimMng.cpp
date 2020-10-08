#include "AnimMng.h"

USING_NS_CC;

std::unique_ptr<AnimMng>AnimMng::s_instance(new AnimMng);

AnimMng::AnimMng()
{
}

AnimMng::~AnimMng()
{
}

void AnimMng::addAnimationCache(std::string actorName, std::string animName, int frame, float duration, AnimationType animTag, ActorType type)
{
	// �A�j���[�V�����L���b�V���̓V���O���g��
	AnimationCache *animationCache = AnimationCache::getInstance();

	//�X�v���C�g�V�[�g�̏���
	auto cache = SpriteFrameCache::getInstance();

	// �p�X�w��
	auto pListStr = actorName + "_" + animName;
	cache->addSpriteFramesWithFile(pListStr + ".plist");

	// �A�j���[�V�����摜�ǉ�
	Animation* animation = Animation::create();

	for (int i = 0; i < frame; i++)
	{
		auto string = animName + "%d.png";		// plist�̒�������p�X����Ȃ�
		auto str = StringUtils::format(string.c_str(), i);
		SpriteFrame* sprite = cache->getSpriteFrameByName(str);
		animation->addSpriteFrame(sprite);
	}

	// �A�j���[�V�����̊Ԋu
	animation->setDelayPerUnit(duration);

	// �A�j���[�V�����I����ɍŏ��ɖ߂����ǂ���
	animation->setRestoreOriginalFrame(true);

	// �o�����A�j���[�V�������L���b�V���ɓo�^
	animationCache->addAnimation(animation, animName);

	// 1�A�j���[�V�����̃L���b�V���f�[�^���i�[���鏈��
	CacheRegistration(animationCache, type, animName);
}

void AnimMng::InitAnimation(cocos2d::Sprite& sprite, ActorType type)
{
	Animation* animation = caches_[static_cast<int>(type)]["run"];

	RepeatForever* action = RepeatForever::create(Animate::create(animation));

	sprite.runAction(action);
}

void AnimMng::ChangeAnimation(cocos2d::Sprite& sprite, std::string name, bool loop, ActorType type)
{
	// ���̓������~�߂�
	sprite.stopAllActions();

	// �L�[�ɂ��A�j���[�V�����̎��o��
	Animation* animation = caches_[static_cast<int>(type)][name];

	// �t���[���A�j���[�V�����͌J��Ԃ�
	if (loop)
	{
		RepeatForever* action = RepeatForever::create(Animate::create(animation));

		//�A�j���[�V���������s
		sprite.runAction(action);
	}
	else
	{
		auto action_ = Repeat::create(Animate::create(animation), 1);
		//�A�j���[�V���������s
		sprite.runAction(action_);
	}
}

void AnimMng::CacheRegistration(cocos2d::AnimationCache* animCache, const ActorType& type, std::string animName)
{
	// �L�����̃^�C�v�ʂ̃A�j���[�V�����L���b�V����1�A�j���[�V�����f�[�^���i�[
	caches_[static_cast<int>(type)].emplace(animName, animCache->getAnimation(animName));
	// ��Ұ��݂̕�����i�[
	animations_[static_cast<int>(type)].emplace_back(animName);
	// 1�A�j���[�V�����ɂ����鎞�Ԃ̊i�[
	animMaxFrame_[static_cast<int>(type)].emplace(animName, caches_[static_cast<int>(type)][animName]->getDelayPerUnit()*
		caches_[static_cast<int>(type)][animName]->getFrames().size());
}

bool AnimMng::IsAnimEnd(const float& delta, ActorType type, std::string animName)
{
	if (delta >= animMaxFrame_[static_cast<int>(type)][animName] - 0.2f)
	{
		return true;
	}
	return false;
}