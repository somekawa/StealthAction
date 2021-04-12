#pragma once
#include <cocos2d.h>
#include <map>
#include <array>
#include <vector>
#include <memory>
#include "obj/Actor.h"
#include "obj/Player.h"

#define lpAnimMng AnimMng::GetInstance()

// アニメーション設定に必要な情報
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

	// アニメーションの登録
	// param@: actorName:: キャラクターの名前
	// param@: animName :: アニメーションの名前
	// param@: cacheName:: 登録するアニメーション名
	// param@: frame    :: アニメーションの総フレーム
	// param@: duration :: アニメーションを再生する間隔
	void addAnimationCache(std::string actorName, std::string animName, int frame, float duration, ActorType type,bool isLoop);
	void addAnimationCache(std::string name, std::string animName, int frame, float duration, bool isLoop);

	// アニメーションの初期化と実行
	void InitAnimation(cocos2d::Sprite& sprite, ActorType type, std::string animName);
	// アニメーションの変更
	void ChangeAnimation(cocos2d::Sprite& sprite, std::string name, bool loop, ActorType type);
	// アニメーションの終了チェック関数
	// アニメーションが終了していたらtrueを返す
	// param@:sprite Actorのスプライト
	// param@:animTag チェックしたいアニメーションのタグ
	// tagでアニメーションを探してチェックするアニメーションを決める
	bool IsAnimEnd(const float& delta, ActorType type, std::string animName);

	void AnimDataClear(void);
	void FireBallClear(void);

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
	// アニメーションキャッシュの取得
	cocos2d::Animation* GetAnimationCache(ActorType type, std::string animationName)
	{
		return caches_[static_cast<int>(type)][animationName];
	}
	// アニメーションキャッシュ(ｷｬﾗｸﾀｰ以外)の取得
	cocos2d::Animation* GetAnimationCache(std::string animationName)
	{
		return cachesExceptCharacter_[animationName];
	}
	// アニメーションループフラグの取得
	const bool& GetIsLoop(ActorType type, std::string animationName)
	{
		return isLoop_[static_cast<int>(type)][animationName];
	}

	const bool& GetIsLoop(std::string animationName)
	{
		return isLoopExceptCharacter_[animationName];
	}

	// アニメーションの最大コマ数
	int GetAnimationMaxNum(ActorType type, std::string animationName);

	// アニメーションデータの読み込み
	void XmlAnimDataRead(std::string string, ActorType type);
	// プレイヤーのパラメーター読み込み
	std::vector<PlParam> XmlPlParamRead(void);
private:
	AnimMng();

	void operator=(AnimMng&) = delete;

	// キャラ別のアニメーションキャッシュの登録
	void CacheRegistration(cocos2d::AnimationCache* animCache, const ActorType& type, std::string animName,bool isLoop);
	void CacheRegistration(cocos2d::AnimationCache* animCache,std::string animName, bool isLoop);
	// キャラクターのタイプ別のアニメーションキャッシュ
	std::array<std::map<std::string, cocos2d::Animation*>, static_cast<int>(ActorType::Max)> caches_;
	// キャラクター以外のアニメーションキャッシュ
	std::map<std::string, cocos2d::Animation*> cachesExceptCharacter_;
	// 1キャラの1アニメーションにかかる時間
	std::array<std::map<std::string, float>, static_cast<int>(ActorType::Max)> animMaxFrame_;

	// キャラクターのアニメーション集
	std::array<std::vector<std::string>, static_cast<int>(ActorType::Max)> animations_;
	// キャラクターのActor毎の名前
	std::array<std::string, static_cast<int>(ActorType::Max)> actorNames_;
	// アニメーションフレーム数
	std::array<std::map<std::string, int>, static_cast<int>(ActorType::Max)> frameNum_;

	// アニメーションのループフラグ
	std::array<std::map< std::string,bool>, static_cast<int>(ActorType::Max)> isLoop_;
	// キャラクター以外のループフラグ
	std::map<std::string, bool> isLoopExceptCharacter_;

	static std::unique_ptr<AnimMng> s_instance;
};