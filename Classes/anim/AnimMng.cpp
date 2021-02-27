#include "AnimMng.h"

USING_NS_CC;

std::unique_ptr<AnimMng>AnimMng::s_instance(new AnimMng);

AnimMng::AnimMng()
{
}

AnimMng::~AnimMng()
{
}

void AnimMng::addAnimationCache(std::string actorName, std::string animName, int frame, float duration, ActorType type,bool isLoop)
{
	actorNames_[static_cast<int>(type)] = actorName;
	std::string name = "";
	switch (type)
	{
	case ActorType::Player:
		// lightとdarkでファイルを分ける
		if (actorName == "image/PlayerAnimationAsset/player/Light/player_Light")
		{
			//name = "player";
			name = "player_Light";
		}
		else
		{
			name = "player_Dark";
		}
		break;
	case ActorType::Imp:
		name = "imp";

		break;
	case ActorType::Assassin:
		name = "assassin";

		break;
	case ActorType::TwistedCultist:
		name = "twistedCultist";

		break;
	case ActorType::Cultist:
		name = "cultist";

		break;
	case ActorType::BigCultist:
		name = "bigCultist";
		break;
	case ActorType::Max:
		break;
	default:
		break;
	}

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
		SpriteFrame* sprite = cache->getSpriteFrameByName(name + "_" + str);

		animation->addSpriteFrame(sprite);
	}

	// アニメーションの間隔
	animation->setDelayPerUnit(duration);

	// アニメーション終了後に最初に戻すかどうか
	animation->setRestoreOriginalFrame(true);

	// 出来たアニメーションをキャッシュに登録
	animationCache->addAnimation(animation, name + "_" + animName);
	//animationCache_->addAnimation(animation, actorName + "_" + animName);

	// 1アニメーションのキャッシュデータを格納する処理
	CacheRegistration(animationCache, type,name + "_" + animName,isLoop);

	if (frameNum_[static_cast<int>(type)].find(animName) == frameNum_[static_cast<int>(type)].end())
	{
		frameNum_[static_cast<int>(type)].emplace(animName, frame);
	}
}

void AnimMng::addAnimationCache(std::string name, std::string animName, int frame, float duration, bool isLoop)
{
	std::string fileName = "image/EnemyAnimationAsset/" + name + "/" + animName;
	// アニメーションキャッシュはシングルトン
	AnimationCache* animationCache = AnimationCache::getInstance();

	//スプライトシートの準備
	auto cache = SpriteFrameCache::getInstance();

	cache->addSpriteFramesWithFile(fileName + ".plist");

	// アニメーション画像追加
	Animation* animation = Animation::create();

	for (int i = 0; i < frame; i++)
	{
		std::string string = "%d.png";		// plistの中だからパスじゃない
		auto str = StringUtils::format(string.c_str(), i);
		auto s = animName + "_" + str;
		SpriteFrame* sprite = cache->getSpriteFrameByName(animName + "_" + str);

		animation->addSpriteFrame(sprite);
	}

	// アニメーションの間隔
	animation->setDelayPerUnit(duration);

	// アニメーション終了後に最初に戻すかどうか
	animation->setRestoreOriginalFrame(true);

	// 出来たアニメーションをキャッシュに登録
	animationCache->addAnimation(animation,animName);

	// 1アニメーションのキャッシュデータを格納する処理
	CacheRegistration(animationCache,animName, isLoop);
}

void AnimMng::InitAnimation(cocos2d::Sprite& sprite, ActorType type, std::string animName)
{
	Animation* animation = caches_[static_cast<int>(type)][animName];

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

void AnimMng::CacheRegistration(cocos2d::AnimationCache* animCache, const ActorType& type, std::string animName,bool isLoop)
{
	if (caches_[static_cast<int>(type)].count(animName) <= 0)
	{
		// キャラのタイプ別のアニメーションキャッシュに1アニメーションデータを格納
		caches_[static_cast<int>(type)].emplace(animName, animCache->getAnimation(animName));
		// ｱﾆﾒｰｼｮﾝの文字列格納
		animations_[static_cast<int>(type)].emplace_back(animName);
		// 1アニメーションにかかる時間の格納
		animMaxFrame_[static_cast<int>(type)].emplace(animName, caches_[static_cast<int>(type)][animName]->getDelayPerUnit() *
			caches_[static_cast<int>(type)][animName]->getFrames().size());

		// ループフラグを設定
		isLoop_[static_cast<int>(type)][animName] = isLoop;
	}
}

void AnimMng::CacheRegistration(cocos2d::AnimationCache* animCache, std::string animName, bool isLoop)
{
	if (cachesExceptCharacter_.count(animName) <= 0)
	{
		// キャラのタイプ別のアニメーションキャッシュに1アニメーションデータを格納
		cachesExceptCharacter_.emplace(animName, animCache->getAnimation(animName));
		// ループフラグを設定
		isLoopExceptCharacter_.emplace(animName, isLoop);
	}
}

bool AnimMng::IsAnimEnd(const float& delta, ActorType type, std::string animName)
{
	if (delta >= animMaxFrame_[static_cast<int>(type)][animName] - 0.2f)
	{
		return true;
	}
	return false;
}

void AnimMng::AnimDataClear(void)
{
	for (auto& data : caches_)
	{
		data.clear();
	}
}

void AnimMng::FireBallClear(void)
{
	cachesExceptCharacter_.clear();
}
