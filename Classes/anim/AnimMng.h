#pragma once

#include "cocos2d.h"

class AnimMng : cocos2d::Sprite
{
public:
	static cocos2d::Sprite* createAnim();

	AnimMng();
	~AnimMng();

	// �A�j���[�V�����̓o�^
	static void addAnimationCache(std::string plist, const char* plist_in_png, std::string cacheName, int startNum, int endNum, bool isReverse, float duration);
	// �A�j���[�V�����̎��s
	static void anim_action(cocos2d::Sprite* delta);
	// �A�j���[�V�����؂�ւ�
	static void ChangeAnim(cocos2d::Sprite* delta, const char* name);	
private:
	CREATE_FUNC(AnimMng);
};

