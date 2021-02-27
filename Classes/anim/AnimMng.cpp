#include "AnimMng.h"

USING_NS_CC;

std::unique_ptr<AnimMng>AnimMng::s_instance(new AnimMng);

AnimMng::AnimMng()
{
}

AnimMng::~AnimMng()
{
}

void AnimMng::addAnimationCache(std::string actorName, std::string animName, int frame, float duration, ActorType type,bool isLoop)
{
	actorNames_[static_cast<int>(type)] = actorName;
	std::string name = "";
	switch (type)
	{
	case ActorType::Player:
		// light��dark�Ńt�@�C���𕪂���
		if (actorName == "image/PlayerAnimationAsset/player/Light/player_Light")
		{
			//name = "player";
			name = "player_Light";
		}
		else
		{
			name = "player_Dark";
		}
		break;
	case ActorType::Imp:
		name = "imp";

		break;
	case ActorType::Assassin:
		name = "assassin";

		break;
	case ActorType::TwistedCultist:
		name = "twistedCultist";

		break;
	case ActorType::Cultist:
		name = "cultist";

		break;
	case ActorType::BigCultist:
		name = "bigCultist";
		break;
	case ActorType::Max:
		break;
	default:
		break;
	}

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
		SpriteFrame* sprite = cache->getSpriteFrameByName(name + "_" + str);

		animation->addSpriteFrame(sprite);
	}

	// �A�j���[�V�����̊Ԋu
	animation->setDelayPerUnit(duration);

	// �A�j���[�V�����I����ɍŏ��ɖ߂����ǂ���
	animation->setRestoreOriginalFrame(true);

	// �o�����A�j���[�V�������L���b�V���ɓo�^
	animationCache->addAnimation(animation, name + "_" + animName);
	//animationCache_->addAnimation(animation, actorName + "_" + animName);

	// 1�A�j���[�V�����̃L���b�V���f�[�^���i�[���鏈��
	CacheRegistration(animationCache, type,name + "_" + animName,isLoop);

	if (frameNum_[static_cast<int>(type)].find(animName) == frameNum_[static_cast<int>(type)].end())
	{
		frameNum_[static_cast<int>(type)].emplace(animName, frame);
	}
}

void AnimMng::addAnimationCache(std::string name, std::string animName, int frame, float duration, bool isLoop)
{
	std::string fileName = "image/EnemyAnimationAsset/" + name + "/" + animName;
	// �A�j���[�V�����L���b�V���̓V���O���g��
	AnimationCache* animationCache = AnimationCache::getInstance();

	//�X�v���C�g�V�[�g�̏���
	auto cache = SpriteFrameCache::getInstance();

	cache->addSpriteFramesWithFile(fileName + ".plist");

	// �A�j���[�V�����摜�ǉ�
	Animation* animation = Animation::create();

	for (int i = 0; i < frame; i++)
	{
		std::string string = "%d.png";		// plist�̒�������p�X����Ȃ�
		auto str = StringUtils::format(string.c_str(), i);
		auto s = animName + "_" + str;
		SpriteFrame* sprite = cache->getSpriteFrameByName(animName + "_" + str);

		animation->addSpriteFrame(sprite);
	}

	// �A�j���[�V�����̊Ԋu
	animation->setDelayPerUnit(duration);

	// �A�j���[�V�����I����ɍŏ��ɖ߂����ǂ���
	animation->setRestoreOriginalFrame(true);

	// �o�����A�j���[�V�������L���b�V���ɓo�^
	animationCache->addAnimation(animation,animName);

	// 1�A�j���[�V�����̃L���b�V���f�[�^���i�[���鏈��
	CacheRegistration(animationCache,animName, isLoop);
}

void AnimMng::InitAnimation(cocos2d::Sprite& sprite, ActorType type, std::string animName)
{
	Animation* animation = caches_[static_cast<int>(type)][animName];

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

void AnimMng::CacheRegistration(cocos2d::AnimationCache* animCache, const ActorType& type, std::string animName,bool isLoop)
{
	if (caches_[static_cast<int>(type)].count(animName) <= 0)
	{
		// �L�����̃^�C�v�ʂ̃A�j���[�V�����L���b�V����1�A�j���[�V�����f�[�^���i�[
		caches_[static_cast<int>(type)].emplace(animName, animCache->getAnimation(animName));
		// ��Ұ��݂̕�����i�[
		animations_[static_cast<int>(type)].emplace_back(animName);
		// 1�A�j���[�V�����ɂ����鎞�Ԃ̊i�[
		animMaxFrame_[static_cast<int>(type)].emplace(animName, caches_[static_cast<int>(type)][animName]->getDelayPerUnit() *
			caches_[static_cast<int>(type)][animName]->getFrames().size());

		// ���[�v�t���O��ݒ�
		isLoop_[static_cast<int>(type)][animName] = isLoop;
	}
}

void AnimMng::CacheRegistration(cocos2d::AnimationCache* animCache, std::string animName, bool isLoop)
{
	if (cachesExceptCharacter_.count(animName) <= 0)
	{
		// �L�����̃^�C�v�ʂ̃A�j���[�V�����L���b�V����1�A�j���[�V�����f�[�^���i�[
		cachesExceptCharacter_.emplace(animName, animCache->getAnimation(animName));
		// ���[�v�t���O��ݒ�
		isLoopExceptCharacter_.emplace(animName, isLoop);
	}
}

bool AnimMng::IsAnimEnd(const float& delta, ActorType type, std::string animName)
{
	if (delta >= animMaxFrame_[static_cast<int>(type)][animName] - 0.2f)
	{
		return true;
	}
	return false;
}

void AnimMng::AnimDataClear(void)
{
	for (auto& data : caches_)
	{
		data.clear();
	}
}

void AnimMng::FireBallClear(void)
{
	cachesExceptCharacter_.clear();
}
