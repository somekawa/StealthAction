#include "AnimMng.h"

USING_NS_CC;

std::unique_ptr<AnimMng>AnimMng::s_instance(new AnimMng);

AnimMng::AnimMng()
{
}

AnimMng::~AnimMng()
{
}

void AnimMng::addAnimationCache( std::string plist, const char* plist_in_png, std::string cacheName, int startNum, int endNum, bool isReverse, float duration)
{
	// アニメーションキャッシュはシングルトン
	AnimationCache *animationCache = AnimationCache::getInstance();

	//スプライトシートの準備
	auto cache = SpriteFrameCache::getInstance();

	// パス指定
	cache->addSpriteFramesWithFile(plist);

	// アニメーション画像追加
	Animation* animation = Animation::create();

	for (int i = startNum; i <= endNum; i++)
	{
		auto string = StringUtils::format(plist_in_png, i);	// plistの中だからパスじゃない
		auto idle = cache->getSpriteFrameByName(string);
		animation->addSpriteFrame(idle);
	}

	// アニメーションの間隔
	animation->setDelayPerUnit(duration);

	// アニメーション終了後に最初に戻すかどうか
	animation->setRestoreOriginalFrame(true);

	// 出来たアニメーションをキャッシュに登録
	animationCache->addAnimation(animation, cacheName);
	
	CacheRegistration(animationCache, cacheName, 0);
	//CacheRegistration(, std::string animName,ActorType type);

	// CacheRegistration
	{
		
	}
}

void AnimMng::anim_action(Sprite * delta)
{
	// キャラ専用のきゃっしゅがほしい
	// アニメーションを取得
	AnimationCache *animationCache = AnimationCache::getInstance();
	Animation *animation = animationCache->getAnimation("idle");

	// フレームアニメーションを繰り返す
	RepeatForever *action = RepeatForever::create(Animate::create(animation));

	// アニメーションを実行
	delta->runAction(action);
}

// アニメーション切り替え
void AnimMng::ChangeAnim(cocos2d::Sprite * delta, const char * name)
{
	// 今の動きを止める
	delta->stopAllActions();

	// キャッシュから "ng"という名前で登録されているアニメーションキャッシュ名を取得
	//AnimationCache *animationCache = AnimationCache::getInstance();
	//Animation *animation = animationCache->getAnimation(name);


	// フレームアニメーションは繰り返し
	RepeatForever *action = RepeatForever::create(Animate::create(cache_[0][name]));

	//アニメーションを実行
	delta->runAction(action);
}

void AnimMng::CacheRegistration(cocos2d::AnimationCache* cache, std::string animName,int type)
{
	//cache_[type].push_back(cache->getAnimation(animName));
	cache_[type].emplace(animName, cache->getAnimation(animName));
}
