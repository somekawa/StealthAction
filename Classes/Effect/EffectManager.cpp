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
		spritePool_.push_back(FXStruct(sprite, false, false, false, 0.0f, {0.0f,0.0f}));
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

void EffectManager::Move(FXStruct fx)
{
	auto move = MoveBy::create(0.0f, Vec2(fx.speed_.x,fx.speed_.y));
	fx.sprite_->runAction(move);
}

const FXStruct& EffectManager::PickUp(std::string effectName,
											cocos2d::Vec2 offset,cocos2d::Vec2 pos, cocos2d::Vec2 speed,
										    int frame, float duration, bool flipFlag, bool loop)
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
	while (spritePool_.at(poolNo_).isActive_)
	{
		// ﾌﾟｰﾙの番号を加算
		poolNo_++;
		// ﾌﾟｰﾙの番号がｴﾌｪｸﾄの最大数まで達したら0に戻す
		if (poolNo_ >= EffectMaxNum)
		{
			poolNo_ = 0;
		}
	}

	spritePool_.at(poolNo_).isActive_ = true;
	// ﾌﾟｰﾙの中からｱｸﾃｨﾌﾞ状態ではないやつの抽出
	// 反転ﾌﾗｸﾞのｾｯﾄ
	SetFlip(flipFlag);
	// ｱﾆﾒｰｼｮﾝ終了ﾌﾗｸﾞの初期化
	spritePool_.at(poolNo_).isAnimEnd_ = false;
	// ﾙｰﾌﾟﾌﾗｸﾞの初期化
	spritePool_.at(poolNo_).isLoop_ = loop;
	// ｴﾌｪｸﾄｱﾆﾒｰｼｮﾝのﾌﾚｰﾑｶｳﾝﾄ変数の初期化
	spritePool_.at(poolNo_).frameCnt_ = 0.0f;
	// ｽﾌﾟﾗｲﾄの名前ｾｯﾄ
	spritePool_.at(poolNo_).sprite_->setName(effectName);
	// 抽出したpoolNo_をﾀｸﾞとしてｾｯﾄ
	spritePool_.at(poolNo_).sprite_->setTag(poolNo_);
	// 左右反転
	spritePool_.at(poolNo_).sprite_->setFlippedX(flipFlag_);
	// ﾌﾟｰﾙに追加したｴﾌｪｸﾄのﾎﾟｼﾞｼｮﾝ設定
	spritePool_.at(poolNo_).sprite_->setPosition(Vec2(pos.x + GetFlipOffset(effectName).x, pos.y + GetFlipOffset(effectName).y));
	// ﾌﾟｰﾙに追加したｴﾌｪｸﾄのvisibleをtrueに
	spritePool_.at(poolNo_).sprite_->setVisible(true);
	// ｽﾋﾟｰﾄﾞの設定
	spritePool_.at(poolNo_).speed_ = speed;
	// ｴﾌｪｸﾄのｱｸﾃｨﾌﾞ状態をtrueに
	spritePool_.at(poolNo_).isActive_ = true;

	Play(effectName, spritePool_.at(poolNo_).sprite_, loop);
	return spritePool_.at(poolNo_);
}

void EffectManager::Play(std::string effectName,Sprite* sprite, bool loop)
{
	if (loop)
	{
		// ｱｸｼｮﾝの設定(loop)
		FiniteTimeAction* repeat = RepeatForever::create(Animate::create(effectAnimation_[effectName]));
		sprite->runAction(repeat);
	}
	else
	{
		// ｱｸｼｮﾝの設定(non-loop)
		FiniteTimeAction* repeat = Repeat::create(Animate::create(effectAnimation_[effectName]), 1);
		sprite->runAction(repeat);
	}
}

void EffectManager::Update(float delta)
{
	for (auto& member : spritePool_)
	{
		if (member.isActive_)
		{
			Move(member);
			member.frameCnt_ += delta;
			auto duration = effectAnimation_[member.sprite_->getName()]->getDuration();
			if (member.frameCnt_ >= duration)
			{
				if (member.isLoop_)
				{
					member.frameCnt_ = 0.0f;
					member.isAnimEnd_ = true;
				}
				else
				{
					member.isActive_ = false;
				}
			}
		}
	}
}

void EffectManager::Reset(const FXStruct& fx)
{
	for (auto& fxData : spritePool_)
	{
		if (!fx.isActive_)
		{
			if (fx.sprite_->getTag() == fxData.sprite_->getTag())
			{
				fxData.isActive_ = false;
			}
		}
	}
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
	// ｱｸﾃｨﾌﾞ状態だと
	if (fx.isActive_)
	{
		// ｴﾌｪｸﾄのﾌﾚｰﾑをdelta値で加算
		fx.frameCnt_ += delta;
		// 各ｴﾌｪｸﾄの総ﾌﾚｰﾑ数の取得
		auto duration = effectAnimation_[fx.sprite_->getName()]->getDuration();
		// ｴﾌｪｸﾄのﾌﾚｰﾑが総ﾌﾚｰﾑ数を超えていたら
		if (fx.frameCnt_ >= duration)
		{
			// そのｴﾌｪｸﾄがﾙｰﾌﾟ再生専用のｴﾌｪｸﾄならば
			if (fx.isLoop_)
			{
				// ﾙｰﾌﾟ再生可能なようにframeをｾﾞﾛｸﾘ
				fx.frameCnt_ = 0.0f;
			}
			// 1回のみの再生ならば
			else
			{
				// ｱｸﾃｨﾌﾞ状態をfalseに
				fx.isActive_ = false;
			}
			return true;
		}

	}
	return false;
}
