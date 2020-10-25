#pragma once

#include "cocos2d.h"
#include <map>
#include <array>
#include <memory>
#include "obj/Actor.h"

#define lpAnimMng AnimMng::GetInstance()

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
	void addAnimationCache(std::string actorName, std::string animName, int frame, float duration, ActorType type);
	// �A�j���[�V�����̏������Ǝ��s
	void InitAnimation(cocos2d::Sprite& sprite, ActorType type);
	// �A�j���[�V�����̎��s
	//void anim_action(cocos2d::Sprite* delta);
	// �A�j���[�V�����̕ύX
	void ChangeAnimation(cocos2d::Sprite& sprite, std::string name, bool loop, ActorType type);
	// �A�j���[�V�����̏I���`�F�b�N�֐�
	// �A�j���[�V�������I�����Ă�����true��Ԃ�
	// param@:sprite Actor�̃X�v���C�g
	// param@:animTag �`�F�b�N�������A�j���[�V�����̃^�O
	// tag�ŃA�j���[�V������T���ă`�F�b�N����A�j���[�V���������߂�
	bool IsAnimEnd(const float& delta, ActorType type, std::string animName);
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
private:
	AnimMng();

	void operator=(AnimMng&) = delete;

	// �L�����ʂ̃A�j���[�V�����L���b�V���̓o�^
	void CacheRegistration(cocos2d::AnimationCache* animCache, const ActorType& type, std::string animName);

	// �L�����N�^�[�̃^�C�v�ʂ̃A�j���[�V�����L���b�V��
	std::array<std::map<std::string, cocos2d::Animation*>, static_cast<int>(ActorType::Max)> caches_;
	// 1�L������1�A�j���[�V�����ɂ����鎞��
	std::array<std::map<std::string, float>, static_cast<int>(ActorType::Max)> animMaxFrame_;

	// ��ׂ̱�Ұ��ݏW
	std::array<std::vector<std::string>, static_cast<int>(ActorType::Max)> animations_;
	// ��ׂ�Actor���̖��O
	std::array<std::string, static_cast<int>(ActorType::Max)> actorNames_;
	// �A�j���[�V�����t���[����
	std::array<std::map<std::string, int>, static_cast<int>(ActorType::Max)> frameNum_;

	static std::unique_ptr<AnimMng> s_instance;
};

