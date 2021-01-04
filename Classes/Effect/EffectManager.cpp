#include "EffectManager.h"

USING_NS_CC;

EffectManager::EffectManager(Layer& layer)
{
	layer.addChild(this);
}

EffectManager::~EffectManager()
{
}

void EffectManager::update(float delta)
{
	/*if (animate_->isDone())
	{
		isAnimEnd_ = true;
	}*/
}

void EffectManager::Play(const EffectType& eType, cocos2d::Vec2 pos)
{
	pos_ = pos;
	type_ = eType;
	animFrame_ = 0.0f;
	isAnimEnd_ = false;
	animate_ = Animate::create(effectAnimation_[eType]);
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
		effectPath = "";
		effectName = "playerSpawn";
		break;
	case EffectType::EnemySpawn:
		effectPath = "effect/enemySpawn";

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
	//animationCache_->addAnimation(animation, actorName + "_" + animName);
	// effectAnimation_に登録
	effectAnimation_.try_emplace(eType, animation);
}
