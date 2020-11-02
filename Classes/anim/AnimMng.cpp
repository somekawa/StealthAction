#include "AnimMng.h"

USING_NS_CC;

std::unique_ptr<AnimMng>AnimMng::s_instance(new AnimMng);

AnimMng::AnimMng()
{
}

AnimMng::~AnimMng()
{
}

void AnimMng::addAnimationCache(std::string actorName, std::string animName, int frame, float duration, ActorType type)
{
	actorNames_[static_cast<int>(type)] = actorName;

	// アニメーションキャッシュはシングルトン
	AnimationCache *animationCache = AnimationCache::getInstance();

	//スプライトシートの準備
	auto cache = SpriteFrameCache::getInstance();

	// パス指定
	auto pListStr = actorName + "_" + animName;
	cache->addSpriteFramesWithFile(pListStr + ".plist");

	// アニメーション画像追加
	Animation* animation = Animation::create();

	for (int i = 0; i < frame; i++)
	{
		auto string = animName + "%d.png";		// plistの中だからパスじゃない
		auto str = StringUtils::format(string.c_str(), i);
		SpriteFrame* sprite = cache->getSpriteFrameByName(str);
		animation->addSpriteFrame(sprite);
	}

	// アニメーションの間隔
	animation->setDelayPerUnit(duration);

	// アニメーション終了後に最初に戻すかどうか
	animation->setRestoreOriginalFrame(true);

	// 出来たアニメーションをキャッシュに登録
	animationCache->addAnimation(animation, animName);
	//animationCache_->addAnimation(animation, actorName + "_" + animName);


	// 1アニメーションのキャッシュデータを格納する処理
	CacheRegistration(animationCache, type, animName);

	if (frameNum_[static_cast<int>(type)].find(animName) == frameNum_[static_cast<int>(type)].end())
	{
		frameNum_[static_cast<int>(type)].emplace(animName, frame);
	}
}

void AnimMng::InitAnimation(cocos2d::Sprite& sprite, ActorType type)
{
	Animation* animation = caches_[static_cast<int>(type)]["Run"];

	RepeatForever* action = RepeatForever::create(Animate::create(animation));

	sprite.runAction(action);
}

void AnimMng::ChangeAnimation(cocos2d::Sprite& sprite, std::string name, bool loop, ActorType type)
{
	// 今の動きを止める
	sprite.stopAllActions();

	// キーによるアニメーションの取り出し
	Animation* animation = caches_[static_cast<int>(type)][name];

	// フレームアニメーションは繰り返し
	if (loop)
	{
		RepeatForever* action = RepeatForever::create(Animate::create(animation));

		//アニメーションを実行
		sprite.runAction(action);
	}
	else
	{
		auto action_ = Repeat::create(Animate::create(animation), 1);
		//アニメーションを実行
		sprite.runAction(action_);
	}
}

void AnimMng::CacheRegistration(cocos2d::AnimationCache* animCache, const ActorType& type, std::string animName)
{
	// キャラのタイプ別のアニメーションキャッシュに1アニメーションデータを格納
	caches_[static_cast<int>(type)].emplace(animName, animCache->getAnimation(animName));
	// ｱﾆﾒｰｼｮﾝの文字列格納
	animations_[static_cast<int>(type)].emplace_back(animName);
	// 1アニメーションにかかる時間の格納
	animMaxFrame_[static_cast<int>(type)].emplace(animName, caches_[static_cast<int>(type)][animName]->getDelayPerUnit()*
		caches_[static_cast<int>(type)][animName]->getFrames().size());
}

bool AnimMng::IsAnimEnd(const float& delta, ActorType type, std::string animName)
{
	if (delta >= animMaxFrame_[static_cast<int>(type)][animName] - 0.2f)
	{
		return true;
	}
	return false;
}