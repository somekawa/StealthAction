#pragma once

#include "cocos2d.h"
#include <map>
#include <array>
#include <memory>

#define lpAnimMng AnimMng::GetInstance()

class AnimMng
{
public:
	static AnimMng& GetInstance()
	{
		return *s_instance;
	}
	
	~AnimMng();

	// アニメーションの登録
	void addAnimationCache(std::string plist, const char* plist_in_png, std::string cacheName, int startNum, int endNum, bool isReverse, float duration);
	// アニメーションの実行
	void anim_action(cocos2d::Sprite* delta);
	// アニメーション切り替え
	void ChangeAnim(cocos2d::Sprite* delta, const char* name);	
private:
	AnimMng();

	void CacheRegistration(cocos2d::AnimationCache* cache, std::string animName, int type = 0);

	std::array<std::map <std::string,cocos2d::Animation*>,1> cache_;
	static std::unique_ptr<AnimMng> s_instance;
};

