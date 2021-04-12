// 全て担当
#include <functional>
#include "anim/AnimMng.h"
#include "_Debug/_DebugConOut.h"
#include "Scene/GameScene.h"
#include "anim/ActionCtl.h"
#include "ActionRect.h"
#include "Loader/CollisionLoader.h"
#include "../Skill/SkillMng.h"
#include "../Skill/SkillBase.h"
#include "../Skill/SkillCode/SkillA.h"
#include "../Skill/SkillCode/SkillB.h"
#include "../Skill/SkillCode/SkillC.h"
#include "../Loader/FileLoder.h"
#include "../HPGauge.h"
#include "SoundMng.h"
#include "ResidualShadow.h"
#include "Player.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "input/OPRT_key.h"
#else
#include "input/OPRT_touch.h"
#endif

USING_NS_CC;

int Player::no_ = 0;

// 画像サイズを取得する関数があったようなきがする
// getContentSizeを試したけど取得不可能だった
Player::Player(int hp,Layer& myLayer, Layer& enemyLayer, SkillBase* skillBasePtr):
	Actor(hp,myLayer),enemyList_(enemyLayer), myNo_(no_),charSize_(15.0f * 3.0f,25.0f * 3.0f)
{
	skillBase_ = skillBasePtr;
	std::list<std::string> path;
	path.push_back("skill_data");
	skillSprite_ = nullptr;

	const auto visibleSize = Director::getInstance()->getVisibleSize();
	const Vec2 origin	   = Director::getInstance()->getVisibleOrigin();

	lpSoundMng.AddSound("recovery" , "BGM/Recovery.mp3" , SoundType::SE);
	lpSoundMng.AddSound("burst"	   , "BGM/s-burst01.mp3", SoundType::SE);
	lpSoundMng.AddSound("burst2"   , "BGM/s-burst02.mp3", SoundType::SE);
	lpSoundMng.AddSound("Transform", "BGM/se_maoudamashii_element_fire01.mp3", SoundType::SE);
	lpSoundMng.AddSound("Knife"	   , "BGM/Knife.mp3"    , SoundType::SE);
	lpSoundMng.AddSound("Dash"	   , "BGM/Dash.mp3"     , SoundType::SE);

	// キー入力かタッチ操作か判断
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	// thisの意味
	oprtState_ = new OPRT_key(this);
#else
	oprtState_ = new OPRT_touch(this);
#endif

	type_ = ActorType::Player;
	ActModuleRegistration();

	currentAnimation_ = "Non";
	actionOld_ = "Run";
	direction_ = Direction::Right;

	AnimRegistrator();

	//@@@イニシャライズ時の{ }と( )の違いについて調べて、
	//それに合わせた使い方に書き換えること!!
	setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y + 200));
	no_++;
	type_ = ActorType::Player;
	playerMode_ = "player_Light_";

	attackMotion_ = 00000000;

	// パラメータのデータを読み込む
	plParam_ = lpAnimMng.XmlPlParamRead();
	int tmpNum = 0;
	for (auto data : plParam_)
	{
		// それぞれのモードの最大連撃数でリサイズし、アニメーション名の情報を入れる
		attackMotionName_[plParam_[tmpNum].modeName].resize(plParam_[tmpNum].attackChain);
		for (int num = 0; num < data.attackChain; num++)
		{
			attackMotionName_[plParam_[tmpNum].modeName][num] = data.chainAnimName[num].first;
		}
		tmpNum++;
	}

	bitFlg_ = { 0,0,0,0,0,0,0 };

	hp_ = hp;
	for (auto anim : lpAnimMng.GetAnimations(type_))
	{
		// colliderBoxのLoad
		lpCol.ReadData(collider_, anim);
	}

	gameOverAction_ = nullptr;

	// 攻撃矩形のサイズ設定
	attackRect_.size_ = Size(30.0f, 30.0f);

	SkillTbl_   = { "magic"  , "fireBall" ,"heal" };
	SkillSETbl_ = { "burst2" , "burst"    ,"recovery" };

	// 残像
	resShadow_ = std::make_shared<ResidualShadow>();
	resShadow_->CreateResidualShadow(*this, myLayer, 2);
	this->SetIsAttacking(false);
}

Player::~Player()
{
	// 動的なインスタンスをしたらdeleteを忘れないように
	delete oprtState_;
	delete skillBase_;
}

Player* Player::CreatePlayer(int hp, Layer& myLayer, Layer& enemyLayer, SkillBase* skillBasePtr)
{
	Player* pRet = new(std::nothrow) Player(hp, myLayer, enemyLayer, skillBasePtr);
	if (pRet && pRet->init())
	{
		pRet->autorelease();
	}
	else
	{
		delete pRet;
		pRet = nullptr;
	}
	return pRet;
}

// 毎フレーム更新
void Player::update(float delta)
{
	// getnameがgamesceneでない場合、何もせずreturn処理
	if (Director::getInstance()->getRunningScene()->getName() != "GameScene")
	{
		return;
	}

	// 死亡状態の更新と確認(deathFlg_がtrueならアップデートをすぐ抜けるようにする)
	if (bitFlg_.deathFlg)
	{
		if (GameOverAction())
		{
			bitFlg_.gameOverSceneFlg = true;
			return;
		}
		return;
	}

	// キャラクター切替
	if (oprtState_->GetNowData()[static_cast<int>(BUTTON::Transfrom)] && !oprtState_->GetOldData()[static_cast<int>(BUTTON::Transfrom)])
	{
		lpSoundMng.PlayBySoundName("Transform", 0.5f);
	}

	SkillAction();

	// 現在のフレームを整数値で取得
	animationFrame_int_ = GetAnimationFrameInt(playerMode_ + currentAnimation_);

	// ActionCtlクラスの更新
	actCtl_.Update(type_,*this);

	// 入力情報(keyかtouch)の更新
	oprtState_->update();	
	
	// 壁スライド時のみアンカーポイントを移動させる(その他は画像の下中央)
	if (currentAnimation_ == "Wall_Slide")
	{
		// 方向：左 = アンカー右端,方向：右 = アンカー左端
		this->setAnchorPoint(direction_ == Direction::Left ? Vec2(1.0f, 0.0f) : Vec2(0.0f, 0.0f));
	}
	else
	{
		// アンカーポイント下中央
		this->setAnchorPoint(Vec2(0.5f, 0.0f));
	}

	// スライド終了時に左右向きを反転させる
	if (currentAnimation_ != "Wall_Slide" && actionOld_ == "Wall_Slide")
	{
		direction_ = (direction_ == Direction::Right ? Direction::Left : Direction::Right);
	}

	// クラス名がHPBarの方が英語圏では一般的、修正しておくこと!
	const auto nowScene = dynamic_cast<Game*>(Director::getInstance()->getRunningScene());
	const auto hpBar    = dynamic_cast<HPGauge*>(nowScene->getChildByTag(static_cast<int>(zOrder::FRONT))->getChildByName("PL_HPgauge"));

	// darkモード(攻撃力は上がるが、HPは減少し続ける)
	if (playerMode_ == "player_Dark_")
	{
		// HP減少処理
		hpBar->SetHP(hpBar->GetHP() - 0.05f);	// スリップダメージ設定
	}

	// ダメージをくらった際の処理(この中にdeath処理を書いてしまうと、darkモードで死亡したときに対応できない)
	if (isHitAttack_)
	{
		// HP減少処理
		hpBar->SetHP(hpBar->GetHP() - 10.0f);
		isHitAttack_ = false;
	}

	// 0を下回った時に生存フラグと死亡フラグを切り替える
	if (hpBar->GetHP() <= 0.0f)
	{
		isAlive_  = false;
		bitFlg_.deathFlg = true;
		deathPos_ = getPosition();
	}

	AttackMotion(delta);
	TransformMotion(delta);
	DashMotion(delta);

	// アニメーションが切り替わるタイミングで呼ばれる再生処理
	if (currentAnimation_ != actionOld_)
	{
		if (currentAnimation_ == "AttackFirst")
		{
			lpSoundMng.PlayBySoundName("Knife");
			attackMotion_ |= ATTAKFLAG;	// AttackFirstのフラグをオン
			// 攻撃エフェクトの追加
			lpEffectMng.PickUp("attack", { 55.0f, 50.0f }, getPosition(), { 0.0f,0.0f }, 5, 0.04f,
							   (direction_ == Direction::Right ? true : false), false, true);
		}

		animationFrame_int_ = 0;
		lpAnimMng.ChangeAnimation(*this, playerMode_ + currentAnimation_, true, ActorType::Player);
	}
	actionOld_ = currentAnimation_;
	SetCollider();
}

void Player::AttackMotion(float delta)
{
	auto keyLambda = [&](bool flag) {
		if (oprtState_->GetNowData()[static_cast<int>(BUTTON::Attack)] && !oprtState_->GetOldData()[static_cast<int>(BUTTON::Attack)])
		{
			return true;
		}
		return flag;
	};

	auto frameLambda = [&]() {
		// frame計算
		animationFrame_ += delta;
		animationFrame_int_ = GetAnimationFrameInt(playerMode_ + currentAnimation_);
		if (animationFrame_int_ <= lpAnimMng.GetAnimationMaxNum(ActorType::Player, playerMode_ + currentAnimation_))
		{
			currentCol_ = collider_[playerMode_ + currentAnimation_][animationFrame_int_];
		}
	};

	// 攻撃アニメーションの設定
	for (unsigned int num = 0; num < attackMotionName_[playerMode_].size(); num++)
	{
		// フラグの状態を確認
		if (attackMotion_ & (ATTAKFLAG << num))
		{
			currentAnimation_ = attackMotionName_[playerMode_][num];
			if (bitFlg_.oldPosKeepFlg_)
			{
				break;
			}
			if (playerMode_ == "player_Dark_")
			{
				const Vec2 tmpPos(this->getPosition());
				this->setPosition(tmpPos.x, tmpPos.y - 15.0f);	// 位置補正を入れないと浮いて見える
			}
			bitFlg_.oldPosKeepFlg_ = true;
			break;
		}
	}

	// 連撃数を可変にできるように、最大数を決めてループで処理が行えるようにしたほうがいい
	for (unsigned int i = 0; i < attackMotionName_[playerMode_].size(); i++)
	{
		// 現在のアニメーション名と一致していないときはcontinue
		if (currentAnimation_ != attackMotionName_[playerMode_][i])
		{
			continue;
		}

		// 最終アニメーションではキーチェックを行わないようにする
		if (i < attackMotionName_[playerMode_].size() - 1)
		{
			// キー押下時に次の攻撃モーションのビットを立てる
			if (keyLambda(attackMotion_ & (ATTAKFLAG << (i + 1))))
			{
				attackMotion_ |= ATTAKFLAG << (i + 1);
			}
		}

		frameLambda();

		if (animationFrame_ <= lpAnimMng.GetAnimationMaxFrame(ActorType::Player, playerMode_ + currentAnimation_))
		{
			currentAnimation_ = attackMotionName_[playerMode_][i];
			isAttacking_ = true;
		}
		else
		{
			// 攻撃フラグが立っていないとき
			if (!(attackMotion_ & (ATTAKFLAG << (i + 1))))
			{
				currentAnimation_ = "Look_Intro";
				this->SetIsAttacking(false);
			}
			else
			{
				lpSoundMng.PlayBySoundName("Knife");
				if (i < attackMotionName_[playerMode_].size() - 1)
				{
					currentAnimation_ = attackMotionName_[playerMode_][i + 1];
					// 攻撃エフェクトの追加
					lpEffectMng.PickUp("attack", { 55.0f, 50.0f }, getPosition(), { 0.0f,0.0f }, 5, 0.04f,
										(direction_ == Direction::Right ? true : false), false, true);
				}
			}

			// darkとlightで画像サイズが違うから補正
			if (playerMode_ == "player_Dark_")
			{
				const Vec2 tmpPos(this->getPosition());
				this->setPosition(tmpPos.x, tmpPos.y + 15.0f); // 位置補正を戻す
			}
			animationFrame_ = 0.0f;
			attackMotion_  &= ~(ATTAKFLAG << i);	// フラグのオフ
			isAttacking_    = false;
			bitFlg_.oldPosKeepFlg_  = false;
		}
	}
}

void Player::TransformMotion(float delta)
{
	if (bitFlg_.transformFlg)
	{
		animationFrame_ += delta;
		if (animationFrame_ < lpAnimMng.GetAnimationMaxFrame(ActorType::Player, playerMode_ + currentAnimation_))
		{
			currentAnimation_ = "Transform";
		}
		else
		{
			currentAnimation_ = "Look_Intro";
			bitFlg_.transformFlg = false;
			const Vec2 tmpPos(this->getPosition());
			this->setPosition(tmpPos.x, tmpPos.y + 10.0f);	// 位置補正分戻す
			playerMode_ = (playerMode_ == "player_Light_" ? "player_Dark_" : "player_Light_");
			animationFrame_ = 0.0f;
		}
	}
	else
	{
		if (oprtState_->GetNowData()[static_cast<int>(BUTTON::Transfrom)] && !oprtState_->GetOldData()[static_cast<int>(BUTTON::Transfrom)])
		{
			currentAnimation_    = "Transform";
			bitFlg_.transformFlg = true;
			const Vec2 tmpPos(this->getPosition());
			this->setPosition(tmpPos.x, tmpPos.y - 10.0f);	// 位置補正を入れないと浮いて見える
		}
	}
}

void Player::DashMotion(float delta)
{
	// 当たり判定
	auto colLambda = [&](Vec2 move) {
		const auto CollisionData = dynamic_cast<TMXLayer*>(Director::getInstance()->getRunningScene()->getChildByTag(static_cast<int>(zOrder::BG))->getChildByName("MapData")->getChildByName("col"));
		const Vec2 plCheckPoint1(this->getPosition() + move);
		const Vec2 plCheckPoint1Chip(plCheckPoint1 / CollisionData->getMapTileSize().width);
		const Vec2 plCheckPoint1Pos (plCheckPoint1Chip.x, CollisionData->getLayerSize().height - plCheckPoint1Chip.y);
		if ((CollisionData->getTileGIDAt(plCheckPoint1Pos)) != 0)
		{
			currentAnimation_ = "Look_Intro";
			bitFlg_.dashFlg   = false;
			animationFrame_   = 0.0f;
			resShadow_->ResShadowEnd();
		}
	};

	if (!bitFlg_.dashFlg)
	{
		// ダッシュアクション開始
		if (oprtState_->GetNowData()[static_cast<int>(BUTTON::Dash)] && !oprtState_->GetOldData()[static_cast<int>(BUTTON::Dash)])
		{
			currentAnimation_ = "Dash";
			bitFlg_.dashFlg   = true;
			lpSoundMng.PlayBySoundName("Dash");
		}
	}
	else
	{
		resShadow_->Start(*this);
		currentAnimation_ = "Dash";
		animationFrame_  += delta;

		// 移動値計算処理(公式 : y = exp(-pow(x,2)))
		// exp…ネイピア数
		const auto move = exp(-pow(Calculation(animationFrame_), 2));
		const Vec2 dashSpeed(move * DashMove, 0);
		if (direction_ == Direction::Right)
		{
			resShadow_->Move(this->getPosition(), dashSpeed);
			runAction(cocos2d::MoveBy::create(0.0f, dashSpeed));
			colLambda({ dashSpeed.x + charSize_.x, dashSpeed.y + charSize_.y / 2.0f });
		}
		else
		{
			resShadow_->Move(this->getPosition(), -dashSpeed);
			runAction(cocos2d::MoveBy::create(0.0f, -dashSpeed));
			colLambda({ dashSpeed.x - charSize_.x, dashSpeed.y + charSize_.y / 2.0f });
		}

		if (animationFrame_ >= lpAnimMng.GetAnimationMaxFrame(ActorType::Player, playerMode_ + currentAnimation_))
		{
			// ダッシュアクション終了
			currentAnimation_ = "Look_Intro";
			bitFlg_.dashFlg   = false;
			animationFrame_   = 0.0f;
			resShadow_->ResShadowEnd();
		}
	}
}

std::string Player::GetAction(void)
{
	return currentAnimation_;
}

void Player::SetAction(std::string action)
{
	currentAnimation_ = action;
}

void Player::SetDir(Direction dir)
{
	direction_ = dir;
}

void Player::KeyInputClear(void)
{
	oprtState_->ResetKey();
}

void Player::AnimRegistrator(void)
{
	// 登録データの初期化
	lpAnimMng.AnimDataClear();	

	lpAnimMng.XmlAnimDataRead("playerAnim_Light", ActorType::Player);
	lpAnimMng.XmlAnimDataRead("playerAnim_Dark" , ActorType::Player);

	// 初期アニメーションの設定
	lpAnimMng.InitAnimation(*this, ActorType::Player, "player_Light_Non");
}

const AttackRect& Player::GetAttackRect(void)
{
	return attackRect_;
}

bool Player::GetGameOverFlg(void)
{
	return bitFlg_.gameOverSceneFlg;
}

int Player::GetGiveDamageNum(void)
{
	// 連撃回数によって火力があがる方式
	float damageUp = 0.0f;
	bool  breakFlg = false;
	// 先に、現在のアニメーションと照らし合わせて、倍率を取得する
	for (auto data : plParam_)
	{
		if (breakFlg)
		{
			break;
		}

		// モード不一致の場合、処理をとばす
		if (data.modeName != playerMode_)
		{
			continue;
		}

		// 倍率を一時変数に保存してfor文を抜ける
		for (auto animName : data.chainAnimName)
		{
			// 現在のアニメーションと不一致の場合、処理をとばす
			if (animName.first != currentAnimation_)
			{
				continue;
			}
			damageUp = animName.second;
			breakFlg = true;
			break;
		}
	}

	// 現在のモードを確認し、上の倍率をもとに最終ダメージを算出する
	int tmpNum = 0;
	for (auto data : plParam_)
	{
		if (data.modeName == playerMode_)
		{
			return static_cast<int>(plParam_[tmpNum].attackDef + (plParam_[tmpNum].attackDef * damageUp));
		}
		tmpNum++;
	}
	return 0;
}

std::string Player::GetPlayerColor(void)
{
	return playerMode_;
}

float Player::GetRunSpeedUp(void)
{
	// 現在のモードから移動速度の加算値を取得する
	int tmpNum = 0;
	for (auto data : plParam_)
	{
		if (data.modeName == playerMode_)
		{
			return plParam_[tmpNum].runSpeedUp;
		}
		tmpNum++;
	}
	return 0.0f;
}

void Player::ActModuleRegistration(void)
{
	Actor::XmlActDataRead("playerModule",oprtState_);
}

void Player::SkillAction(void)
{
	// 何かのスキルがactiveであるときは、ボタンが反応しないようにしている
	const auto director = Director::getInstance();

	if (dynamic_cast<SkillBase*>(director)->GetAllSkillActivate())
	{
		// ここに入ってきたら、active中のスキルがあるということだから、他のスキルが発動不可になる
		return;
	} 

	// スキルの設定
	auto SetSkillLambda = [&](std::string seName) {
		skillSprite_ = dynamic_cast<SkillBase*>(director->getRunningScene()->getChildByTag(static_cast<int>(zOrder::FRONT))->getChildByName("skillSprite"));
		skillSprite_->SetPlayerPos(getPosition());
		skillSprite_->SetPlayerDirection(direction_);
		SkillBase* skill = nullptr;
		if (seName == "burst2")
		{
			skill = SkillA::CreateSkillA(skillSprite_);
		}
		else if (seName == "burst")
		{
			const auto playerPos = getPosition();
			auto minPos = Vec2(0.0f, 0.0f);
			auto minLen = FLT_MAX;			// FLT_MAX：floatの最大の有限値
			for (auto enemy : enemyList_.getChildren())
			{
				const auto enemyPos = enemy->getPosition();
				const auto vecComp  = enemyPos - playerPos;
				const auto length   = sqrt(vecComp.x * vecComp.x + vecComp.y * vecComp.y);
				if (minLen > length)
				{
					minLen = length;
					minPos = enemyPos;
				}
			}
			skillSprite_->SetTargetPos(minPos);
			skill = SkillB::CreateSkillB(skillSprite_);
		}
		else if (seName == "recovery")
		{
			// darkモードでは使用不可にする
			if (playerMode_ == "player_Dark_")
			{
				return;
			}

			// 回復処理
			const auto hpGauge = dynamic_cast<HPGauge*>(director->getRunningScene()->getChildByTag(static_cast<int>(zOrder::FRONT))->getChildByName("PL_HPgauge"));
			hpGauge->SetHP(hpGauge->GetHP() + 50.0f);
			skill = SkillC::CreateSkillC(skillSprite_);
		}
		else
		{
			return;
		}
		skillSprite_->AddActiveSkill(skill);
		skillSprite_->addChild(skill);
		lpSoundMng.PlayBySoundName(seName);	
	};

	// スキル発動処理
	for (int i = static_cast<int>(BUTTON::SkillA); i <= static_cast<int>(BUTTON::SkillC); i++)
	{
		if (oprtState_->GetNowData()[static_cast<int>(i)] && !oprtState_->GetOldData()[static_cast<int>(i)])
		{
			if (dynamic_cast<SkillBase*>(director)->GetSkillCT(SkillTbl_[i - static_cast<int>(BUTTON::SkillA)]))
			{
				SetSkillLambda(SkillSETbl_[i - static_cast<int>(BUTTON::SkillA)]);
			}
			else
			{
				// クールタイム中
			}
		}
	}

	if (skillSprite_ != nullptr)
	{
		skillSprite_->RemoveActiveSkill();
	}
}

bool Player::GameOverAction(void)
{
	if (!bitFlg_.gameOverFlg)
	{
		// 今の動きを止める
		stopAllActions();

		this->setPosition(deathPos_);	// 死亡した位置で設定する
		AnimationCache* animationCache = AnimationCache::getInstance();
		Animation* animation = animationCache->getAnimation(playerMode_ + "Death");
		animation->setRestoreOriginalFrame(false);
		//アニメーションを実行
		gameOverAction_ = runAction(
			Sequence::create(
				Animate::create(animation),
				nullptr
			));
		CC_SAFE_RETAIN(gameOverAction_);
		bitFlg_.gameOverFlg = true;
		return false;
	}
	if (gameOverAction_ == nullptr)
	{
		// アクションが終わっている
		return true;
	}
	if (gameOverAction_->isDone())
	{
		// アクションが終わった
		CC_SAFE_RELEASE_NULL(gameOverAction_);
		return true;
	}
	return false;
}