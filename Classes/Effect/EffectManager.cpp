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
	// プールの容量まで回す
	for (int num = 0; num < EffectMaxNum; num++)
	{
		// 空のスプライトの生成
		sprite = Sprite::create();
		// まず見えなくする
		sprite->setVisible(false);
		// エフェクト用レイヤーにaddChild
		layer.addChild(sprite);
		// スプライトプールに空のスプライトを入れる
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

void EffectManager::ClearPool()
{
	spritePool_.clear();
}

void EffectManager::Move(FXStruct fx)
{
	const auto move = MoveBy::create(0.0f, Vec2(fx.speed_.x,fx.speed_.y));
	fx.sprite_->runAction(move);
}

const FXStruct& EffectManager::PickUp(std::string effectName,
											cocos2d::Vec2 offset,cocos2d::Vec2 pos, cocos2d::Vec2 speed,
										    int frame, float duration, bool flipFlag, bool loop,bool animReset)
{
	// 登録されていなければ、アニメーションの登録をする
	if (effectAnimation_.find(effectName) == effectAnimation_.end())
	{
		// エフェクトの画像パス
		std::string effectPath = "effect/" + effectName;

		// アニメーションキャッシュはシングルトン
		AnimationCache* animationCache = AnimationCache::getInstance();

		//スプライトシートの準備
		const auto cache = SpriteFrameCache::getInstance();

		// パス指定
		cache->addSpriteFramesWithFile(effectPath + ".plist");
		// アニメーション画像追加
		Animation* animation = Animation::create();

		for (int i = 0; i < frame; i++)
		{
			const auto string = effectName + "%d.png";		// plistの中だからパスじゃない
			const auto str = StringUtils::format(string.c_str(), i);
			SpriteFrame* sprite = cache->getSpriteFrameByName(str);

			animation->addSpriteFrame(sprite);
		}

		// アニメーションの間隔
		animation->setDelayPerUnit(duration);

		// アニメーション終了後に最初に戻すかどうか
		animation->setRestoreOriginalFrame(animReset);

		// 出来たアニメーションをキャッシュに登録
		animationCache->addAnimation(animation, effectName);

		// エフェクト毎のアニメーションデータの保存
		effectAnimation_.emplace(effectName, animationCache->getAnimation(effectName));
		// エフェクト毎のオフセット値の保存
		offset_.emplace(effectName, offset);
	}

	// スプライトプールの中の特定の番目のポインタを取得
	while (spritePool_.at(poolNo_).isActive_)
	{
		// プールの番号を加算
		poolNo_++;
		// プールの番号がエフェクトの最大数まで達したら0に戻す
		if (poolNo_ >= EffectMaxNum)
		{
			poolNo_ = 0;
		}
	}

	spritePool_.at(poolNo_).isActive_ = true;
	// プールの中からアクティブ状態ではないやつの抽出
	// 反転フラグのセット
	SetFlip(flipFlag);
	// アニメーション終了フラグの初期化
	spritePool_.at(poolNo_).isAnimEnd_ = false;
	// ループフラグの初期化
	spritePool_.at(poolNo_).isLoop_ = loop;
	// エフェクトアニメーションのフレームカウント変数の初期化
	spritePool_.at(poolNo_).frameCnt_ = 0.0f;
	// スプライトの名前セット
	spritePool_.at(poolNo_).sprite_->setName(effectName);
	// 抽出したpoolNo_をタグとしてセット
	spritePool_.at(poolNo_).sprite_->setTag(poolNo_);
	// 左右反転
	spritePool_.at(poolNo_).sprite_->setFlippedX(flipFlag_);
	// プールに追加したエフェクトのポジション設定
	spritePool_.at(poolNo_).sprite_->setPosition(Vec2(pos.x + GetFlipOffset(effectName).x, pos.y + GetFlipOffset(effectName).y));
	// プールに追加したエフェクトのvisibleをtrueに
	spritePool_.at(poolNo_).sprite_->setVisible(true);
	// スピードの設定
	spritePool_.at(poolNo_).speed_ = speed;
	// エフェクトのアクティブ状態をtrueに
	spritePool_.at(poolNo_).isActive_ = true;

	Play(effectName, spritePool_.at(poolNo_).sprite_, loop);
	return spritePool_.at(poolNo_);
}

void EffectManager::Play(std::string effectName,Sprite* sprite, bool loop)
{
	if (loop)
	{
		// アクションの設定(loop)
		FiniteTimeAction* repeat = RepeatForever::create(Animate::create(effectAnimation_[effectName]));
		sprite->runAction(repeat);
	}
	else
	{
		// アクションの設定(non-loop)
		FiniteTimeAction* repeat = Repeat::create(Animate::create(effectAnimation_[effectName]), 1);
		sprite->runAction(repeat);
	}
}

void EffectManager::Update(float delta)
{
	for (auto& member : spritePool_)
	{
		if (!member.isActive_)
		{
			continue;
		}

		Move(member);
		member.frameCnt_ += delta;
		if (member.frameCnt_ >= effectAnimation_[member.sprite_->getName()]->getDuration())
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

void EffectManager::Reset(const FXStruct& fx)
{
	for (auto& fxData : spritePool_)
	{
		// エフェクトが再生中の場合は処理を行わない
		if (fx.isActive_)
		{
			continue;
		}

		if (fx.sprite_->getTag() == fxData.sprite_->getTag())
		{
			fxData.isActive_ = false;
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
	// アクティブ状態だと
	if (fx.isActive_)
	{
		// エフェクトのフレームをdelta値で加算
		fx.frameCnt_ += delta;

		// エフェクトのフレームが総フレーム数を超えていない場合は処理を行わない
		if (fx.frameCnt_ < effectAnimation_[fx.sprite_->getName()]->getDuration())
		{
			return false;
		}

		// そのエフェクトがループ再生専用のエフェクトならば
		if (fx.isLoop_)
		{
			// ループ再生可能なようにframeをゼロクリア
			fx.frameCnt_ = 0.0f;
		}
		else
		{
			// 1回のみの再生ならばアクティブ状態をfalseに
			fx.isActive_ = false;
		}
		return true;
	}
	return false;
}