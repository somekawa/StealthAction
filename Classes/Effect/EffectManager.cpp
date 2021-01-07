#include "EffectManager.h"

USING_NS_CC;

EffectManager::EffectManager(Layer& layer)
{
	// GameSceneのEffectLayerにぶら下げる
	layer.addChild(this);
}

EffectManager::~EffectManager()
{
}

void EffectManager::update(float delta)
{
	//if (animate_->isDone())
	//{
	//	isAnimEnd_ = true;
	//}
}

void EffectManager::Play(const EffectType& eType, cocos2d::Vec2 pos)
{
	// ﾎﾟｼﾞｼｮﾝｾｯﾄ
	pos_ = pos;
	setPosition(pos);
	// ﾀｲﾌﾟｾｯﾄ
	type_ = eType;
	// ｱﾆﾒｰｼｮﾝｺﾏ数ｶｳﾝﾄ変数初期化
	animFrame_ = 0.0f;
	// ｱﾆﾒｰｼｮﾝが終了したかのﾌﾗｸﾞ初期化
	isAnimEnd_ = false;
	// 現在のｴﾌｪｸﾄｱﾆﾒｰｼｮﾝﾃﾞｰﾀの格納
	animate_ = Animate::create(effectAnimation_[eType]);
	// 現在のｱﾆﾒｰｼｮﾝﾃﾞｰﾀでrunActionをする
	auto action = Repeat::create(Animate::create(effectAnimation_[eType]), 1);
	runAction(action);
}

void EffectManager::Load(EffectType eType,int frame,float duration)
{
	std::string effectPath = "";
	std::string effectName = "";
	switch (eType)
	{
	case EffectType::PlayerSpawn:
		// ｴﾌｪｸﾄがあるpath名
		effectPath = "";
		// ｴﾌｪｸﾄ固有の名前
		effectName = "playerSpawn";
		break;
	case EffectType::EnemySpawn:
		// ｴﾌｪｸﾄがあるpath名
		effectPath = "effect/enemySpawn";
		// ｴﾌｪｸﾄ固有の名前
		effectName = "enemySpawn";
		break;
	case EffectType::PlayerAttack1:
		effectPath = "";

		effectName = "playerAttack1";
		break;
	case EffectType::PlayerAttack2:
		effectPath = "";

		effectName = "playerAttack2";
		break;
	case EffectType::Max:
		break;
	default:
		break;
	}
	// アニメーションキャッシュはシングルトン
	AnimationCache* animationCache = AnimationCache::getInstance();

	//スプライトシートの準備
	auto cache = SpriteFrameCache::getInstance();

	// パス指定
	cache->addSpriteFramesWithFile(effectPath + ".plist");

	// アニメーション画像追加
	Animation* animation = Animation::create();

	for (int i = 0; i < frame; i++)
	{
		auto string = effectName + "%d.png";		// plistの中だからパスじゃない
		auto str = StringUtils::format(string.c_str(), i);
		SpriteFrame* sprite = cache->getSpriteFrameByName(str);

		animation->addSpriteFrame(sprite);
	}

	// アニメーションの間隔
	animation->setDelayPerUnit(duration);

	// アニメーション終了後に最初に戻すかどうか
	animation->setRestoreOriginalFrame(true);

	// 出来たアニメーションをキャッシュに登録
	animationCache->addAnimation(animation,effectName);
	// effectAnimation_に登録
	effectAnimation_.try_emplace(eType, animation);
}
