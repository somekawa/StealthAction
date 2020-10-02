#pragma once

#include "cocos2d.h"

class AnimMng : cocos2d::Sprite
{
public:
	static cocos2d::Sprite* createAnim();

	AnimMng();
	~AnimMng();

	// アニメーションの登録
	static void addAnimationCache(std::string plist, const char* plist_in_png, std::string cacheName, int startNum, int endNum, bool isReverse, float duration);
	// アニメーションの実行
	static void anim_action(cocos2d::Sprite* delta);
	// アニメーション切り替え
	static void ChangeAnim(cocos2d::Sprite* delta, const char* name);	
private:
	CREATE_FUNC(AnimMng);
};

