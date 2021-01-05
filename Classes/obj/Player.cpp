#include "Scene/GameScene.h"
#include "PL_HPgauge.h"
#include "Player.h"
#include "anim/ActionCtl.h"
#include "ActionRect.h"
#include "_Debug/_DebugConOut.h"
#include "Loader/CollisionLoader.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
#include "input/OPRT_key.h"
#else
#include "input/OPRT_touch.h"
#endif

USING_NS_CC;

int Player::no_ = 0;

Player::Player(int hp,Layer& myLayer):
	Actor(hp,myLayer)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	// this->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	// 上のやつを短くしたversion↓
	//setPosition(Vec2{ Vec2(visibleSize) / 2 + origin - Vec2(0,-200) });

	// キー入力かタッチ操作か判断
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	// thisの意味
	_oprtState = new OPRT_key(this);
#else
	_oprtState = new OPRT_touch(this);
#endif

	//layer_[static_cast<int>(zOlder::CHAR_PL)]->getChildByName("player1")->setScale(3.0f);
	//layer_[static_cast<int>(zOlder::CHAR_PL)]->getChildByName("player1")->setAnchorPoint(Vec2(0.5f, 0.0f));


	actModuleRegistration();

	currentAnimation_ = "player_Non";
	actionOld_ = "player_Run";
	direction_ = Direction::Right;

	// アニメーションの登録
	AnimRegistrator();

	pos_ = { (int)visibleSize.width / 2 + (int)origin.x - 0,(int)visibleSize.height / 2 + (int)origin.y + 200 };
	setPosition(Vec2(pos_.x, pos_.y));
	myNo_ = no_;
	no_++;
	type_ = ActorType::Player;
	oldPos_ = 0;

	oldPosKeepFlg_ = false;
	bitFlg_.FirstAttackFlg = false;
	bitFlg_.SecondAttackFlg = false;
	bitFlg_.ThirdAttackFlg = false;
	bitFlg_.TransfromFlg = false;

	for (auto anim : lpAnimMng.GetAnimations(type_))
	{
		// colliderBoxのLoad
		lpCol.Load(collider_, anim);
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

	this->SetIsAttacking(false);
}

Player::~Player()
{
	// 動的なインスタンスをしたらdeleteを忘れないように!
	delete _oprtState;
}

// 毎フレーム更新
void Player::update(float delta)
{
	// getnameがgamesceneでない場合、何もしないreturn処理
	if (Director::getInstance()->getRunningScene()->getName() != "GameScene")
	{
		return;
	}

	// 死亡状態の更新と確認(gameOverActionがtrueになったらアップデートをすぐ抜けるようにする)
	if (deathFlg_)
	{
		if (gameOverAction())
		{
			return;
		}
	}

	// 現在のフレームを整数値で取得
	animationFrame_int_ = GetAnimationFrameInt();
	colliderVisible();

	Action();

	_actCtl.update(delta,*this);

	// 実行する入力先(keyかtouch)のupdateへ移動
	_oprtState->update();	
	
	if (currentAnimation_ == "player_Wall_Slide")
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
		this->setAnchorPoint(Vec2(0.5f, 0.0f));
	}

	// スライド終了時に左右向きを反転させる
	if (currentAnimation_ != "player_Wall_Slide" && actionOld_ == "player_Wall_Slide")
	{
		auto dir = direction_;
		direction_ == Direction::Right ? direction_ = Direction::Left : direction_ = Direction::Right;
		auto tmpdir = direction_;
	}

	if (onDamaged_)
	{
		// HP減少のテストコード
		auto a = ((Game*)Director::getInstance()->getRunningScene());
		auto b = (PL_HPgauge*)a->getChildByTag((int)zOlder::FRONT)->getChildByName("PL_HPgauge");
		b->SetHP(b->GetHP() - 10);	// -10などのダメージ量は敵の攻撃力に変えればいい
		onDamaged_ = false;

		if (b->GetHP() <= 0)
		{
			// 死亡状態に切り替え
			deathFlg_ = true;
		}
	}

	//if (deathFlg_)
	//{
	//	this->setPosition(deathPos_);	// 死亡した位置で設定しておかないと、おかしくなる
	//	currentAnimation_ = "Death2";
	//	animationFrame_ += delta;
	//	if (animationFrame_ >= 0.8f)
	//	{
	//		animationFrame_ = 0.8f;
	//		return;
	//	}
	//	else
	//	{
	//		currentAnimation_ = "Death2";
	//	}
	//}

	attackMotion(delta);

	// トランスフォーム
	if (_oprtState->GetNowData()[static_cast<int>(BUTTON::Transfrom)] && !_oprtState->GetOldData()[static_cast<int>(BUTTON::Transfrom)])
	{
		this->setPosition(this->getPosition().x, this->getPosition().y - 10);	// 位置補正を入れないと浮いて見える
		currentAnimation_ = "player_Transform";
		bitFlg_.TransfromFlg = true;
	}
	if (bitFlg_.TransfromFlg)
	{
		animationFrame_ += delta;
		if (animationFrame_ >= 1.85f)
		{
			bitFlg_.TransfromFlg = false;
			animationFrame_ = 0.0f;
			this->setPosition(this->getPosition().x, this->getPosition().y + 10);	// 位置補正分戻す
		}
		else
		{
			currentAnimation_ = "player_Transform";
		}
	}

	if (currentAnimation_ != actionOld_)
	{
		if (currentAnimation_ == "player_AttackFirst" && !bitFlg_.FirstAttackFlg)
		{
			bitFlg_.FirstAttackFlg = true;
		}
		lpAnimMng.ChangeAnimation(*this, currentAnimation_, true, ActorType::Player);
	}
	actionOld_ = currentAnimation_;

	TRACE("playerPos:(%f,%f)", getPosition().x, getPosition().y);
}

void Player::Action(void)
{
	int a = 0;
}

void Player::ChangeDirection(void)
{
	int a = 0;
}

void Player::attackMotion(float sp)
{
	auto moveLambda = [&](int sign) {
		// ここで壁すり抜け防止したい
		auto director = Director::getInstance();
		auto CollisionData = (TMXLayer*)director->getRunningScene()->getChildByTag((int)zOlder::BG)->getChildByName("MapData")->getChildByName("col");
		auto ColSize = CollisionData->getLayerSize();
		auto plCheckPoint1 = Vec2(oldPos_ + (AttackMove * sign), this->getPosition().y + 37);	// 自分の縦サイズ/2を加算しておく(アンカーポイントが足元だから)
		auto plCheckPoint1Pos = Vec2(plCheckPoint1.x / 48, ColSize.height - (plCheckPoint1.y / 48));
		// 範囲外check(早めのcheckで)
		if (plCheckPoint1Pos.x > ColSize.width - 2 || plCheckPoint1Pos.x < 2 ||
			plCheckPoint1Pos.y > ColSize.height || plCheckPoint1Pos.y < 0)
		{
			return false;
		}
		//TRACE("X:%f,Y:%f\n", plCheckPoint1Chip.x, plCheckPoint1Chip.y);
		if (CollisionData->getTileGIDAt(plCheckPoint1Pos) != 0)
		{
			return false;
		}

		this->runAction(cocos2d::MoveTo::create(0.0f, cocos2d::Vec2(oldPos_ + (AttackMove * sign), this->getPosition().y)));
		this->setPosition(Vec2(oldPos_ + (AttackMove * sign), this->getPosition().y));
	};

	auto keyLambda = [&](bool flag) {
		// すでにtrueになってたらtrueで抜ける
		if (flag)
		{
			return true;
		}
		if (_oprtState->GetNowData()[1] && !_oprtState->GetOldData()[1] && !flag)
		{
			return true;
		}
		return false;
	};

	if (bitFlg_.FirstAttackFlg)
	{
		currentAnimation_ = "player_AttackFirst";
		//isAttacking_ = true;
	}
	else if (bitFlg_.SecondAttackFlg)
	{
		currentAnimation_ = "player_AttackSecond";
		//isAttacking_ = true;
	}
	else if (bitFlg_.ThirdAttackFlg)
	{
		currentAnimation_ = "player_AttackThird";
		//isAttacking_ = true;
	}

	if (currentAnimation_ == "player_AttackFirst" && bitFlg_.FirstAttackFlg)
	{
		// フレーム数の取得テスト
		//auto a = animationFrame_ * 100;
		//auto b = 0.05 * 100;
		//auto c = (int)a / (int)b;
		//TRACE("%d\n", c);

		bitFlg_.SecondAttackFlg = keyLambda(bitFlg_.SecondAttackFlg);

		animationFrame_ += sp;

		// frame計算
		animationFrame_int_ = (int)(animationFrame_ * 100) / (int)(0.05 * 100);
		if (animationFrame_int_ < 10)
		{
			// 2フレーム目にdataが2つ入り、そのうちの片方がtype:0だから攻撃矩形になってる
			currentCol_ = collider_[currentAnimation_][animationFrame_int_];
			colliderVisible();
		}

		if (bitFlg_.FirstAttackFlg && animationFrame_ <= 0.5f)
		{
			if (!oldPosKeepFlg_)
			{
				oldPos_ = this->getPosition().x;
				oldPosKeepFlg_ = true;
				direction_ == Direction::Left ? moveLambda(-1) : moveLambda(1);
			}
			currentAnimation_ = "player_AttackFirst";
		}
		else
		{
			if (!bitFlg_.SecondAttackFlg)
			{
				// HP減少のテストコード
				{
					auto a = ((Game*)Director::getInstance()->getRunningScene());
					auto b = (PL_HPgauge*)a->getChildByTag((int)zOlder::FRONT)->getChildByName("PL_HPgauge");
					b->SetHP(b->GetHP() - 100);	// -10などのダメージ量は敵の攻撃力に変えればいい
					if (b->GetHP() <= 0)
					{
						// 死亡状態に切り替え
						deathFlg_ = true;
						deathPos_ = getPosition();
					}
				}

				currentAnimation_ = "player_Look_Intro";
				this->SetIsAttacking(false);
			}
			else
			{
				currentAnimation_ = "player_AttackSecond";
				oldPos_ = this->getPosition().x;
			}
			animationFrame_ = 0.0f;
			oldPosKeepFlg_ = false;
			bitFlg_.FirstAttackFlg = false;
			isAttacking_ = false;
		}
	}

	if ((currentAnimation_ == "player_AttackSecond" && bitFlg_.SecondAttackFlg))
	{
		bitFlg_.ThirdAttackFlg = keyLambda(bitFlg_.ThirdAttackFlg);

		animationFrame_ += sp;

		// frame計算
		animationFrame_int_ = (int)(animationFrame_ * 100) / (int)(0.08 * 100);
		if (animationFrame_int_ < 10)
		{
			// 2フレーム目にdataが2つ入り、そのうちの片方がtype:0だから攻撃矩形になってる
			currentCol_ = collider_[currentAnimation_][animationFrame_int_];
			colliderVisible();
		}

		if (bitFlg_.SecondAttackFlg && animationFrame_ <= 0.8f)
		{
			direction_ == Direction::Left ? moveLambda(-1) : moveLambda(1);
			currentAnimation_ = "player_AttackSecond";
		}
		else
		{
			if (!bitFlg_.ThirdAttackFlg)
			{
				currentAnimation_ = "player_Look_Intro";
			}
			else
			{
				currentAnimation_ = "player_AttackThird";
				oldPos_ = this->getPosition().x;
			}

			bitFlg_.SecondAttackFlg = false;
			isAttacking_ = false;
			animationFrame_ = 0.0f;
		}
	}

	if ((currentAnimation_ == "player_AttackThird" && bitFlg_.ThirdAttackFlg))
	{
		animationFrame_ += sp;

		// frame計算
		animationFrame_int_ = (int)(animationFrame_ * 100) / (int)(0.08 * 100);
		if (animationFrame_int_ < 11)
		{
			// 2フレーム目にdataが2つ入り、そのうちの片方がtype:0だから攻撃矩形になってる
			currentCol_ = collider_[currentAnimation_][animationFrame_int_];
			colliderVisible();
		}

		if (bitFlg_.ThirdAttackFlg && animationFrame_ <= 0.8f)
		{
			direction_ == Direction::Left ? moveLambda(-1) : moveLambda(1);
			currentAnimation_ = "player_AttackThird";
		}
		else
		{
			bitFlg_.ThirdAttackFlg = false;
			isAttacking_ = false;
			currentAnimation_ = "player_Look_Intro";
			animationFrame_ = 0.0f;
		}
	}
}

void Player::colliderVisible(void)
{
	// トランスフォームの時はコライダーを無視する
	if (currentAnimation_ == "player_Transform" || currentAnimation_ == "player_Death2")
	{
		return;
	}

	if (animationFrame_int_ <= 0)
	{
		animationFrame_int_ = 0;
	}
	CCLOG("plHP:%d", hp_);
	currentCol_ = collider_[currentAnimation_][animationFrame_int_];
	for (auto collider : this->getChildren())
	{
		if (currentAnimation_ == collider->getName() &&
			animationFrame_int_ == collider->getTag())
		{
			// 攻撃の時だけオフセットが必要
			attackCollider("player_AttackFirst", collider, attackColOffset_);
			attackCollider("player_AttackSecond", collider, attackColOffset_);
			attackCollider("player_AttackThird", collider, attackColOffset_);

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

			collider->setVisible(true);
		}
		else
		{
			collider->setVisible(false);
		}
	}
}

void Player::attackCollider(std::string str,cocos2d::Node* col,float& pos)
{
	// すでにオフセット値が0以上で設定されているときはtrueにする
	//bool tmpFlg = false;
	//if (pos > 0)
	//{
	//	tmpFlg = true;
	//}

	// ダメージ矩形だけ動かしたいけど、できていない
	// currentでtypeみる感じにするためにautofor文参照で回しても個別で設定できなかった
	// 攻撃の時だけオフセットが必要
	if (currentAnimation_ == str.c_str())
	{
		if (!this->IsAttacking())
		{
			if (direction_ == Direction::Right)
			{
				//pos = col->getPosition().x + 30.0f;
				//for (auto plcol : currentCol_)
				//{
					//if (plcol->GetData().type_ == 0)
					//{
				this->SetAttackOffset(Vec2(col->getPosition().x + 30.0f, col->getPosition().y));
				this->SetIsAttacking(true);
				//}
			//}
			}
			else
			{
				this->SetAttackOffset(cocos2d::Vec2(0.0f, 0.0f));
				this->SetIsAttacking(true);
			}

			for (auto col : this->getChildren())
			{
				// 矩形の名前がattackならば
				if (col->getName() == this->GetAction())
				{
					// 矩形のﾀｲﾌﾟがattackのやつのみの抽出
					if (col->getLocalZOrder() == 0)
					{
						// 矩形のﾎﾟｼﾞｼｮﾝの変更
						// 矩形のﾎﾟｼﾞｼｮﾝは1度のみの変更でないと
						// 毎ﾌﾚｰﾑ回ってしまうと矩形がどっかいくのでここでｾｯﾄしている。
						// 矩形がずれていくが、当たり判定は本来の位置で行われている
						col->setPosition(this->GetAttackRect().offset_.x, col->getPosition().y);
					}
				}
			}
		}

		//col->setPosition(pos, col->getPosition().y);
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
	_oprtState->KeyReset();
}

void Player::AnimRegistrator(void)
{
	// アニメーションをキャッシュに登録
	// non
	lpAnimMng.addAnimationCache("image/PlayerAnimationAsset/player/player", "Non", 6, (float)0.3, ActorType::Player,false);

	// idle
	lpAnimMng.addAnimationCache("image/PlayerAnimationAsset/player/player", "Look_Intro", 6, (float)0.3, ActorType::Player, false);

	// run
	lpAnimMng.addAnimationCache("image/PlayerAnimationAsset/player/player", "Run", 9, (float)0.08, ActorType::Player, false);

	// fall
	lpAnimMng.addAnimationCache("image/PlayerAnimationAsset/player/player", "Fall", 3, (float)1.0, ActorType::Player, false);

	// jump
	lpAnimMng.addAnimationCache("image/PlayerAnimationAsset/player/player", "Jump", 3, (float)0.05, ActorType::Player, false);

	lpAnimMng.addAnimationCache("image/PlayerAnimationAsset/player/player", "Jumping", 3, (float)0.05, ActorType::Player, false);

	// AttackFirst
	lpAnimMng.addAnimationCache("image/PlayerAnimationAsset/player/player", "AttackFirst", 9, (float)0.05, ActorType::Player, false);

	// AttackSecond
	lpAnimMng.addAnimationCache("image/PlayerAnimationAsset/player/player", "AttackSecond", 9, (float)0.08, ActorType::Player, false);

	// AttackThird
	lpAnimMng.addAnimationCache("image/PlayerAnimationAsset/player/player", "AttackThird", 10, (float)0.08, ActorType::Player, false);

	// wallslide
	lpAnimMng.addAnimationCache("image/PlayerAnimationAsset/player/player", "Wall_Slide", 3, (float)0.3, ActorType::Player, false);

	// Transform
	lpAnimMng.addAnimationCache("image/PlayerAnimationAsset/player/player", "Transform", 37, (float)0.05, ActorType::Player, false);

	// death
	lpAnimMng.addAnimationCache("image/PlayerAnimationAsset/player/player", "Death2", 10, (float)0.08, ActorType::Player, false);

	lpAnimMng.InitAnimation(*this, ActorType::Player, "player_Non");
}

const AttackRect& Player::GetAttackRect(void)
{
	return attackRect_;
}

Player* Player::CreatePlayer(int hp,Layer& myLayer)
{
	Player* pRet = new(std::nothrow) Player(hp,myLayer);
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


void Player::actModuleRegistration(void)
{
	Vec2 charSize = { 15.0f * 3.0f,25.0f * 3.0f };
	// 右移動
	{
		ActModule act;
		act.state = _oprtState;
		act.vel = Vec2{ 5,0 };
		act.actName = "player_Run";
		act.button = BUTTON::RIGHT;
		act.checkPoint1 = Vec2{ charSize.x/2, charSize.y/2 };	// 右上
		act.checkPoint2 = Vec2{ charSize.x/2,  15 };			// 右下
		act.touch = TOUCH_TIMMING::TOUCHING;	// 押しっぱなし
		//act.blackList.emplace_back(ACTION::FALLING);	// 落下中に右移動してほしくないときの追加の仕方

		//act.whiteList.emplace_back(ACTION::JUMPING);
		act.blackList.emplace_back("player_AttackFirst");
		act.blackList.emplace_back("player_AttackSecond");
		act.blackList.emplace_back("player_AttackThird");
		act.blackList.emplace_back("player_Wall_Slide");
		_actCtl.ActCtl("右移動", act);
	}

	// 左移動
	{
		ActModule act;
		act.state = _oprtState;
		act.vel = Vec2{ -5,0 };
		act.actName = "player_Run";
		act.button = BUTTON::LEFT;
		act.checkPoint1 = Vec2{ -charSize.x/2, charSize.y/2 };	// 左上
		act.checkPoint2 = Vec2{ -charSize.x/2,  15 };			// 左下
		act.touch = TOUCH_TIMMING::TOUCHING;    // 押しっぱなし

		//act.blackList.emplace_back(ACTION::FALLING);

		//act.whiteList.emplace_back(ACTION::JUMPING);
		act.blackList.emplace_back("player_AttackFirst");
		act.blackList.emplace_back("player_AttackSecond");
		act.blackList.emplace_back("player_AttackThird");
		act.blackList.emplace_back("player_Wall_Slide");
		_actCtl.ActCtl("左移動", act);
	}

	// 右向き反転
	{
		ActModule flipAct;
		flipAct.state = _oprtState;
		flipAct.flipFlg = false;
		flipAct.actName = "player_Non";
		flipAct.button = BUTTON::RIGHT;
		flipAct.touch = TOUCH_TIMMING::TOUCHING; // 押しっぱなし

		//flipAct.blackList.emplace_back(ACTION::FALLING);

		flipAct.blackList.emplace_back("player_AttackFirst");
		flipAct.blackList.emplace_back("player_AttackSecond");
		flipAct.blackList.emplace_back("player_AttackThird");
		flipAct.blackList.emplace_back("player_Wall_Slide");
		_actCtl.ActCtl("右向き", flipAct);
	}

	// 左向き反転
	{
		ActModule flipAct;
		flipAct.state = _oprtState;
		flipAct.flipFlg = true;
		flipAct.actName = "player_Non";
		flipAct.button = BUTTON::LEFT;
		flipAct.touch = TOUCH_TIMMING::TOUCHING; // 押しっぱなし

		//flipAct.blackList.emplace_back(ACTION::FALLING);

		flipAct.blackList.emplace_back("player_AttackFirst");
		flipAct.blackList.emplace_back("player_AttackSecond");
		flipAct.blackList.emplace_back("player_AttackThird");
		flipAct.blackList.emplace_back("player_Wall_Slide");
		_actCtl.ActCtl("左向き", flipAct);
	}

	// 落下
	{
		// checkkeylistに離している間の設定もしたけど特に効果なし
		ActModule act;
		act.actName = "player_Fall";
		act.state = _oprtState;
		act.button = BUTTON::DOWN;
		//act.checkPoint1 = Vec2{ 0,-10 };			// 左下
		//act.checkPoint2 = Vec2{ 0,-10 };			// 右下
		act.checkPoint1 = Vec2{ 0,0 };				// 左下
		act.checkPoint2 = Vec2{ 0,0 };				// 右下

		act.checkPoint3 = Vec2{ charSize.x / 2, charSize.y / 2 };  // 右上
		act.checkPoint4 = Vec2{ -charSize.x / 2, charSize.y / 2 }; // 左上

		act.gravity = Vec2{ 0.0f,-5.0f };
		act.touch = TOUCH_TIMMING::RELEASED;	// ずっと離している
		act.blackList.emplace_back("player_Jumping");	// ジャンプ中に落下してほしくない
		act.blackList.emplace_back("player_Wall_Slide");	
		//act.blackList.emplace_back(ACTION::JUMP);	// ジャンプ中に落下してほしくない

		_actCtl.ActCtl("落下", act);
	}

	// ジャンプ
	{
		ActModule act;
		act.state = _oprtState;
		act.button = BUTTON::UP;
		act.actName = "player_Jump";
		act.checkPoint1 = Vec2{ -charSize.x/3 + 5, charSize.y };		// 左上
		act.checkPoint2 = Vec2{ charSize.x/3 - 5, charSize.y };			// 右上
		//act.checkPoint1 = Vec2{ -10, 30 };						// 左上
		//act.checkPoint2 = Vec2{ +10, 30 };						// 右上
		act.jumpVel = Vec2{ 0.0f,20.0f };
		act.touch = TOUCH_TIMMING::ON_TOUCH;	// 押した瞬間

		// これをコメントアウトしていると、左右押しながらのジャンプができる
		// でも連続でジャンプして上昇し続けるようになる
		// しかもFALLとJUMPが混ざって高さが出ない
		act.blackList.emplace_back("player_Fall");	// 落下中にジャンプしてほしくない
		act.blackList.emplace_back("player_AttackFirst");
		act.blackList.emplace_back("player_AttackSecond");
		act.blackList.emplace_back("player_AttackThird");
		//act.whiteList.emplace_back(ACTION::RUN);

		_actCtl.ActCtl("ジャンプ", act);
	}

	// ジャンピング
	{
		ActModule act;
		act.state = _oprtState;
		act.button = BUTTON::UP;
		act.actName = "player_Jumping";
		act.checkPoint1 = Vec2{ -charSize.x/3 + 5, charSize.y };	// 左上
		act.checkPoint2 = Vec2{ charSize.x/3 - 5, charSize.y };		// 右上
		//act.checkPoint1 = Vec2{ -10, 30 };					// 左上
		//act.checkPoint2 = Vec2{ +10, 30 };					// 右上
		act.jumpVel = Vec2{ 0.0f,20.0f };
		act.touch = TOUCH_TIMMING::TOUCHING;	// 押しっぱなし

		act.blackList.emplace_back("player_Fall");	// 落下中にジャンプしてほしくない
		act.blackList.emplace_back("player_Look_Intro");
		act.blackList.emplace_back("player_Run");
		act.blackList.emplace_back("player_AttackFirst");
		act.blackList.emplace_back("player_AttackSecond");
		act.blackList.emplace_back("player_AttackThird");
		act.blackList.emplace_back("player_Non");
		act.blackList.emplace_back("player_Wall_Slide");
		act.blackList.emplace_back("player_Transform");

		act.whiteList.emplace_back("player_Jump");
		_actCtl.ActCtl("ジャンピング", act);
		//_actCtl.ActCtl("ジャンプ", act);
	}

	// 攻撃
	{
		ActModule act;
		act.state = _oprtState;
		//act.button = BUTTON::ATTACK;
		act.button = BUTTON::DOWN;
		act.actName = "player_AttackFirst";
		//act.checkPoint1 = Vec2{ 0, 0 };		
		//act.checkPoint2 = Vec2{ 0, 0 };
		//act.blackList.emplace_back("Fall");	// 一時的コメントアウト
		act.touch = TOUCH_TIMMING::ON_TOUCH;	// 押した瞬間
		_actCtl.ActCtl("攻撃", act);
	}

	// 攻撃2
	{
		ActModule act;
		act.state = _oprtState;
		act.button = BUTTON::DOWN;
		act.actName = "player_AttackSecond";
		act.blackList.emplace_back("player_Fall");
		act.touch = TOUCH_TIMMING::ON_TOUCH;	// 押した瞬間
		_actCtl.ActCtl("攻撃", act);
	}

	// 攻撃3
	{
		ActModule act;
		act.state = _oprtState;
		act.button = BUTTON::DOWN;
		act.actName = "player_AttackThird";
		act.blackList.emplace_back("player_Fall");
		act.touch = TOUCH_TIMMING::ON_TOUCH;	// 押した瞬間
		_actCtl.ActCtl("攻撃", act);
	}

	// 右壁スライド
	{
		ActModule act;
		act.state = _oprtState;
		act.gravity = Vec2{ 0.0f,-1.0f };
		act.checkPoint1 = Vec2{ charSize.x / 2, charSize.y / 2 };	// 右上
		act.checkPoint2 = Vec2{ charSize.x / 2, 0 };				// 右下
		act.button = BUTTON::RIGHT;
		act.flipFlg = true;
		act.actName = "player_Wall_Slide";
		act.touch = TOUCH_TIMMING::TOUCHING;	
		act.blackList.emplace_back("player_Jumping");	
		act.blackList.emplace_back("player_Run");	
		act.blackList.emplace_back("player_Fall");
		act.blackList.emplace_back("player_Look_Intro");
		act.blackList.emplace_back("player_Non");
		act.blackList.emplace_back("player_AttackFirst");
		act.blackList.emplace_back("player_AttackSecond");
		act.blackList.emplace_back("player_AttackThird");
		_actCtl.ActCtl("右壁スライド", act);
	}

	// 左壁スライド
	{
		ActModule act;
		act.state = _oprtState;
		act.gravity = Vec2{ 0.0f,-1.0f };
		act.checkPoint1 = Vec2{-charSize.x / 2, charSize.y / 2 };	// 左上
		act.checkPoint2 = Vec2{-charSize.x / 2, 0 };				// 左下
		act.button = BUTTON::LEFT;
		act.flipFlg = false;
		act.actName = "player_Wall_Slide";
		act.touch = TOUCH_TIMMING::TOUCHING;
		act.blackList.emplace_back("player_Jumping");	
		act.blackList.emplace_back("player_Run");
		act.blackList.emplace_back("player_Fall");
		act.blackList.emplace_back("player_Look_Intro");
		act.blackList.emplace_back("player_Non");
		act.blackList.emplace_back("player_AttackFirst");
		act.blackList.emplace_back("player_AttackSecond");
		act.blackList.emplace_back("player_AttackThird");
		_actCtl.ActCtl("左壁スライド", act);
	}
}

bool Player::gameOverAction(void)
{
	if (!_gameOverFlg)
	{
		// 今の動きを止める
		stopAllActions();

		this->setPosition(deathPos_);	// 死亡した位置で設定しておかないと、おかしくなる
		AnimationCache* animationCache = AnimationCache::getInstance();
		Animation* animation = animationCache->getAnimation("player_Death2");
		animation->setRestoreOriginalFrame(false);
		//アニメーションを実行
		_gameOverAction = runAction(
			Sequence::create(
				Animate::create(animation),
				nullptr
			));
		CC_SAFE_RETAIN(_gameOverAction);
		_gameOverFlg = true;
		return false;
	}
	if (_gameOverAction == nullptr)
	{
		// アクションが終わっている
		return true;
	}
	if (_gameOverAction->isDone())
	{
		// アクションが終わった
		CC_SAFE_RELEASE_NULL(_gameOverAction);
		return true;
	}
	return false;
}


// アンカーポイントを変更せずに攻撃モーション追加しないといけない
// →MoveTo使用

// ①colliderのpositionを保存しないといけない
// ②保存した変数にdir毎のoffsetをかける

// hpが0になったときにモーションを切り替えるところだけ作っておく