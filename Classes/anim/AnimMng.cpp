#include "AnimMng.h"

USING_NS_CC;

std::unique_ptr<AnimMng>AnimMng::s_instance(new AnimMng);

AnimMng::AnimMng()
{
}

AnimMng::~AnimMng()
{
}

void AnimMng::addAnimationCache( std::string plist, const char* plist_in_png, std::string cacheName, int startNum, int endNum, bool isReverse, float duration)
{
	// �A�j���[�V�����L���b�V���̓V���O���g��
	AnimationCache *animationCache = AnimationCache::getInstance();

	//�X�v���C�g�V�[�g�̏���
	auto cache = SpriteFrameCache::getInstance();

	// �p�X�w��
	cache->addSpriteFramesWithFile(plist);

	// �A�j���[�V�����摜�ǉ�
	Animation* animation = Animation::create();

	for (int i = startNum; i <= endNum; i++)
	{
		auto string = StringUtils::format(plist_in_png, i);	// plist�̒�������p�X����Ȃ�
		auto idle = cache->getSpriteFrameByName(string);
		animation->addSpriteFrame(idle);
	}

	// �A�j���[�V�����̊Ԋu
	animation->setDelayPerUnit(duration);

	// �A�j���[�V�����I����ɍŏ��ɖ߂����ǂ���
	animation->setRestoreOriginalFrame(true);

	// �o�����A�j���[�V�������L���b�V���ɓo�^
	animationCache->addAnimation(animation, cacheName);
	
	CacheRegistration(animationCache, cacheName, 0);
	//CacheRegistration(, std::string animName,ActorType type);

	// CacheRegistration
	{
		
	}
}

void AnimMng::anim_action(Sprite * delta)
{
	// �L������p�̂�������オ�ق���
	// �A�j���[�V�������擾
	AnimationCache *animationCache = AnimationCache::getInstance();
	Animation *animation = animationCache->getAnimation("idle");

	// �t���[���A�j���[�V�������J��Ԃ�
	RepeatForever *action = RepeatForever::create(Animate::create(animation));

	// �A�j���[�V���������s
	delta->runAction(action);
}

// �A�j���[�V�����؂�ւ�
void AnimMng::ChangeAnim(cocos2d::Sprite * delta, const char * name)
{
	// ���̓������~�߂�
	delta->stopAllActions();

	// �L���b�V������ "ng"�Ƃ������O�œo�^����Ă���A�j���[�V�����L���b�V�������擾
	//AnimationCache *animationCache = AnimationCache::getInstance();
	//Animation *animation = animationCache->getAnimation(name);


	// �t���[���A�j���[�V�����͌J��Ԃ�
	RepeatForever *action = RepeatForever::create(Animate::create(cache_[0][name]));

	//�A�j���[�V���������s
	delta->runAction(action);
}

void AnimMng::CacheRegistration(cocos2d::AnimationCache* cache, std::string animName,int type)
{
	//cache_[type].push_back(cache->getAnimation(animName));
	cache_[type].emplace(animName, cache->getAnimation(animName));
}
