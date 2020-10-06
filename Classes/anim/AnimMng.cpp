#include "AnimMng.h"

USING_NS_CC;

cocos2d::Sprite * AnimMng::createAnim()
{
	return AnimMng::create();
}

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
}

void AnimMng::anim_action(Sprite * delta)
{
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
	AnimationCache *animationCache = AnimationCache::getInstance();
	Animation *animation = animationCache->getAnimation(name);

	// �t���[���A�j���[�V�����͌J��Ԃ�
	RepeatForever *action = RepeatForever::create(Animate::create(animation));

	//�A�j���[�V���������s
	delta->runAction(action);
}
