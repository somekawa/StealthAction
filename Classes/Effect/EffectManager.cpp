#include "EffectManager.h"

USING_NS_CC;

EffectManager::~EffectManager()
{
}

void EffectManager::CreatePools(cocos2d::Layer& layer)
{
	// poolNo_の初期化
	poolNo_ = 0;

	Sprite* sprite;
	// spriteを一定数ためておくﾌﾟｰﾙの作成
	spritePool_ = new Vector<Sprite*>(EffectMaxNum);
	// ﾌﾟｰﾙの容量まで回す
	for (int num = 0; num < EffectMaxNum; num++)
	{
		// 空のｽﾌﾟﾗｲﾄの生成
		sprite = Sprite::create();
		// まず見えなくする
		sprite->setVisible(false);
		// ｴﾌｪｸﾄ用ﾚｲﾔｰにaddChild
		layer.addChild(sprite);
		// ｽﾌﾟﾗｲﾄﾌﾟｰﾙに空のｽﾌﾟﾗｲﾄを入れる
		spritePool_->pushBack(sprite);
	}
}

// effectManagerはシングルトンで
void EffectManager::Update(const Layer& layer)
{

}

void EffectManager::AddEffect(std::string effectName, int frame, float duration, Vec2 offset, Layer& layer, bool isMove)
{
	// 登録されていなければ、ｱﾆﾒｰｼｮﾝの登録をする
	if(effectAnimation_.find(effectName) == effectAnimation_.end())
	{
		// ｴﾌｪｸﾄの画像ﾊﾟｽ
		std::string effectPath = "effect/" + effectName;

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
		animationCache->addAnimation(animation, effectName);

		// ｴﾌｪｸﾄ毎のｱﾆﾒｰｼｮﾝﾃﾞｰﾀの保存
		effectAnimation_.emplace(effectName, animationCache->getAnimation(effectName));
		// ｴﾌｪｸﾄ毎のｵﾌｾｯﾄ値の保存
		offset_.emplace(effectName, offset);
	}

}

void EffectManager::AddEffect(std::string effectName, int frame, float duration, cocos2d::Vec2 offset)
{
	// 登録されていなければ、ｱﾆﾒｰｼｮﾝの登録をする
	if (effectAnimation_.find(effectName) == effectAnimation_.end())
	{
		// ｴﾌｪｸﾄの画像ﾊﾟｽ
		std::string effectPath = "effect/" + effectName;

		// アニメーションキャッシュはシングルトン
		AnimationCache* animationCache = AnimationCache::getInstance();

		//スプライトシートの準備
		auto cache = SpriteFrameCache::getInstance();

		// パス指定
		cache->addSpriteFramesWithFile(effectPath + ".plist");
		// アニメーション画像追加
		Animation * animation = Animation::create();

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
		animationCache->addAnimation(animation, effectName);

		// ｴﾌｪｸﾄ毎のｱﾆﾒｰｼｮﾝﾃﾞｰﾀの保存
		effectAnimation_.emplace(effectName, animationCache->getAnimation(effectName));
		// ｴﾌｪｸﾄ毎のｵﾌｾｯﾄ値の保存
		offset_.emplace(effectName, offset);
	}
	// ｽﾌﾟﾗｲﾄﾌﾟｰﾙの中の特定の番目のﾎﾟｲﾝﾀを取得
	auto curEffect = spritePool_->at(poolNo_);
	// ﾌﾟｰﾙの番号を加算
	poolNo_++;
	// ﾌﾟｰﾙの番号がｴﾌｪｸﾄの最大数まで達したら0に戻す
	if (poolNo_ >= EffectMaxNum)
	{
		poolNo_ = 0;
	}
	// ﾌﾟｰﾙに追加したｴﾌｪｸﾄのﾎﾟｼﾞｼｮﾝ設定
	curEffect->setPosition({ 100.0f + offset.x,200.0f });
	// ﾌﾟｰﾙに追加したｴﾌｪｸﾄ毎のｱｸｼｮﾝの実行
	curEffect->runAction(Animate::create(effectAnimation_[effectName]));
	// ﾌﾟｰﾙに追加したｴﾌｪｸﾄのvisibleをtrueに
	curEffect->setVisible(true);
}

void EffectManager::runaction(std::string effectName)
{
	effectSprites_[effectName].runAction(Animate::create(effectAnimation_[effectName]));
}

// ｴﾌｪｸﾄを再生するoffsetを引数で自由に決める事が可能なように
//void EffectManager::Play(const EffectType& eType, cocos2d::Vec2 pos)
//{
//	// ﾎﾟｼﾞｼｮﾝｾｯﾄ
//	// autoRelease()をいれる可能性あり
//	pos_ = pos;
//	setPosition(pos);
//	// ﾀｲﾌﾟｾｯﾄ
//	type_ = eType;
//	// ｱﾆﾒｰｼｮﾝｺﾏ数ｶｳﾝﾄ変数初期化
//	animFrame_ = 0.0f;
//	// ｱﾆﾒｰｼｮﾝが終了したかのﾌﾗｸﾞ初期化
//	isAnimEnd_ = false;
//	// 現在のｴﾌｪｸﾄｱﾆﾒｰｼｮﾝﾃﾞｰﾀの格納
//	animate_ = Animate::create(effectAnimation_[eType]);
//	// 現在のｱﾆﾒｰｼｮﾝﾃﾞｰﾀでrunActionをする
//	auto action = Repeat::create(Animate::create(effectAnimation_[eType]), 1);
//	runAction(action);
//}

//void EffectManager::Load(EffectType eType,int frame,float duration)
//{
//	std::string effectPath = "";
//	std::string effectName = "";
//	switch (eType)
//	{
//	case EffectType::PlayerSpawn:
//		// ｴﾌｪｸﾄがあるpath名
//		effectPath = "";
//		// ｴﾌｪｸﾄ固有の名前
//		effectName = "playerSpawn";
//		break;
//	case EffectType::EnemySpawn:
//		// ｴﾌｪｸﾄがあるpath名
//		effectPath = "effect/enemySpawn";
//		// ｴﾌｪｸﾄ固有の名前
//		effectName = "enemySpawn";
//		break;
//	case EffectType::PlayerAttack1:
//		effectPath = "";
//
//		effectName = "playerAttack1";
//		break;
//	case EffectType::PlayerAttack2:
//		effectPath = "";
//
//		effectName = "playerAttack2";
//		break;
//	case EffectType::Max:
//		break;
//	default:
//		break;
//	}
//	// アニメーションキャッシュはシングルトン
//	AnimationCache* animationCache = AnimationCache::getInstance();
//
//	//スプライトシートの準備
//	auto cache = SpriteFrameCache::getInstance();
//
//	// パス指定
//	cache->addSpriteFramesWithFile(effectPath + ".plist");
//
//	// アニメーション画像追加
//	Animation* animation = Animation::create();
//
//	for (int i = 0; i < frame; i++)
//	{
//		auto string = effectName + "%d.png";		// plistの中だからパスじゃない
//		auto str = StringUtils::format(string.c_str(), i);
//		SpriteFrame* sprite = cache->getSpriteFrameByName(str);
//
//		animation->addSpriteFrame(sprite);
//	}
//
//	// アニメーションの間隔
//	animation->setDelayPerUnit(duration);
//
//	// アニメーション終了後に最初に戻すかどうか
//	animation->setRestoreOriginalFrame(true);
//
//	// 出来たアニメーションをキャッシュに登録
//	animationCache->addAnimation(animation,effectName);
//	// effectAnimation_に登録
//	effectAnimation_.try_emplace(eType, animation);
//}
