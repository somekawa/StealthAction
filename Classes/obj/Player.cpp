#include "Scene/GameScene.h"
#include "Player.h"
#include "anim/ActionCtl.h"
#include "ActionRect.h"
#include "_Debug/_DebugConOut.h"
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

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
#include "input/OPRT_key.h"
#else
#include "input/OPRT_touch.h"
#endif

// skillmngのSkillActivateをタッチ操作したときに呼ぶ
// testskillクラス:引数はskillBase_

USING_NS_CC;

int Player::no_ = 0;

Player::Player(int hp,Layer& myLayer, Layer& enemyLayer, SkillBase* skillBasePtr):
	Actor(hp,myLayer),enemyList_(enemyLayer)
{
	skillBase_ = skillBasePtr;
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	lpSoundMng.AddSound("recovery", "BGM/Recovery.mp3", SoundType::SE);
	lpSoundMng.AddSound("burst", "BGM/s-burst01.mp3", SoundType::SE);
	lpSoundMng.AddSound("burst2", "BGM/s-burst02.mp3", SoundType::SE);
	lpSoundMng.AddSound("Transform", "BGM/se_maoudamashii_element_fire01.mp3", SoundType::SE);
	lpSoundMng.AddSound("Knife", "BGM/Knife.mp3", SoundType::SE);

	// キー入力かタッチ操作か判断
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	// thisの意味
	oprtState_ = new OPRT_key(this);
#else
	oprtState_ = new OPRT_touch(this);
#endif

	std::list<std::string> path;
	path.push_back("skill_data");
	//Actor.cppで読み込んだPlayer用のSkillデータの受け取り
	//plfile_ = fileLoad_["Player"];											// この中に今、スキルの情報が読み込まれている(名前とか)

	type_ = ActorType::Player;
	actModuleRegistration();

	currentAnimation_ = "Non";
	actionOld_ = "Run";
	direction_ = Direction::Right;

	AnimRegistrator();

	pos_ = { (int)visibleSize.width / 2 + (int)origin.x - 0,(int)visibleSize.height / 2 + (int)origin.y + 200 };
	setPosition(Vec2(pos_.x, pos_.y));
	myNo_ = no_;
	no_++;
	type_ = ActorType::Player;
	//oldPos_ = 0;

	oldPosKeepFlg_ = false;
	bitFlg_.FirstAttackFlg = false;
	bitFlg_.SecondAttackFlg = false;
	bitFlg_.ThirdAttackFlg = false;
	bitFlg_.TransfromFlg = false;

	gameOverFlg_ = false;
	deathFlg_ = false;
	hp_ = hp;
	for (auto anim : lpAnimMng.GetAnimations(type_))
	{
		// colliderBoxのLoad
		lpCol.ReadData(collider_, anim);
		for (auto col : collider_[anim])
		{
			for (int colNum = 0; colNum < col.size(); colNum++)
			{
				// colliderBoxを自身の子にする
				auto draw = col[colNum]->create();
				draw->setContentSize(Size{ (float)col[colNum]->GetData().size_.x,(float)col[colNum]->GetData().size_.y });
				draw->drawRect(Vec2(0, 0), Vec2{ (float)col[colNum]->GetData().size_.x,(float)col[colNum]->GetData().size_.y }, col[colNum]->GetColor());
				draw->setTag(col[colNum]->GetData().frame_);
				// ここでsetLocalOrderをやんないといけない
				// 攻撃矩形だと0に、ﾀﾞﾒｰｼﾞ矩形だと1に
				// なので、第二引数にcol->GetData().type_みたいなことをやればそれぞれの矩形のZorderになる。
				this->addChild(draw, 0, anim);
			}
		}
	}

	// こう書くとplayerが下の段に移動したときにくっついてくる文字になる
	// updateで見るだけして、1度だけ読み込みさせて文字を描画できるようにしてみるとか
	//auto label = Label::createWithTTF("Text_Yomikomitai", "fonts/Marker Felt.ttf", 24);
	//if (label == nullptr)
	//{
	//	int a = 0;
	//}
	//label->setPosition(Vec2(100,100));
	//this->addChild(label, 1);

	// 攻撃矩形のサイズ設定
	attackRect_.size_ = Size(30.0f, 30.0f);
	attackColOffset_ = 0.0f;


	// 残像
	resShadow_ = std::make_shared<ResidualShadow>();
	resShadow_->CreateResidualShadow(*this, myLayer, 2);
	this->SetIsAttacking(false);
}

Player::~Player()
{
	// 動的なインスタンスをしたらdeleteを忘れないように!
	delete oprtState_;
	delete skillBase_;
}

// 毎フレーム更新
void Player::update(float delta)
{
	// getnameがgamesceneでない場合、何もしないreturn処理
	if (Director::getInstance()->getRunningScene()->getName() != "GameScene")
	{
		return;
	}

	// 死亡状態の更新と確認(deathFlg_がtrueならアップデートをすぐ抜けるようにする)
	if (deathFlg_)
	{
		if (gameOverAction())
		{
			gameOverSceneFlg_ = true;
			return;
		}
		return;
	}

	// バーサーカーモード(攻撃力は上がるが、HPは減少し続ける)
	if (playerColor == "player_Dark_")
	{
		// HP減少処理
		auto nowScene = ((Game*)Director::getInstance()->getRunningScene());
		auto hpGauge = (HPGauge*)nowScene->getChildByTag((int)zOlder::FRONT)->getChildByName("PL_HPgauge");
		hpGauge->SetHP(hpGauge->GetHP() - 0.05f);	// HPが減り続けるように設定
	}

	// キャラクター切替
	if (oprtState_->GetNowData()[static_cast<int>(BUTTON::Transfrom)] && !oprtState_->GetOldData()[static_cast<int>(BUTTON::Transfrom)])
	{
		playerColor == "player_Light_" ? playerColor = "player_Dark_" : playerColor = "player_Light_";
	}

	skillAction();

	// 現在のフレームを整数値で取得
	animationFrame_int_ = GetAnimationFrameInt(playerColor + currentAnimation_);
	//currentCol_ = collider_[currentAnimation_][animationFrame_int_];

	colliderVisible();

	// ActionCtlクラスの更新
	actCtl_.update(type_,delta,*this);

	// 入力情報(keyかtouch)の更新
	oprtState_->update();	
	
	// スライド時のみアンカーポイントを移動させる(その他は画像の下中央)
	if (currentAnimation_ == "Wall_Slide")
	{
		if (direction_ == Direction::Left)
		{
			// アンカーポイント右端
			this->setAnchorPoint(Vec2(1.0f, 0.0f));
		}
		else if (direction_ == Direction::Right)
		{
			// アンカーポイント左端
			this->setAnchorPoint(Vec2(0.0f, 0.0f));
		}
	}
	else
	{
		// アンカーポイント下中央
		this->setAnchorPoint(Vec2(0.5f, 0.0f));
	}

	// スライド終了時に左右向きを反転させる
	if (currentAnimation_ != "Wall_Slide" && actionOld_ == "Wall_Slide")
	{
		auto dir = direction_;
		direction_ == Direction::Right ? direction_ = Direction::Left : direction_ = Direction::Right;
		auto tmpdir = direction_;
	}

	// isHitAttack: 攻撃が当たったらtrueになります。
	// 攻撃が連続で当たっているようになり、HPの減少が激しい。<- 直さないといけない。
	onDamaged_ = isHitAttack_;

	// ダメージをくらった際の処理(この中にdeath処理を書いてしまうと、バーサーカーモードで死亡したときに対応できない)
	if (onDamaged_)
	{
		// HP減少処理
		auto nowScene = ((Game*)Director::getInstance()->getRunningScene());
		auto hpGauge = (HPGauge*)nowScene->getChildByTag((int)zOlder::FRONT)->getChildByName("PL_HPgauge");
		hpGauge->SetHP(hpGauge->GetHP() - 10);	// -10などのダメージ量は敵の攻撃力に変えればいい
		onDamaged_ = false;
		isHitAttack_ = false;
	}

	auto nowScene = ((Game*)Director::getInstance()->getRunningScene());
	auto hpGauge = (HPGauge*)nowScene->getChildByTag((int)zOlder::FRONT)->getChildByName("PL_HPgauge");
	// 0を下回った時にフラグを切り替える
	if (hpGauge->GetHP() <= 0)
	{
		// 死んだﾌﾗｸﾞ(Actorｸﾗｽに宣言してます)
		// 正確には生きているかを確認するﾌﾗｸﾞなので
		// 死んだ: false 生きている:trueとなる
		isAlive_ = false;
		deathFlg_ = true;
		deathPos_ = getPosition();
	}

	attackMotion(delta);
	transformMotion(delta);
	dashMotion(delta);

	// アニメーションが切り替わるタイミングで呼ばれる再生処理
	if (currentAnimation_ != actionOld_)
	{
		if (currentAnimation_ == "AttackFirst" && !bitFlg_.FirstAttackFlg)
		{
			lpSoundMng.PlayBySoundName("Knife");
			bitFlg_.FirstAttackFlg = true;
			// 反転ﾌﾗｸﾞのｾｯﾄ
			bool flip = false;
			direction_ == Direction::Right ? flip = true : flip = false;
			// 攻撃ｴﾌｪｸﾄの追加
			auto attackSprite = lpEffectMng.PickUp("attack", { 55.0f, 50.0f }, getPosition(), {0.0f,0.0f}, 5, 0.04f,flip,false,true);

		}
		animationFrame_int_ = 0.0f;
		lpAnimMng.ChangeAnimation(*this, playerColor + currentAnimation_, true, ActorType::Player);
	}
	actionOld_ = currentAnimation_;
	// 各ｺﾗｲﾀﾞｰのﾎﾟｼﾞｼｮﾝとｻｲｽﾞの設定
	SetCollider();
	//TRACE("playerPos:(%f,%f)", getPosition().x, getPosition().y);
}

// Actorにあるが未使用
void Player::Action(void)
{
	int a = 0;
}

// Actorにあるが未使用
void Player::ChangeDirection(float delta)
{
	int a = 0;
}

void Player::attackMotion(float delta)
{
	auto keyLambda = [&](bool flag) {
		// すでにtrueになってたらtrueで抜ける
		if (flag)
		{
			return true;
		}
		if (oprtState_->GetNowData()[static_cast<int>(BUTTON::DOWN)] && !oprtState_->GetOldData()[static_cast<int>(BUTTON::DOWN)] && !flag)
		{
			return true;
		}
		return false;
	};

	if (bitFlg_.FirstAttackFlg)
	{
		currentAnimation_ = "AttackFirst";
		if (!oldPosKeepFlg_)
		{
			if (playerColor == "player_Dark_")
			{
				this->setPosition(this->getPosition().x, this->getPosition().y - 15);		// 位置補正を入れないと浮いて見える
			}
			//oldPos_ = this->getPosition().x;
			oldPosKeepFlg_ = true;
		}
	}
	else if (bitFlg_.SecondAttackFlg)
	{
		currentAnimation_ = "AttackSecond";
	}
	else if (bitFlg_.ThirdAttackFlg)
	{
		currentAnimation_ = "AttackThird";
	}

	if (currentAnimation_ == "AttackFirst" && bitFlg_.FirstAttackFlg)
	{
		// フレーム数の取得テスト
		//auto a = animationFrame_ * 100;
		//auto b = 0.05 * 100;
		//auto c = (int)a / (int)b;
		//TRACE("%d\n", c);

		bitFlg_.SecondAttackFlg = keyLambda(bitFlg_.SecondAttackFlg);

		// frame計算
		animationFrame_ += delta;
		animationFrame_int_ = (int)(animationFrame_ * 100) / (int)(0.05 * 100);
		if (animationFrame_int_ < 10)
		{
			// 2フレーム目にdataが2つ入り、そのうちの片方がtype:0だから攻撃矩形になってる
			auto col = collider_[playerColor + currentAnimation_];
			currentCol_ = col[animationFrame_int_];
			colliderVisible();
		}

		if (bitFlg_.FirstAttackFlg && animationFrame_ <= 0.5f)
		{
			currentAnimation_ = "AttackFirst";
			isAttacking_ = true;
		}
		else
		{
			if (!bitFlg_.SecondAttackFlg)
			{
				currentAnimation_ = "Look_Intro";
				this->SetIsAttacking(false);

				// darkとlightで画像サイズが違うからその補正
				if (playerColor == "player_Dark_")
				{
					this->setPosition(this->getPosition().x, this->getPosition().y + 15);		// 位置補正を戻す
				}
				else
				{
					//this->setPosition(oldPos_, this->getPosition().y);
					this->setPosition(this->getPosition().x, this->getPosition().y);
				}
			}
			else
			{
				currentAnimation_ = "AttackSecond";
				// 反転ﾌﾗｸﾞのｾｯﾄ
				bool flip = false;
				direction_ == Direction::Right ? flip = true : flip = false;
				// 攻撃ｴﾌｪｸﾄの追加
				auto attackSprite = lpEffectMng.PickUp("attack", { 55.0f, 50.0f }, getPosition(), { 0.0f,0.0f }, 5, 0.04f, flip, false,true);
			}
			animationFrame_ = 0.0f;
			bitFlg_.FirstAttackFlg = false;
			isAttacking_ = false;
			oldPosKeepFlg_ = false;
		}
	}

	if ((currentAnimation_ == "AttackSecond" && bitFlg_.SecondAttackFlg))
	{
		bitFlg_.ThirdAttackFlg = keyLambda(bitFlg_.ThirdAttackFlg);

		// frame計算
		animationFrame_ += delta;
		animationFrame_int_ = (int)(animationFrame_ * 100) / (int)(0.08 * 100);
		if (animationFrame_int_ < 10)
		{
			// 2フレーム目にdataが2つ入り、そのうちの片方がtype:0だから攻撃矩形になってる
			currentCol_ = collider_[playerColor + currentAnimation_][animationFrame_int_];
			colliderVisible();
		}

		if (bitFlg_.SecondAttackFlg && animationFrame_ <= 0.8f)
		{
			currentAnimation_ = "AttackSecond";
			isAttacking_ = true;
		}
		else
		{
			if (!bitFlg_.ThirdAttackFlg)
			{
				currentAnimation_ = "Look_Intro";
				// darkとlightで画像サイズが違うからその補正
				if (playerColor == "player_Dark_")
				{
					this->setPosition(this->getPosition().x, this->getPosition().y + 15);		// 位置補正を戻す
				}
				else
				{
					this->setPosition(this->getPosition().x, this->getPosition().y);
					//this->setPosition(oldPos_, this->getPosition().y);
				}
			}
			else
			{
				currentAnimation_ = "AttackThird";
				// 反転ﾌﾗｸﾞのｾｯﾄ
				bool flip = false;
				direction_ == Direction::Right ? flip = true : flip = false;
				// 攻撃ｴﾌｪｸﾄの追加
				auto attackSprite = lpEffectMng.PickUp("attack", { 55.0f, 50.0f }, getPosition(), { 0.0f,0.0f }, 5, 0.04f, flip, false,true);
			}
			bitFlg_.SecondAttackFlg = false;
			isAttacking_ = false;
			animationFrame_ = 0.0f;
			//oldPosKeepFlg_ = false;
		}
	}

	if ((currentAnimation_ == "AttackThird" && bitFlg_.ThirdAttackFlg))
	{
		// frame計算
		animationFrame_ += delta;
		animationFrame_int_ = (int)(animationFrame_ * 100) / (int)(0.08 * 100);
		if (animationFrame_int_ < 11)
		{
			// 2フレーム目にdataが2つ入り、そのうちの片方がtype:0だから攻撃矩形になってる
			currentCol_ = collider_[playerColor + currentAnimation_][animationFrame_int_];
			colliderVisible();
		}

		if (bitFlg_.ThirdAttackFlg && animationFrame_ <= 0.8f)
		{
			currentAnimation_ = "AttackThird";
			isAttacking_ = true;
		}
		else
		{
			bitFlg_.ThirdAttackFlg = false;
			isAttacking_ = false;
			currentAnimation_ = "Look_Intro";
			// darkとlightで画像サイズが違うからその補正
			if (playerColor == "player_Dark_")
			{
				this->setPosition(this->getPosition().x, this->getPosition().y + 15);		// 位置補正を戻す
			}
			else
			{
				this->setPosition(this->getPosition().x, this->getPosition().y);
				//this->setPosition(oldPos_, this->getPosition().y);
			}
			animationFrame_ = 0.0f;
			//oldPosKeepFlg_ = false;
		}
	}
}

void Player::transformMotion(float delta)
{
	// トランスフォーム
	if (bitFlg_.TransfromFlg)
	{
		lpSoundMng.PlayBySoundName("Transform",0.2f);
		currentAnimation_ = "Transform";
	}
	if (!bitFlg_.TransfromFlg)
	{
		if (oprtState_->GetNowData()[static_cast<int>(BUTTON::Transfrom)] && !oprtState_->GetOldData()[static_cast<int>(BUTTON::Transfrom)])
		{
			this->setPosition(this->getPosition().x, this->getPosition().y - 10);		// 位置補正を入れないと浮いて見える
			currentAnimation_ = "Transform";
			bitFlg_.TransfromFlg = true;
		}
	}
	if (bitFlg_.TransfromFlg)
	{
		animationFrame_ += delta;
		if (animationFrame_ >= 1.85f)
		{
			bitFlg_.TransfromFlg = false;
			currentAnimation_ = "Look_Intro";
			animationFrame_ = 0.0f;
			this->setPosition(this->getPosition().x, this->getPosition().y + 10);	// 位置補正分戻す
		}
		else
		{
			currentAnimation_ = "Transform";
		}
	}
}

void Player::dashMotion(float delta)
{
	// これで回避の式はできてる
	//if (testnum >= -4.0f && testnum <= 4.0f)
	//{
	//	// 0のときが最大値(1.0)になる
	//	auto move = exp(-pow(testnum, 2));
	//	TRACE("num:%f,pl_move:%f\n", testnum,move);
	//	testnum+=0.1f;
	//}
	//else
	//{
	//	int stop = 0;
	//}
	// 16のときが最大値(1.0)になる?→ならない。0が1.0で最大値になる仕組み
	// 上の式をdashのアニメーションframeで計算した場合の書き方テスト
	//if (testnum >= 0.00f && testnum <= 0.32f)
	//{
	//	auto tmpnum = testnum;
	//	tmpnum *= 100.0f;
	//	tmpnum -= 16.0f;
	//	tmpnum /= 4.0f;
	//	auto move = exp(-pow(tmpnum, 2));
	//	TRACE("num:%f,pl_move:%f\n", tmpnum, move);
	//	testnum+=delta;
	//}
	//else
	//{
	//	int stop = 0;
	//}

	// 当たり判定
	auto lambda = [&](Vec2 move) {
		auto director = Director::getInstance();
		auto plPos = this->getPosition();
		auto CollisionData = (TMXLayer*)director->getRunningScene()->getChildByTag((int)zOlder::BG)->getChildByName("MapData")->getChildByName("col");
		auto& ColSize = CollisionData->getLayerSize();
		const int chipSize = CollisionData->getMapTileSize().width;
		auto plCheckPoint1 = plPos + move;
		auto plCheckPoint1Chip = Vec2{ plCheckPoint1 } / chipSize;
		auto plCheckPoint1Pos = Vec2(plCheckPoint1Chip.x, ColSize.height - plCheckPoint1Chip.y);
		auto plCheckPoint1Gid = CollisionData->getTileGIDAt(plCheckPoint1Pos);
		if (plCheckPoint1Gid != 0)
		{
			return false;
		}
		return true;
	};

	if (bitFlg_.DashFlg)
	{
		resShadow_->Start(*this);
		currentAnimation_ = "Dash";
	}
	if (!bitFlg_.DashFlg)
	{
		if (oprtState_->GetNowData()[static_cast<int>(BUTTON::Dash)] && !oprtState_->GetOldData()[static_cast<int>(BUTTON::Dash)])
		{
			//this->setPosition(this->getPosition().x + 100, this->getPosition().y);		
			currentAnimation_ = "Dash";
			bitFlg_.DashFlg = true;
		}
	}
	if (bitFlg_.DashFlg)
	{
		//y = exp(-pow(x,2))
		// pow…2乗とかできるやつ
		// exp…ネイピア数
		float tmpnum = animationFrame_;
		tmpnum = ((tmpnum * 100.0f) - 16.0f) / 4.0f;
		//tmpnum *= 100.0f;
		//tmpnum -= 16.0f;
		//tmpnum /= 4.0f;
		auto move = exp(-pow(tmpnum, 2));
		Vec2 dashSpeed;
		if (direction_ == Direction::Right)
		{
			dashSpeed = Vec2(move * 30, 0);

			resShadow_->Move(this->getPosition(), dashSpeed);
			// 等速移動(比較できるようにコメントアウトで置いてるやつ)
			//runAction(cocos2d::MoveBy::create(0.0f, cocos2d::Vec2(0.3 * 30, 0)));
			runAction(cocos2d::MoveBy::create(0.0f, dashSpeed));

			Vec2 charSize = { 15.0f * 3.0f,25.0f * 3.0f };
			if (!lambda(Vec2(move * 30 + charSize.x / 2, 0 + charSize.y / 2)))
			{
				resShadow_->ResShadowEnd();
				TRACE("move終了\n");
				bitFlg_.DashFlg = false;
				currentAnimation_ = "Look_Intro";
				animationFrame_ = 0.0f;
			}
		}
		else
		{
			dashSpeed = Vec2(move * -30, 0);

			resShadow_->Move(this->getPosition(), dashSpeed);
			runAction(cocos2d::MoveBy::create(0.0f, dashSpeed));
			Vec2 charSize = { 15.0f * 3.0f,25.0f * 3.0f };
			if (!lambda(Vec2(move * 30 - charSize.x / 2, 0 + charSize.y / 2)))
			{

				resShadow_->ResShadowEnd();
				TRACE("move終了\n");
				bitFlg_.DashFlg = false;
				currentAnimation_ = "Look_Intro";
				animationFrame_ = 0.0f;
			}
		}
		TRACE("num:%f,pl_move:%f\n", tmpnum, move * 50);

		animationFrame_ += delta;
		//runAction(cocos2d::MoveBy::create(0.0f, cocos2d::Vec2(move, 0)));
		if (animationFrame_ >= 0.32f)
		{

			resShadow_->ResShadowEnd();
			bitFlg_.DashFlg = false;
			currentAnimation_ = "Look_Intro";
			animationFrame_ = 0.0f;
		}
		else
		{
			currentAnimation_ = "Dash";
		}
	}
}

void Player::colliderVisible(void)
{
	// トランスフォームの時はコライダーを無視する
	if (currentAnimation_ == "Transform" || currentAnimation_ == "Death")
	{
		return;
	}

	if (animationFrame_int_ <= 0)
	{
		animationFrame_int_ = 0;
	}
	//CCLOG("plHP:%d", hp_);
	if (collider_[playerColor + currentAnimation_].size() <= animationFrame_int_)
	{
		return;
	}
	currentCol_ = collider_[playerColor + currentAnimation_][animationFrame_int_];
	for (auto collider : this->getChildren())
	{
		if (playerColor + currentAnimation_ == collider->getName() &&
			animationFrame_int_ == collider->getTag())
		{
			// 攻撃の時だけオフセットが必要
			attackCollider("AttackFirst", collider, attackColOffset_);
			attackCollider("AttackSecond", collider, attackColOffset_);
			attackCollider("AttackThird", collider, attackColOffset_);

			//if (currentAnimation_ == "AttackFirst")
			//{
			//	if (direction_ == Direction::Right)
			//	{
			//		if (!attackF_offsetFlg_)
			//		{
			//			attackColOffset_ = collider->getPosition().x + 30.0f;
			//			attackF_offsetFlg_ = true;
			//		}
			//	}
			//	else
			//	{
			//		attackColOffset_ = 0.0f;
			//		attackF_offsetFlg_ = false;
			//	}
			//	collider->setPosition(attackColOffset_,collider->getPosition().y);
			//}
			//if (currentAnimation_ == "AttackSecond")
			//{
			//	if (direction_ == Direction::Right)
			//	{
			//		if (!attackS_offsetFlg_)
			//		{
			//			attackS_offsetPos_ = collider->getPosition().x + 30.0f;
			//			attackS_offsetFlg_ = true;
			//		}
			//	}
			//	else
			//	{
			//		attackS_offsetPos_ = 0.0f;
			//		attackS_offsetFlg_ = false;
			//	}
			//	collider->setPosition(attackS_offsetPos_, collider->getPosition().y);
			//}
			//if (currentAnimation_ == "AttackThird")
			//{
			//	if (direction_ == Direction::Right)
			//	{
			//		if (!attackT_offsetFlg_)
			//		{
			//			attackT_offsetPos_ = collider->getPosition().x + 30.0f;
			//			attackT_offsetFlg_ = true;
			//		}
			//	}
			//	else
			//	{
			//		attackT_offsetPos_ = 0.0f;
			//		attackT_offsetFlg_ = false;
			//	}
			//	collider->setPosition(attackT_offsetPos_, collider->getPosition().y);
			//}

			//collider->setVisible(true);
		}
		else
		{
			collider->setVisible(false);
		}
	}
}

void Player::attackCollider(std::string str,cocos2d::Node* col,float& pos)
{
	if (!oldPosKeepFlg_)
	{
		return;
	}

	// 攻撃の時だけオフセットが必要
	if (currentAnimation_ == str.c_str())
	{
		if (!this->IsAttacking())	// 2・3撃目はずっとattackingがtrueだからコライダー位置の設定のしなおしにならない
		{
			if (direction_ == Direction::Right)
			{
				this->SetAttackOffset(Vec2(col->getPosition().x + 30.0f, col->getPosition().y));
				this->runAction(cocos2d::MoveTo::create(0.0f, cocos2d::Vec2(this->getPosition().x + AttackMove, this->getPosition().y)));
				//this->runAction(cocos2d::MoveTo::create(0.0f, cocos2d::Vec2(oldPos_ + AttackMove, this->getPosition().y)));
				this->SetIsAttacking(true);
			}
			else
			{
				this->SetAttackOffset(cocos2d::Vec2(0.0f, 0.0f));
				this->runAction(cocos2d::MoveTo::create(0.0f, cocos2d::Vec2(this->getPosition().x - AttackMove, this->getPosition().y)));
				//this->runAction(cocos2d::MoveTo::create(0.0f, cocos2d::Vec2(oldPos_ - AttackMove, this->getPosition().y)));
				this->SetIsAttacking(true);
			}

			//for (auto col : this->getChildren())
			//{
			//	// 矩形の名前がattackならば
			//	if (col->getName() == this->GetAction())
			//	{
			//		// 矩形のﾀｲﾌﾟがattackのやつのみの抽出
			//		if (col->getLocalZOrder() == 0)
			//		{
			//			// 矩形のﾎﾟｼﾞｼｮﾝの変更
			//			// 矩形のﾎﾟｼﾞｼｮﾝは1度のみの変更でないと
			//			// 毎ﾌﾚｰﾑ回ってしまうと矩形がどっかいくのでここでｾｯﾄしている。
			//			// 矩形がずれていくが、当たり判定は本来の位置で行われている
			//			col->setPosition(this->GetAttackRect().offset_.x, col->getPosition().y);
			//		}
			//	}
			//}
		}
		col->setPosition(this->GetAttackRect().offset_.x, col->getPosition().y);
	}
}

// 現在のアクション情報を取得する
std::string Player::GetAction(void)
{
	return currentAnimation_;
}

// 現在のアクション状態をセットする
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
	oprtState_->KeyReset();
}

void Player::AnimRegistrator(void)
{
	lpAnimMng.AnimDataClear();	// 登録データの初期化

	std::string path_light = "image/PlayerAnimationAsset/player/Light/player_Light";
	std::string path_dark = "image/PlayerAnimationAsset/player/Dark/player_Dark";

	// アニメーションをキャッシュに登録
	// non
	lpAnimMng.addAnimationCache(path_light, "Non", 6, (float)0.3, ActorType::Player,false);

	// idle
	lpAnimMng.addAnimationCache(path_light, "Look_Intro", 6, (float)0.3, ActorType::Player, false);

	// run
	lpAnimMng.addAnimationCache(path_light, "Run", 9, (float)0.08, ActorType::Player, false);

	// fall
	lpAnimMng.addAnimationCache(path_light, "Fall", 3, (float)1.0, ActorType::Player, false);

	// jump
	lpAnimMng.addAnimationCache(path_light, "Jump", 3, (float)0.05, ActorType::Player, false);

	lpAnimMng.addAnimationCache(path_light, "Jumping", 3, (float)0.05, ActorType::Player, false);

	// AttackFirst
	lpAnimMng.addAnimationCache(path_light, "AttackFirst", 9, (float)0.05, ActorType::Player, false);

	// AttackSecond
	lpAnimMng.addAnimationCache(path_light, "AttackSecond", 9, (float)0.08, ActorType::Player, false);

	// AttackThird
	lpAnimMng.addAnimationCache(path_light, "AttackThird", 10, (float)0.08, ActorType::Player, false);

	// wallslide
	lpAnimMng.addAnimationCache(path_light, "Wall_Slide", 3, (float)0.3, ActorType::Player, false);

	// Transform
	lpAnimMng.addAnimationCache(path_light, "Transform", 37, (float)0.05, ActorType::Player, false);

	// death
	lpAnimMng.addAnimationCache(path_light, "Death", 10, (float)0.08, ActorType::Player, false);

	// dash(回避)
	lpAnimMng.addAnimationCache(path_light, "Dash", 4, (float)0.08, ActorType::Player, false);

	// non
	lpAnimMng.addAnimationCache(path_dark, "Non", 6, (float)0.3, ActorType::Player, false);

	// idle
	lpAnimMng.addAnimationCache(path_dark, "Look_Intro", 6, (float)0.3, ActorType::Player, false);

	// run
	lpAnimMng.addAnimationCache(path_dark, "Run", 9, (float)0.08, ActorType::Player, false);

	// fall
	lpAnimMng.addAnimationCache(path_dark, "Fall", 3, (float)1.0, ActorType::Player, false);

	// jump
	lpAnimMng.addAnimationCache(path_dark, "Jump", 3, (float)0.05, ActorType::Player, false);

	lpAnimMng.addAnimationCache(path_dark, "Jumping", 3, (float)0.05, ActorType::Player, false);

	// AttackFirst
	lpAnimMng.addAnimationCache(path_dark, "AttackFirst", 9, (float)0.05, ActorType::Player, false);

	// AttackSecond
	lpAnimMng.addAnimationCache(path_dark, "AttackSecond", 9, (float)0.08, ActorType::Player, false);

	// AttackThird
	lpAnimMng.addAnimationCache(path_dark, "AttackThird", 10, (float)0.08, ActorType::Player, false);

	// wallslide
	lpAnimMng.addAnimationCache(path_dark, "Wall_Slide", 3, (float)0.3, ActorType::Player, false);

	// Transform
	lpAnimMng.addAnimationCache(path_dark, "Transform", 37, (float)0.05, ActorType::Player, false);

	// death
	lpAnimMng.addAnimationCache(path_dark, "Death", 12, (float)0.08, ActorType::Player, false);

	// dash(回避)
	lpAnimMng.addAnimationCache(path_dark, "Dash", 4, (float)0.08, ActorType::Player, false);

	lpAnimMng.InitAnimation(*this, ActorType::Player, "player_Light_Non");
}

const AttackRect& Player::GetAttackRect(void)
{
	return attackRect_;
}

Player* Player::CreatePlayer(int hp,Layer& myLayer, Layer& enemyLayer, SkillBase* skillBasePtr)
{
	Player* pRet = new(std::nothrow) Player(hp,myLayer,enemyLayer,skillBasePtr);
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

bool Player::GetGameOverFlg(void)
{
	return gameOverSceneFlg_;
}


void Player::actModuleRegistration(void)
{
	Vec2 charSize = { 15.0f * 3.0f,25.0f * 3.0f };
	// 右移動
	{
		ActModule act;
		act.state = oprtState_;
		act.vel = Vec2{ 5,0 };
		act.actName = "Run";
		act.button = BUTTON::RIGHT;
		act.checkPoint1 = Vec2{ charSize.x/2, charSize.y/2 };	// 右上
		act.checkPoint2 = Vec2{ charSize.x/2,  15 };			// 右下
		act.touch = TOUCH_TIMMING::TOUCHING;	// 押しっぱなし
		//act.blackList.emplace_back(ACTION::FALLING);	// 落下中に右移動してほしくないときの追加の仕方

		//act.whiteList.emplace_back(ACTION::JUMPING);
		act.blackList.emplace_back("AttackFirst");
		act.blackList.emplace_back("AttackSecond");
		act.blackList.emplace_back("AttackThird");
		act.blackList.emplace_back("Wall_Slide");
		actCtl_.RunInitializeActCtl(type_,"右移動", act);

		auto anchor1 = DrawNode::create();
		anchor1->drawDot(act.checkPoint1, 3.0f, Color4F::BLUE);
		this->addChild(anchor1, 5);
		auto anchor2 = DrawNode::create();
		anchor2->drawDot(act.checkPoint2, 3.0f, Color4F::BLUE);
		this->addChild(anchor2, 5);
	}

	// 左移動
	{
		ActModule act;
		act.state = oprtState_;
		act.vel = Vec2{ -5,0 };
		act.actName = "Run";
		act.button = BUTTON::LEFT;
		act.checkPoint1 = Vec2{ -charSize.x/2, charSize.y/2 };	// 左上
		act.checkPoint2 = Vec2{ -charSize.x/2,  15 };			// 左下
		act.touch = TOUCH_TIMMING::TOUCHING;    // 押しっぱなし

		//act.blackList.emplace_back(ACTION::FALLING);

		//act.whiteList.emplace_back(ACTION::JUMPING);
		act.blackList.emplace_back("AttackFirst");
		act.blackList.emplace_back("AttackSecond");
		act.blackList.emplace_back("AttackThird");
		act.blackList.emplace_back("Wall_Slide");
		actCtl_.RunInitializeActCtl(type_,"左移動", act);
		auto anchor3 = DrawNode::create();
		anchor3->drawDot(act.checkPoint1, 3.0f, Color4F::GREEN);
		this->addChild(anchor3, 5);
		auto anchor4 = DrawNode::create();
		anchor4->drawDot(act.checkPoint2, 3.0f, Color4F::GREEN);
		this->addChild(anchor4, 5);
	}

	// 右向き反転
	{
		ActModule flipAct;
		flipAct.state = oprtState_;
		flipAct.flipFlg = false;
		flipAct.actName = "Non";
		flipAct.button = BUTTON::RIGHT;
		flipAct.touch = TOUCH_TIMMING::TOUCHING; // 押しっぱなし

		//flipAct.blackList.emplace_back(ACTION::FALLING);

		flipAct.blackList.emplace_back("AttackFirst");
		flipAct.blackList.emplace_back("AttackSecond");
		flipAct.blackList.emplace_back("AttackThird");
		flipAct.blackList.emplace_back("Wall_Slide");
		actCtl_.RunInitializeActCtl(type_,"右向き", flipAct);
	}

	// 左向き反転
	{
		ActModule flipAct;
		flipAct.state = oprtState_;
		flipAct.flipFlg = true;
		flipAct.actName = "Non";
		flipAct.button = BUTTON::LEFT;
		flipAct.touch = TOUCH_TIMMING::TOUCHING; // 押しっぱなし

		//flipAct.blackList.emplace_back(ACTION::FALLING);

		flipAct.blackList.emplace_back("AttackFirst");
		flipAct.blackList.emplace_back("AttackSecond");
		flipAct.blackList.emplace_back("AttackThird");
		flipAct.blackList.emplace_back("Wall_Slide");
		actCtl_.RunInitializeActCtl(type_,"左向き", flipAct);
	}

	// 落下
	{
		// checkkeylistに離している間の設定もしたけど特に効果なし
		ActModule act;
		act.actName = "Fall";
		act.state = oprtState_;
		act.button = BUTTON::DOWN;
		//act.checkPoint1 = Vec2{ 0,-10 };			// 左下
		//act.checkPoint2 = Vec2{ 0,-10 };			// 右下
		act.checkPoint1 = Vec2{ 0,0 };				// 左下
		act.checkPoint2 = Vec2{ 0,0 };				// 右下

		act.checkPoint3 = Vec2{ charSize.x / 2, charSize.y / 2 };  // 右上
		act.checkPoint4 = Vec2{ -charSize.x / 2, charSize.y / 2 }; // 左上

		act.gravity = Vec2{ 0.0f,-5.0f };
		act.touch = TOUCH_TIMMING::RELEASED;	// ずっと離している
		act.blackList.emplace_back("Jumping");	// ジャンプ中に落下してほしくない
		act.blackList.emplace_back("Wall_Slide");	
		//act.blackList.emplace_back(ACTION::JUMP);	// ジャンプ中に落下してほしくない

		actCtl_.RunInitializeActCtl(type_,"落下", act);
	}

	// ジャンプ
	{
		ActModule act;
		act.state = oprtState_;
		act.button = BUTTON::UP;
		act.actName = "Jump";
		act.checkPoint1 = Vec2{ -charSize.x/3 + 5, charSize.y };		// 左上
		act.checkPoint2 = Vec2{ charSize.x/3 - 5, charSize.y };			// 右上
		//act.checkPoint1 = Vec2{ -10, 30 };						// 左上
		//act.checkPoint2 = Vec2{ +10, 30 };						// 右上
		act.jumpVel = Vec2{ 0.0f,20.0f };
		act.touch = TOUCH_TIMMING::ON_TOUCH;	// 押した瞬間

		// これをコメントアウトしていると、左右押しながらのジャンプができる
		// でも連続でジャンプして上昇し続けるようになる
		// しかもFALLとJUMPが混ざって高さが出ない
		act.blackList.emplace_back("Fall");	// 落下中にジャンプしてほしくない
		act.blackList.emplace_back("AttackFirst");
		act.blackList.emplace_back("AttackSecond");
		act.blackList.emplace_back("AttackThird");
		//act.whiteList.emplace_back(ACTION::RUN);

		actCtl_.RunInitializeActCtl(type_,"ジャンプ", act);
	}

	// ジャンピング
	{
		ActModule act;
		act.state = oprtState_;
		act.button = BUTTON::UP;
		act.actName = "Jumping";
		act.checkPoint1 = Vec2{ -charSize.x/3 + 5, charSize.y };	// 左上
		act.checkPoint2 = Vec2{ charSize.x/3 - 5, charSize.y };		// 右上
		//act.checkPoint1 = Vec2{ -10, 30 };					// 左上
		//act.checkPoint2 = Vec2{ +10, 30 };					// 右上
		act.jumpVel = Vec2{ 0.0f,20.0f };
		act.touch = TOUCH_TIMMING::TOUCHING;	// 押しっぱなし

		act.blackList.emplace_back("Fall");	// 落下中にジャンプしてほしくない
		act.blackList.emplace_back("Look_Intro");
		act.blackList.emplace_back("Run");
		act.blackList.emplace_back("AttackFirst");
		act.blackList.emplace_back("AttackSecond");
		act.blackList.emplace_back("AttackThird");
		act.blackList.emplace_back("Non");
		act.blackList.emplace_back("Wall_Slide");
		act.blackList.emplace_back("Transform");
		act.blackList.emplace_back("Dash");

		act.whiteList.emplace_back("Jump");
		actCtl_.RunInitializeActCtl(type_,"ジャンピング", act);
		//actCtl_.ActCtl("ジャンプ", act);
	}

	// 攻撃
	{
		ActModule act;
		act.state = oprtState_;
		//act.button = BUTTON::ATTACK;
		act.button = BUTTON::DOWN;
		act.actName = "AttackFirst";
		//act.checkPoint1 = Vec2{ 0, 0 };		
		//act.checkPoint2 = Vec2{ 0, 0 };
		//act.blackList.emplace_back("Fall");	// 一時的コメントアウト
		act.touch = TOUCH_TIMMING::ON_TOUCH;	// 押した瞬間
		actCtl_.RunInitializeActCtl(type_,"攻撃", act);
	}

	// 攻撃2
	{
		ActModule act;
		act.state = oprtState_;
		act.button = BUTTON::DOWN;
		act.actName = "AttackSecond";
		act.blackList.emplace_back("Fall");
		act.touch = TOUCH_TIMMING::ON_TOUCH;	// 押した瞬間
		actCtl_.RunInitializeActCtl(type_,"攻撃", act);
	}

	// 攻撃3
	{
		ActModule act;
		act.state = oprtState_;
		act.button = BUTTON::DOWN;
		act.actName = "AttackThird";
		act.blackList.emplace_back("Fall");
		act.touch = TOUCH_TIMMING::ON_TOUCH;	// 押した瞬間
		actCtl_.RunInitializeActCtl(type_,"攻撃", act);
	}

	// 右壁スライド
	{
		ActModule act;
		act.state = oprtState_;
		act.gravity = Vec2{ 0.0f,-1.0f };
		act.checkPoint1 = Vec2{ charSize.x / 2, charSize.y / 2 };	// 右上
		act.checkPoint2 = Vec2{ charSize.x / 2, 0 };				// 右下
		act.button = BUTTON::RIGHT;
		act.flipFlg = true;
		act.actName = "Wall_Slide";
		act.touch = TOUCH_TIMMING::TOUCHING;	
		act.blackList.emplace_back("Jumping");	
		act.blackList.emplace_back("Run");	
		act.blackList.emplace_back("Fall");
		act.blackList.emplace_back("Intro");
		act.blackList.emplace_back("Non");
		act.blackList.emplace_back("AttackFirst");
		act.blackList.emplace_back("AttackSecond");
		act.blackList.emplace_back("AttackThird");
		actCtl_.RunInitializeActCtl(type_,"右壁スライド", act);
	}

	// 左壁スライド
	{
		ActModule act;
		act.state = oprtState_;
		act.gravity = Vec2{ 0.0f,-1.0f };
		act.checkPoint1 = Vec2{-charSize.x / 2, charSize.y / 2 };	// 左上
		act.checkPoint2 = Vec2{-charSize.x / 2, 0 };				// 左下
		act.button = BUTTON::LEFT;
		act.flipFlg = false;
		act.actName = "Wall_Slide";
		act.touch = TOUCH_TIMMING::TOUCHING;
		act.blackList.emplace_back("Jumping");
		act.blackList.emplace_back("Run");
		act.blackList.emplace_back("Fall");
		act.blackList.emplace_back("Intro");
		act.blackList.emplace_back("Non");
		act.blackList.emplace_back("AttackFirst");
		act.blackList.emplace_back("AttackSecond");
		act.blackList.emplace_back("AttackThird");
		actCtl_.RunInitializeActCtl(type_,"左壁スライド", act);
	}

	// 更新関数の登録
	actCtl_.InitUpdater(type_);
}

void Player::skillAction(void)
{
	// 何かのスキルがactiveであるときは、ボタンが反応しないようにする
	auto director = Director::getInstance();
	auto list = ((SkillBase*)director)->GetSkillList();
	bool flag = ((SkillBase*)director)->GetAllSkillActivate();

	if (!flag)
	{
		// なんかとぶやつ
		if (oprtState_->GetNowData()[static_cast<int>(BUTTON::SkillA)] && !oprtState_->GetOldData()[static_cast<int>(BUTTON::SkillA)])
		{
			bool skillFlg = ((SkillBase*)director)->GetSkillCT("magic");
			if (skillFlg)
			{
				lpSoundMng.PlayBySoundName("burst2");
				auto director = Director::getInstance();
				skillSprite = (SkillBase*)director->getRunningScene()->getChildByTag((int)zOlder::FRONT)->getChildByName("skillSprite");		//FLT_MAX : float の最大の有限値
				skillSprite->SetPlayerPos(getPosition());
				skillSprite->SetPlayerDirection(direction_);
				SkillBase* skill_t = SkillA::CreateSkillA(skillSprite);
				skillSprite->AddActiveSkill(skill_t);
				skillSprite->addChild(skill_t);
			}
			else
			{
				// クールタイム中
			}
			//lpSkillMng.SkillActivate(plfile_[0]);
		}

		// 炎(SkillBにする予定のやつ)
		if (oprtState_->GetNowData()[static_cast<int>(BUTTON::SkillB)] && !oprtState_->GetOldData()[static_cast<int>(BUTTON::SkillB)])
		{
			bool skillFlg = ((SkillBase*)director)->GetSkillCT("enemySpawn");
			if (skillFlg)
			{
				lpSoundMng.PlayBySoundName("burst");
				auto director = Director::getInstance();
				skillSprite = (SkillBase*)director->getRunningScene()->getChildByTag((int)zOlder::FRONT)->getChildByName("skillSprite");		//FLT_MAX : float の最大の有限値
				auto ppos = getPosition();
				auto minpos = Vec2(0, 0);
				auto minlen = FLT_MAX;
				for (auto enemy : enemyList_.getChildren())
				{
					auto epos = enemy->getPosition();
					auto veccomp = epos - ppos;
					auto length = sqrt(veccomp.x * veccomp.x + veccomp.y * veccomp.y);
					if (minlen > length)
					{
						minlen = length;
						minpos = epos;
					}
				}
				skillSprite->SetTargetPos(minpos);
				skillSprite->SetPlayerPos(getPosition());
				skillSprite->SetPlayerDirection(direction_);
				SkillBase* skill_t = SkillB::CreateSkillB(skillSprite);
				skillSprite->AddActiveSkill(skill_t);
				skillSprite->addChild(skill_t);
			}
			else
			{
				// クールタイム中
			}

			//lpSkillMng.SkillActivate(plfile_[1]);
		}

		// HP回復
		if (oprtState_->GetNowData()[static_cast<int>(BUTTON::SkillC)] && !oprtState_->GetOldData()[static_cast<int>(BUTTON::SkillC)])
		{
			bool skillFlg = ((SkillBase*)director)->GetSkillCT("heal");
			if (skillFlg)
			{
				lpSoundMng.PlayBySoundName("recovery");
				auto director = Director::getInstance();
				skillSprite = (SkillBase*)director->getRunningScene()->getChildByTag((int)zOlder::FRONT)->getChildByName("skillSprite");		//FLT_MAX : float の最大の有限値
				skillSprite->SetPlayerPos(getPosition());
				SkillBase* skill_t = SkillC::CreateSkillC(skillSprite);
				skillSprite->AddActiveSkill(skill_t);
				skillSprite->addChild(skill_t);

				// 回復処理
				auto nowScene = ((Game*)Director::getInstance()->getRunningScene());
				auto hpGauge = (HPGauge*)nowScene->getChildByTag((int)zOlder::FRONT)->getChildByName("PL_HPgauge");
				hpGauge->SetHP(hpGauge->GetHP() + 50);	// とりあえず半回復
			}
			else
			{
				// クールタイム中
			}

			//lpSkillMng.SkillActivate(plfile_[2]);
		}

		if (skillSprite != nullptr)
		{
			skillSprite->RemoveActiveSkill();
		}
	}
	else
	{
		// ここに入ってきたら、active中のスキルがあるということだから、他のスキルが発動不可になる
	}
}

bool Player::gameOverAction(void)
{
	if (!gameOverFlg_)
	{
		// 今の動きを止める
		stopAllActions();

		this->setPosition(deathPos_);	// 死亡した位置で設定しておかないと、おかしくなる
		AnimationCache* animationCache = AnimationCache::getInstance();
		Animation* animation = animationCache->getAnimation(playerColor + "Death");
		animation->setRestoreOriginalFrame(false);
		//アニメーションを実行
		gameOverAction_ = runAction(
			Sequence::create(
				Animate::create(animation),
				nullptr
			));
		CC_SAFE_RETAIN(gameOverAction_);
		gameOverFlg_ = true;
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