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
	//// spriteを一定数ためておくﾌﾟｰﾙの作成
	//spritePool_ = new std::vector<FXStruct>(EffectMaxNum);
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
		spritePool_.push_back(FXStruct(sprite,false,false,false,0.0f));
	}
}

void EffectManager::SetFlip(bool flg)
{
	flipFlag_ = flg;
}

cocos2d::Vec2 EffectManager::GetFlipOffset(std::string effectName)
{
	Vec2 offset = {};
	// 左右反転している
	if (flipFlag_)
	{
		offset = offset_[effectName];
	}
	// 反転していない
	else
	{
		offset = Vec2{ -offset_[effectName].x,offset_[effectName].y };
	}
	return offset;
}

// effectManagerはシングルトンで
void EffectManager::Update(float delta)
{
	/*for (auto fx = spritePool_.begin(); fx != spritePool_.end(); fx++)
	{
		if (fx->isActive_ && fx->isLoop_)
		{
			fx->frameCnt_ += delta;

			auto duration = effectAnimation_[fx->sprite_->getName()]->getDuration();

			if (fx->frameCnt_ >= duration)
			{
				fx->isActive_ = false;
				fx->sprite_->setVisible(false);
			}
		}
	}*/
}

void EffectManager::Move(cocos2d::Sprite* sprite, cocos2d::Vec2 speed)
{
	auto sp = Vec2{ 0.0f,0.0f };
	// 右向きならば+方向に動く
	if (flipFlag_)
	{
		sp = speed;
	}
	else
	{
		// 左向きならば-方向に動く
		sp = Vec2{ -speed.x,speed.y };
	}
	auto move = MoveTo::create(0.0f,sprite->getPosition() + sp);

	sprite->runAction(move);
}

const FXStruct& EffectManager::createEffect(std::string effectName, int frame, float duration, cocos2d::Vec2 offset,cocos2d::Vec2 pos,bool loop)
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
	// ｽﾌﾟﾗｲﾄﾌﾟｰﾙの中の特定の番目のﾎﾟｲﾝﾀを取得
	auto curEffect = spritePool_.at(poolNo_);
	// ﾌﾟｰﾙの番号を加算
	poolNo_++;
	// ﾌﾟｰﾙの番号がｴﾌｪｸﾄの最大数まで達したら0に戻す
	if (poolNo_ >= EffectMaxNum)
	{
		poolNo_ = 0;
	}
	// ｱﾆﾒｰｼｮﾝ終了ﾌﾗｸﾞの初期化
	curEffect.isAnimEnd_ = false;
	// ﾙｰﾌﾟﾌﾗｸﾞの初期化
	curEffect.isLoop_ = loop;
	// ｴﾌｪｸﾄｱﾆﾒｰｼｮﾝのﾌﾚｰﾑｶｳﾝﾄ変数の初期化
	curEffect.frameCnt_ = 0.0f;
	// ｽﾌﾟﾗｲﾄの名前ｾｯﾄ
	curEffect.sprite_->setName(effectName);
	// 左右反転
	curEffect.sprite_->setFlippedX(flipFlag_);
	// ﾌﾟｰﾙに追加したｴﾌｪｸﾄのﾎﾟｼﾞｼｮﾝ設定
	curEffect.sprite_->setPosition(Vec2(pos.x + GetFlipOffset(effectName).x, pos.y + GetFlipOffset(effectName).y));
	// ﾌﾟｰﾙに追加したｴﾌｪｸﾄのvisibleをtrueに
	curEffect.sprite_->setVisible(true);
	// ｴﾌｪｸﾄのｱｸﾃｨﾌﾞ状態をtrueに
	curEffect.isActive_ = true;

	return curEffect;
}

void EffectManager::PlayWithOnce(FXStruct& fx,std::string effectName)
{
	// アクションの設定
	FiniteTimeAction* repeat = Repeat::create(Animate::create(effectAnimation_[effectName]), 1);

	fx.sprite_->runAction(repeat);
}

void EffectManager::PlayWithLoop(FXStruct& fx,std::string effectName)
{
	// アクションの設定
	FiniteTimeAction* repeat = RepeatForever::create(Animate::create(effectAnimation_[effectName]));

	fx.sprite_->runAction(repeat);
}

bool EffectManager::AnimEndChecker(FXStruct& fx,float delta)
{
	if (fx.isActive_)
	{
		fx.frameCnt_ += delta;
		auto duration = effectAnimation_[fx.sprite_->getName()]->getDuration();

		if (fx.isLoop_)
		{
			if (fx.frameCnt_ >= duration)
			{
				// ｱｸﾃｨﾌﾞ状態をfalseに
				fx.isActive_ = false;
				return true;
			}
		}
	}
	return false;
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
