#include "Scene/GameScene.h"
#include "PL_HPgauge.h"
#include "Player.h"
#include "anim/ActionCtl.h"
#include "input/DIR_ID.h"
#include "ActionRect.h"
#include "_Debug/_DebugConOut.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
#include "input/OPRT_key.h"
#else
#include "input/OPRT_touch.h"
#endif

USING_NS_CC;

int Player::no_ = 0;

Player::Player(std::unordered_map<std::string, std::vector<std::vector<std::shared_ptr<ActionRect>>>>& collider):
	Actor(collider)
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

	actionNow_ = "Look_Intro";
	actionOld_ = "Run";
	_dir_Now = DIR::RIGHT;

	_attackCheckL = Vec2(0, 0);
	_attackCheckR = Vec2(0, 0);

	Anim_Registration((Sprite*)this);			// アニメーションの登録

	pos_ = { (int)visibleSize.width / 2 + (int)origin.x - 0,(int)visibleSize.height / 2 + (int)origin.y + 200 };
	setPosition(Vec2(pos_.x, pos_.y));
	myNo_ = no_;
	no_++;
	type_ = ActorType::Player;

	//for (auto anim : lpAnimMng.GetAnimations(type_))
	//{
	//	auto a = lpAnimMng.GetFrameNum(type_, anim);
	//	for (int i = 0; i < lpAnimMng.GetFrameNum(type_, anim); i++)
	//	{
	//		for (auto c : collider[anim][i])
	//		{
	//			auto col = c->CreateCollider();
	//			col->drawRect(Vec2(0, 0), Vec2(c->GetSize().x, c->GetSize().y), c->GetColor());
	//			col->setName(anim);
	//			this->addChild(col);
	//		}
	//	}
	//}
}

Player::~Player()
{
	// 動的なインスタンスをしたらdeleteを忘れないように!
	delete _oprtState;
}

// 毎フレーム更新
void Player::update(float sp)
{
	// getnameがgamesceneでない場合、何もしないreturn処理
	if (Director::getInstance()->getRunningScene()->getName() != "GameScene")
	{
		return;
	}
	//TRACE("%f,%f\n",getPosition().x, getPosition().y);
	Action();

	_actCtl.update(sp,*this);

	// 実行する入力先(keyかtouch)のupdateへ移動
	_oprtState->update();	

	attackMotion(sp);

	//if (_action_Now == ACTION::JUMPING)
	//{
	//	cntTest ++;
	//	if (cntTest < 20.0f)
	//	{
	//		_action_Now = ACTION::JUMPING;
	//		//actFlg = true;
	//		//return;
	//	}
	//	else
	//	{
	//		_action_Now = ACTION::FALLING;
	//		cntTest = 0.0f;
	//	}
	//	//((Player&)sprite).SetAction(ACTION::FALLING);
	//}
	// 当たり判定練習中
	//if (((Game*)Director::getInstance()->getRunningScene())->enemySprite->isVisible())
	//{
	//	int chipsize = 48;
	//	int scale = 3.0f;
	//	auto plpos = this->getPosition();
	//	auto plsize = this->getContentSize();
	//	auto enepos = ((Game*)Director::getInstance()->getRunningScene())->enemySprite->getPosition();
	//	if (plpos.x + (plsize.width * scale) / 2 >= enepos.x - chipsize / 2 &&
	//		plpos.x - (plsize.width * scale) / 2 < enepos.x + chipsize / 2 &&
	//		plpos.y + (plsize.height * scale) >= enepos.y + chipsize / 2 &&
	//		plpos.y < enepos.y + chipsize / 2)
	//	{
	//		// 攻撃した瞬間に当たり判定を入れる
	//		if (_action_Now == ACTION::ATTACK)
	//		{
	//			// 敵のvisibleをfalseにして見えなくする
	//			int a = 0;
	//			((Game*)Director::getInstance()->getRunningScene())->enemySprite->setVisible(false);
	//		}
	//	}
	//}


	// 敵との当たり判定テスト
	//if (_action_Now == ACTION::ATTACK || _action_Old == ACTION::ATTACK)
	//{
	//	if (((Game*)Director::getInstance()->getRunningScene())->enemySprite->isVisible())
	//	{
	//		int chipsize = 48;
	//		int scale = 3.0;
	//		auto plpos = this->getPosition();
	//		auto plsize = this->getContentSize();
	//		auto enepos = ((Game*)Director::getInstance()->getRunningScene())->enemySprite->getPosition();
	//		//auto enesize = ((Game*)Director::getInstance()->getRunningScene())->enemySprite->getContentSize();
	//		// 右の時はoffset+  左はoffset-
	//		auto offset = Vec2(plsize.width * 3.0f, 0.0f);
	//		if (_dir_Now == DIR::LEFT)
	//		{
	//			_attackCheckL = Vec2(plpos.x + plsize.width / 2, plpos.y) - offset;
	//			_attackCheckR = Vec2(plpos.x + plsize.width + plsize.width / 2, plpos.y + plsize.width) - offset;
	//		}
	//		else if (_dir_Now == DIR::RIGHT)
	//		{
	//			_attackCheckL = Vec2(plpos.x - (plsize.width * scale + plsize.width * scale / 2), plpos.y + plsize.width * scale) + offset;
	//			_attackCheckR = Vec2(plpos.x - plsize.width * scale / 2, plpos.y) + offset;
	//		}
	//		auto e = enepos.x - chipsize / 2;
	//		auto ee = enepos.x + chipsize / 2;
	//		//TRACE("%f\n", _attackCheckL.x + offset.x);
	//		if (_attackCheckR.x >= enepos.x - chipsize / 2 &&
	//			_attackCheckL.x < enepos.x + chipsize / 2 &&
	//			plpos.y + (plsize.height * scale) >= enepos.y + chipsize / 2 &&
	//			plpos.y < enepos.y + chipsize / 2)
	//		{
	//			// 攻撃した瞬間に当たり判定を入れる
	//			if (_action_Now == ACTION::ATTACK)
	//			{
	//				// 敵のvisibleをfalseにして見えなくする
	//				((Game*)Director::getInstance()->getRunningScene())->enemySprite->setVisible(false);
	//			}
	//		}
	//	}
	//}

	//auto plpos = this->getPosition();
	//TRACE("%f\n", plpos.y);
	//TRACE("%d\n", _action_Now);
	// 範囲外check
	//OutOfMapCheck();	

	if (actionNow_ == "Wall_Slide")
	{
		if (_dir_Now == DIR::LEFT)
		{
			// アンカーポイント右端
			this->setAnchorPoint(Vec2(1.0f, 0.0f));
		}
		else if (_dir_Now == DIR::RIGHT)
		{
			// アンカーポイント左端
			this->setAnchorPoint(Vec2(0.0f, 0.0f));
		}
	}

	//if (actionNow_ == "Look_Intro")
	//{
	//	this->setAnchorPoint(Vec2(0.5f, 0.0f));
	//}

	if (actionNow_ != actionOld_)
	{
		lpAnimMng.ChangeAnimation(*this, actionNow_, true, ActorType::Player);
	}
	actionOld_ = actionNow_;

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
	// flagがtrueの時は強制的にAttackSecondへ切替
	if (attackflg)
	{
		actionNow_ = "AttackSecond";
	}

	if (actionNow_ == "AttackFirst" || actionOld_ == "AttackFirst")
	{
		// フレーム数の取得テスト
		//auto a = cntTest * 100;
		//auto b = 0.05 * 100;
		//auto c = (int)a / (int)b;
		//TRACE("%d\n", c);

		// 攻撃中にもう一度攻撃ボタンが押されたらAttackFirstが終了後、AttackSecondへ移行するようにする
		auto keyN = _oprtState->GetNowData();
		auto keyO = _oprtState->GetOldData();
		if (keyN[1] && !keyO[1])
		{
			attackflg = true;
		}

		cntTest += sp;
		if (cntTest <= 0.5f)
		{
			if (_dir_Now == DIR::LEFT)
			{
				// アンカーポイント右端
				this->setAnchorPoint(Vec2(1.0f, 0.0f));
			}
			else if (_dir_Now == DIR::RIGHT)
			{
				// アンカーポイント左端
				this->setAnchorPoint(Vec2(0.0f, 0.0f));
			}
			actionNow_ = "AttackFirst";
		}
		else
		{
			if (!attackflg)
			{
				actionNow_ = "Look_Intro";
			}
			else
			{
				actionNow_ = "AttackSecond";
				oldPos_ = this->getPosition().x;
			}
			cntTest = 0.0f;

			// HP減少のテストコード
			// 攻撃するたびにHPが10減るようにしている
			auto a = ((Game*)Director::getInstance()->getRunningScene());
			auto b = (PL_HPgauge*)a->getChildByTag((int)zOlder::FRONT)->getChildByName("PL_HPgauge");
			b->SetHP(b->GetHP() - 10);
		}
	}

	if ((actionNow_ == "AttackSecond" && attackflg))
	{
		cntTest += sp;
		if (attackflg && cntTest <= 0.8f)
		{
			if (_dir_Now == DIR::LEFT)
			{
				// アンカーポイント右端
				this->setAnchorPoint(Vec2(1.0f, 0.0f));
				// 踏み込み移動
				this->runAction(cocos2d::MoveTo::create(0.0f, cocos2d::Vec2(oldPos_ - 30, this->getPosition().y)));
				this->setPosition(Vec2(oldPos_ - 30, this->getPosition().y));
			}
			else if (_dir_Now == DIR::RIGHT)
			{
				// アンカーポイント左端
				this->setAnchorPoint(Vec2(0.0f, 0.0f));
				// 踏み込み移動
				this->runAction(cocos2d::MoveTo::create(0.0f, cocos2d::Vec2(oldPos_ + 30, this->getPosition().y)));
				this->setPosition(Vec2(oldPos_ + 30, this->getPosition().y));
			}
			actionNow_ = "AttackSecond";
		}
		else
		{
			attackflg = false;
			actionNow_ = "Look_Intro";
			cntTest = 0.0f;
		}
	}
}

// ("plistの名前","plistにあるpngの名前","つけたい名前",開始番号, 終了番号,反転するか,描画速度)
void Player::Anim_Registration(Sprite* delta)
{
	// アニメーションをキャッシュに登録
	// non
	lpAnimMng.addAnimationCache("image/PlayerAnimetionAsset/Light/Light", "NON", 6, (float)0.3, ActorType::Player);

	// idle
	lpAnimMng.addAnimationCache("image/PlayerAnimetionAsset/Light/Light", "Look_Intro", 6, (float)0.3, ActorType::Player);

	// run
	lpAnimMng.addAnimationCache("image/PlayerAnimetionAsset/Light/Light", "Run",9, (float)0.08, ActorType::Player);

	// fall
	lpAnimMng.addAnimationCache("image/PlayerAnimetionAsset/Light/Light", "Fall", 3, (float)1.0, ActorType::Player);

	// jump
	lpAnimMng.addAnimationCache("image/PlayerAnimetionAsset/Light/Light", "Jump", 3, (float)0.05, ActorType::Player);

	lpAnimMng.addAnimationCache("image/PlayerAnimetionAsset/Light/Light", "Jumping", 3, (float)0.05, ActorType::Player);

	// AttackFirst
	lpAnimMng.addAnimationCache("image/PlayerAnimetionAsset/Light/Light", "AttackFirst", 9, (float)0.05, ActorType::Player);

	// AttackSecond
	lpAnimMng.addAnimationCache("image/PlayerAnimetionAsset/Light/Light", "AttackSecond", 9, (float)0.08, ActorType::Player);

	// wallslide
	lpAnimMng.addAnimationCache("image/PlayerAnimetionAsset/Light/Light", "Wall_Slide", 3, (float)0.3, ActorType::Player);

	lpAnimMng.InitAnimation(*delta, ActorType::Player);

}

// 現在のアクション情報を取得する
std::string Player::GetAction(void)
{
	return actionNow_;
}

// 現在のアクション状態をセットする
void Player::SetAction(std::string action)
{
	actionNow_ = action;
}

void Player::SetDir(DIR dir)
{
	_dir_Now = dir;
}

Player* Player::CreatePlayer(std::unordered_map<std::string, std::vector<std::vector<std::shared_ptr<ActionRect>>>>& collider)
{
	Player* pRet = new(std::nothrow) Player(collider);
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

// playerがmapの範囲外に出ていないかの確認
void Player::OutOfMapCheck(void)
{
	auto director = Director::getInstance();
	CharCallData(director, "player");
	CollisionMapCallData(director, "MapData", "col");
	_plPos = { _player->getPosition()+ _outOfMapCheckVel };
	_ColLayerSize = _CollisionData->getLayerSize();

	// 上
	if (_plPos.y > _ColLayerSize.height * _tileChip - (_tileChip * 2))
	{
		setPosition(getPosition().x , _ColLayerSize.height * _tileChip - (_tileChip * 2));
	}

	// 下
	if (_plPos.y < 90)
	{
		setPosition(getPosition() + Vec2{ 0.0f,5.0f });
	}

	// 左
	if (_plPos.x < 40)
	{
		setPosition(getPosition() - _outOfMapCheckVel+ Vec2{ 5.0f,0.0f });
	}

	// 右
	if (_plPos.x > _ColLayerSize.width  * _tileChip - 70)
	{
		setPosition(getPosition() - _outOfMapCheckVel - Vec2{ 5.0f,0.0f });
	}
}

// playerの情報
void Player::CharCallData(cocos2d::Director * director, const char * charName)
{
	// 情報の辿り方(Director->GameScene->CharLayer->PlSprite)
	_player = (TMXLayer*)director->getRunningScene()->getChildByName("CHAR_LAYER")->getChildByName(charName);
}

// 当たり判定したいmapの情報
void Player::CollisionMapCallData(cocos2d::Director * director, const char * mapName, const char * collisionName)
{
	// 情報の辿り方(Director->GameScene->bgLayer->TMXのmapData->さらにその中のcollisionレイヤー)
	_CollisionData = (TMXLayer*)director->getRunningScene()->getChildByName("BG_BACK")->getChildByName(mapName)->getChildByName(collisionName);
}

void Player::actModuleRegistration(void)
{
	Vec2 charSize = { 15.0f * 3.0f,25.0f * 3.0f };
	// 右移動
	{
		ActModule act;
		act.state = _oprtState;
		act.vel = Vec2{ 5,0 };
		act.actName = "Run";
		act.button = BUTTON::RIGHT;
		act.checkPoint1 = Vec2{ charSize.x/2, charSize.y/2 };	// 右上
		act.checkPoint2 = Vec2{ charSize.x/2,  15 };			// 右下
		act.touch = TOUCH_TIMMING::TOUCHING;	// 押しっぱなし
		act.jumpFlg = false;
		//act.blackList.emplace_back(ACTION::FALLING);	// 落下中に右移動してほしくないときの追加の仕方

		//act.whiteList.emplace_back(ACTION::JUMPING);
		act.blackList.emplace_back("AttackFirst");
		act.blackList.emplace_back("AttackSecond");
		act.blackList.emplace_back("Wall_Slide");
		_actCtl.ActCtl("右移動", act);
	}

	// 左移動
	{
		ActModule act;
		act.state = _oprtState;
		act.vel = Vec2{ -5,0 };
		act.actName = "Run";
		act.button = BUTTON::LEFT;
		act.checkPoint1 = Vec2{ -charSize.x/2, charSize.y/2 };	// 左上
		act.checkPoint2 = Vec2{ -charSize.x/2,  15 };			// 左下
		act.touch = TOUCH_TIMMING::TOUCHING;    // 押しっぱなし
		act.jumpFlg = false;

		//act.blackList.emplace_back(ACTION::FALLING);

		//act.whiteList.emplace_back(ACTION::JUMPING);
		act.blackList.emplace_back("AttackFirst");
		act.blackList.emplace_back("AttackSecond");
		act.blackList.emplace_back("Wall_Slide");
		_actCtl.ActCtl("左移動", act);
	}

	// 右向き反転
	{
		ActModule flipAct;
		flipAct.state = _oprtState;
		flipAct.flipFlg = false;
		flipAct.actName = "NON";
		flipAct.button = BUTTON::RIGHT;
		flipAct.touch = TOUCH_TIMMING::TOUCHING; // 押しっぱなし
		flipAct.jumpFlg = false;

		//flipAct.blackList.emplace_back(ACTION::FALLING);

		flipAct.blackList.emplace_back("AttackFirst");
		flipAct.blackList.emplace_back("AttackSecond");
		flipAct.blackList.emplace_back("Wall_Slide");
		_actCtl.ActCtl("右向き", flipAct);
	}

	// 左向き反転
	{
		ActModule flipAct;
		flipAct.state = _oprtState;
		flipAct.flipFlg = true;
		flipAct.actName = "NON";
		flipAct.button = BUTTON::LEFT;
		flipAct.touch = TOUCH_TIMMING::TOUCHING; // 押しっぱなし
		flipAct.jumpFlg = false;

		//flipAct.blackList.emplace_back(ACTION::FALLING);

		flipAct.blackList.emplace_back("AttackFirst");
		flipAct.blackList.emplace_back("AttackSecond");
		flipAct.blackList.emplace_back("Wall_Slide");
		_actCtl.ActCtl("左向き", flipAct);
	}

	// 落下
	{
		// checkkeylistに離している間の設定もしたけど特に効果なし
		ActModule act;
		act.actName = "Fall";
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
		act.jumpFlg = false;
		act.blackList.emplace_back("Jumping");	// ジャンプ中に落下してほしくない
		act.blackList.emplace_back("Wall_Slide");	
		//act.blackList.emplace_back(ACTION::JUMP);	// ジャンプ中に落下してほしくない

		_actCtl.ActCtl("落下", act);
	}

	// ジャンプ
	{
		ActModule act;
		act.state = _oprtState;
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
		act.jumpFlg = true;
		act.blackList.emplace_back("AttackFirst");
		act.blackList.emplace_back("AttackSecond");

		//act.whiteList.emplace_back(ACTION::RUN);

		_actCtl.ActCtl("ジャンプ", act);
	}

	// ジャンピング
	{
		ActModule act;
		act.state = _oprtState;
		act.button = BUTTON::UP;
		act.actName = "Jumping";
		act.checkPoint1 = Vec2{ -charSize.x/3 + 5, charSize.y };	// 左上
		act.checkPoint2 = Vec2{ charSize.x/3 - 5, charSize.y };		// 右上
		//act.checkPoint1 = Vec2{ -10, 30 };					// 左上
		//act.checkPoint2 = Vec2{ +10, 30 };					// 右上
		act.jumpVel = Vec2{ 0.0f,20.0f };
		act.touch = TOUCH_TIMMING::TOUCHING;	// 押しっぱなし
		act.jumpFlg = true;

		act.blackList.emplace_back("Fall");	// 落下中にジャンプしてほしくない
		act.blackList.emplace_back("Look_Intro");
		act.blackList.emplace_back("Run");
		act.blackList.emplace_back("AttackFirst");
		act.blackList.emplace_back("AttackSecond");
		act.blackList.emplace_back("NON");
		act.blackList.emplace_back("Wall_Slide");

		act.whiteList.emplace_back("Jump");
		_actCtl.ActCtl("ジャンピング", act);
		//_actCtl.ActCtl("ジャンプ", act);
	}

	// 攻撃
	{
		ActModule act;
		act.state = _oprtState;
		//act.button = BUTTON::ATTACK;
		act.button = BUTTON::DOWN;
		act.actName = "AttackFirst";
		//act.checkPoint1 = Vec2{ 0, 0 };		
		//act.checkPoint2 = Vec2{ 0, 0 };
		act.touch = TOUCH_TIMMING::ON_TOUCH;	// 押した瞬間
		_actCtl.ActCtl("攻撃", act);
	}

	// 攻撃2
	{
		ActModule act;
		act.state = _oprtState;
		act.button = BUTTON::DOWN;
		act.actName = "AttackSecond";
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
		act.actName = "Wall_Slide";
		act.touch = TOUCH_TIMMING::TOUCHING;	
		act.blackList.emplace_back("Jumping");	// ジャンプ中に落下してほしくない
		act.blackList.emplace_back("Run");	
		act.blackList.emplace_back("Fall");
		act.blackList.emplace_back("Look_Intro");
		act.blackList.emplace_back("NON");
		act.num = 95;
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
		act.actName = "Wall_Slide";
		act.touch = TOUCH_TIMMING::TOUCHING;
		act.blackList.emplace_back("Jumping");	// ジャンプ中に落下してほしくない
		act.blackList.emplace_back("Run");
		act.blackList.emplace_back("Fall");
		act.blackList.emplace_back("Look_Intro");
		act.blackList.emplace_back("NON");
		act.num = 95;
		_actCtl.ActCtl("左壁スライド", act);
	}
}