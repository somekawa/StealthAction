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

	// アニメーションの登録
	// param@: actorName:: キャラクターの名前
	// param@: animName :: アニメーションの名前
	// param@: cacheName:: 登録するアニメーション名
	// param@: frame    :: アニメーションの総フレーム
	// param@: duration :: アニメーションを再生する間隔
	void addAnimationCache(std::string actorName, std::string animName, int frame, float duration, ActorType type);
	// アニメーションの初期化と実行
	void InitAnimation(cocos2d::Sprite& sprite, ActorType type);
	// アニメーションの実行
	//void anim_action(cocos2d::Sprite* delta);
	// アニメーションの変更
	void ChangeAnimation(cocos2d::Sprite& sprite, std::string name, bool loop, ActorType type);
	// アニメーションの終了チェック関数
	// アニメーションが終了していたらtrueを返す
	// param@:sprite Actorのスプライト
	// param@:animTag チェックしたいアニメーションのタグ
	// tagでアニメーションを探してチェックするアニメーションを決める
	bool IsAnimEnd(const float& delta, ActorType type, std::string animName);
	const float& GetAnimationMaxFrame(const ActorType& type, std::string animName)
	{
		return animMaxFrame_[static_cast<int>(type)][animName];
	}
	// アニメーション集の取得
	const std::vector<std::string>& GetAnimations(const ActorType& type) const
	{
		return animations_[static_cast<int>(type)];
	}

	// アニメーションフレーム数の取得
	const int& GetFrameNum(const ActorType& type, const std::string& animName)
	{
		return frameNum_[static_cast<int>(type)][animName];
	}
private:
	AnimMng();

	void operator=(AnimMng&) = delete;

	// キャラ別のアニメーションキャッシュの登録
	void CacheRegistration(cocos2d::AnimationCache* animCache, const ActorType& type, std::string animName);

	// キャラクターのタイプ別のアニメーションキャッシュ
	std::array<std::map<std::string, cocos2d::Animation*>, static_cast<int>(ActorType::Max)> caches_;
	// 1キャラの1アニメーションにかかる時間
	std::array<std::map<std::string, float>, static_cast<int>(ActorType::Max)> animMaxFrame_;

	// ｷｬﾗのｱﾆﾒｰｼｮﾝ集
	std::array<std::vector<std::string>, static_cast<int>(ActorType::Max)> animations_;
	// ｷｬﾗのActor毎の名前
	std::array<std::string, static_cast<int>(ActorType::Max)> actorNames_;
	// アニメーションフレーム数
	std::array<std::map<std::string, int>, static_cast<int>(ActorType::Max)> frameNum_;

	static std::unique_ptr<AnimMng> s_instance;
};

