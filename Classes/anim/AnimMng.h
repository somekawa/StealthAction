#pragma once
#include <cocos2d.h>
#include <map>
#include <array>
#include <vector>
#include <memory>
#include "obj/Actor.h"
#include "obj/Player.h"

#define lpAnimMng AnimMng::GetInstance()

// �A�j���[�V�����ݒ�ɕK�v�ȏ��
struct AnimSt
{
	std::string animName;
	int frame;
	float duration;
	bool loop;
};

class AnimMng
{
public:
	static AnimMng& GetInstance()
	{
		return *s_instance;
	}
	~AnimMng();

	// �A�j���[�V�����̓o�^
	// param@: actorName:: �L�����N�^�[�̖��O
	// param@: animName :: �A�j���[�V�����̖��O
	// param@: cacheName:: �o�^����A�j���[�V������
	// param@: frame    :: �A�j���[�V�����̑��t���[��
	// param@: duration :: �A�j���[�V�������Đ�����Ԋu
	void addAnimationCache(std::string actorName, std::string animName, int frame, float duration, ActorType type,bool isLoop);
	void addAnimationCache(std::string name, std::string animName, int frame, float duration, bool isLoop);

	// �A�j���[�V�����̏������Ǝ��s
	void InitAnimation(cocos2d::Sprite& sprite, ActorType type, std::string animName);
	// �A�j���[�V�����̕ύX
	void ChangeAnimation(cocos2d::Sprite& sprite, std::string name, bool loop, ActorType type);
	// �A�j���[�V�����̏I���`�F�b�N�֐�
	// �A�j���[�V�������I�����Ă�����true��Ԃ�
	// param@:sprite Actor�̃X�v���C�g
	// param@:animTag �`�F�b�N�������A�j���[�V�����̃^�O
	// tag�ŃA�j���[�V������T���ă`�F�b�N����A�j���[�V���������߂�
	bool IsAnimEnd(const float& delta, ActorType type, std::string animName);

	void AnimDataClear(void);
	void FireBallClear(void);

	const float& GetAnimationMaxFrame(const ActorType& type, std::string animName)
	{
		return animMaxFrame_[static_cast<int>(type)][animName];
	}
	// �A�j���[�V�����W�̎擾
	const std::vector<std::string>& GetAnimations(const ActorType& type) const
	{
		return animations_[static_cast<int>(type)];
	}
	// �A�j���[�V�����t���[�����̎擾
	const int& GetFrameNum(const ActorType& type, const std::string& animName)
	{
		return frameNum_[static_cast<int>(type)][animName];
	}
	// �A�j���[�V�����L���b�V���̎擾
	cocos2d::Animation* GetAnimationCache(ActorType type, std::string animationName)
	{
		return caches_[static_cast<int>(type)][animationName];
	}
	// �A�j���[�V�����L���b�V��(��׸���ȊO)�̎擾
	cocos2d::Animation* GetAnimationCache(std::string animationName)
	{
		return cachesExceptCharacter_[animationName];
	}
	// �A�j���[�V�������[�v�t���O�̎擾
	const bool& GetIsLoop(ActorType type, std::string animationName)
	{
		return isLoop_[static_cast<int>(type)][animationName];
	}

	const bool& GetIsLoop(std::string animationName)
	{
		return isLoopExceptCharacter_[animationName];
	}

	// �A�j���[�V�����̍ő�R�}��
	int GetAnimationMaxNum(ActorType type, std::string animationName);

	// �A�j���[�V�����f�[�^�̓ǂݍ���
	void XmlAnimDataRead(std::string string, ActorType type);
	// �v���C���[�̃p�����[�^�[�ǂݍ���
	std::vector<PlParam> XmlPlParamRead(void);
private:
	AnimMng();

	void operator=(AnimMng&) = delete;

	// �L�����ʂ̃A�j���[�V�����L���b�V���̓o�^
	void CacheRegistration(cocos2d::AnimationCache* animCache, const ActorType& type, std::string animName,bool isLoop);
	void CacheRegistration(cocos2d::AnimationCache* animCache,std::string animName, bool isLoop);
	// �L�����N�^�[�̃^�C�v�ʂ̃A�j���[�V�����L���b�V��
	std::array<std::map<std::string, cocos2d::Animation*>, static_cast<int>(ActorType::Max)> caches_;
	// �L�����N�^�[�ȊO�̃A�j���[�V�����L���b�V��
	std::map<std::string, cocos2d::Animation*> cachesExceptCharacter_;
	// 1�L������1�A�j���[�V�����ɂ����鎞��
	std::array<std::map<std::string, float>, static_cast<int>(ActorType::Max)> animMaxFrame_;

	// �L�����N�^�[�̃A�j���[�V�����W
	std::array<std::vector<std::string>, static_cast<int>(ActorType::Max)> animations_;
	// �L�����N�^�[��Actor���̖��O
	std::array<std::string, static_cast<int>(ActorType::Max)> actorNames_;
	// �A�j���[�V�����t���[����
	std::array<std::map<std::string, int>, static_cast<int>(ActorType::Max)> frameNum_;

	// �A�j���[�V�����̃��[�v�t���O
	std::array<std::map< std::string,bool>, static_cast<int>(ActorType::Max)> isLoop_;
	// �L�����N�^�[�ȊO�̃��[�v�t���O
	std::map<std::string, bool> isLoopExceptCharacter_;

	static std::unique_ptr<AnimMng> s_instance;
};