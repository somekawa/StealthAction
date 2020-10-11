#include "GameScene.h"
#include "Player.h"
#include "anim/ActionCtl.h"
#include "input/DIR_ID.h"
#include "_Debug/_DebugConOut.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
#include "input/OPRT_touch.h"
#else
#include "input/OPRT_touch.h"
#endif

//#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
//#include "input/OPRT_touch.h"
//#else
//#include "input/OPRT_touch.h"
//#endif

USING_NS_CC;

int Player::no_ = 0;

Player* Player::createPlayer()
{
	return Player::create();
}

Player::Player()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// this->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	// 上のやつを短くしたversion↓
	setPosition( Vec2{ Vec2(visibleSize) / 2 + origin - Vec2(0,-200)} );

	// キー入力かタッチ操作か判断
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	// thisの意味
	_oprtState = new OPRT_touch(this);
#else
	_oprtState = new OPRT_touch(this);
#endif

//#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
//// thisの意味
//	_oprtState = new OPRT_touch(this);
//#else
//	_oprtState = new OPRT_touch(this);
//#endif
	
	actModuleRegistration();

	_action_Now = ACTION::IDLE;
	_action_Old = ACTION::IDLE;
	_dir_Now = DIR::RIGHT;

	_attackCheckL = Vec2(0, 0);
	_attackCheckR = Vec2(0, 0);

	Anim_Registration((Sprite*)this);			// アニメーションの登録

	myNo_ = no_;
	no_++;
	type_ = ActorType::Player;
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
	
	Action();

	_actCtl.update(sp,*this);

	// 実行する入力先(keyかtouch)のupdateへ移動
	_oprtState->update();	

	// Aキーで攻撃モーション--------------------

	// ActModuleに設定すれば、ここのキー設定は不要になる
	//auto nowdata = _oprtState->GetNowData();
	//auto olddata = _oprtState->GetOldData();
	//if (nowdata[4] && !olddata[4])
	//{
	//	sp = 0;
	//	_action_Now = ACTION::ATTACK;
	//}

	if (_action_Now == ACTION::ATTACK || _action_Old == ACTION::ATTACK)
	{
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
			_action_Now = ACTION::ATTACK;
		}
		else
		{
			_action_Now = ACTION::IDLE;
			cntTest = 0.0f;
		}
	}
	//------------------------------------------

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
	//TRACE("%f\n", plpos.x);
	TRACE("%d\n", _action_Now);
	// 範囲外check
	//OutOfMapCheck();	

	if (_action_Now != _action_Old)
	{
		AnimCheck(this);
	}
	_action_Old = _action_Now;

}

void Player::Action(void)
{
	int a = 0;
}

void Player::ChangeDirection(void)
{
	int a = 0;
}

// アニメーション切り替え指定
void Player::AnimCheck(cocos2d::Sprite * delta)
{
	//if (_action_Now == ACTION::ATTACK)
	//{
	//	AnimMng::ChangeAnim(delta, "attack");
	//	return;
	//}

	// 現在のアクション状態と比べて、一致しているものに切り替える
	for (ACTION _act = begin(ACTION()); _act <= end(ACTION()); ++_act)
	{
		if (_action_Now == _act)
		{
			lpAnimMng.ChangeAnimation(*delta, _animTable[static_cast<int>(_act)],true,ActorType::Player);
			return;							// 一致したときにそれ以上for文を回す必要がないからreturnする
		}
	}
}

// ("plistの名前","plistにあるpngの名前","つけたい名前",開始番号, 終了番号,反転するか,描画速度)
void Player::Anim_Registration(Sprite* delta)
{
	//// アニメーションをキャッシュに登録
	//// idle
	//lpAnimMng.addAnimationCache("image/PlayerAnimetionAsset/Light/Light_Look_Intro.plist", "look_intro%d.png", "idle", 0, 6, false, (float)0.3);
	//// run
	//lpAnimMng.addAnimationCache("image/PlayerAnimetionAsset/Light/Light_Run.plist"      , "run%d.png"  , "run" , 0, 9, false, (float)0.08);
	//// fall
	//lpAnimMng.addAnimationCache("image/PlayerAnimetionAsset/Light/Light_Fall.plist"        , "fall%d.png"    , "fall"    , 0, 3,  false, (float)1.0);
	//// shoot-up
	////lpAnimMng.addAnimationCache("image/Sprites/player/player-shoot-up/shoot-up-big.plist", "player-shoot-up-big.png", "shoot-up", 0, 0,  false, (float)1.0);
	//// jump
	//lpAnimMng.addAnimationCache("image/PlayerAnimetionAsset/Light/Light_Jump.plist"        , "jump%d.png" , "jump"    , 0, 3,  false, (float)0.05);
	//// attack
	//lpAnimMng.addAnimationCache("image/PlayerAnimetionAsset/Light/Light_Attack1.plist", "attack1_%d.png", "attack", 0, 9, false, (float)0.05);
	//lpAnimMng.anim_action(delta);

	// アニメーションをキャッシュに登録
	// idle
	lpAnimMng.addAnimationCache("image/PlayerAnimetionAsset/Light/Light", "Look_Intro", 6, (float)0.3, AnimationType::Idle, ActorType::Player);

	// run
	lpAnimMng.addAnimationCache("image/PlayerAnimetionAsset/Light/Light", "Run",9, (float)0.08, AnimationType::Run, ActorType::Player);

	// fall
	lpAnimMng.addAnimationCache("image/PlayerAnimetionAsset/Light/Light", "Fall", 3, (float)1.0, AnimationType::Fall, ActorType::Player);

	// jump
	lpAnimMng.addAnimationCache("image/PlayerAnimetionAsset/Light/Light", "Jump", 3, (float)0.05, AnimationType::Jump, ActorType::Player);

	// attack
	lpAnimMng.addAnimationCache("image/PlayerAnimetionAsset/Light/Light", "AttackA", 9, (float)0.05, AnimationType::Attack, ActorType::Player);

	lpAnimMng.InitAnimation(*delta, ActorType::Player);

}

// 現在のアクション情報を取得する
ACTION Player::GetAction(void)
{
	return _action_Now;
}

// 現在のアクション状態をセットする
void Player::SetAction(ACTION action)
{
	_action_Now = action;
}

void Player::SetDir(DIR dir)
{
	_dir_Now = dir;
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
		act.action = ACTION::RUN_R;
		act.button = BUTTON::RIGHT;
		act.checkPoint1 = Vec2{ charSize.x/2, charSize.y/2 };	// 右上
		act.checkPoint2 = Vec2{ charSize.x/2,  15 };			// 右下
		act.touch = TOUCH_TIMMING::TOUCHING;	// 押しっぱなし
		act.jumpFlg = false;
		//act.blackList.emplace_back(ACTION::FALLING);	// 落下中に右移動してほしくないときの追加の仕方

		//act.whiteList.emplace_back(ACTION::JUMPING);
		act.blackList.emplace_back(ACTION::ATTACK);
		_actCtl.ActCtl("右移動", act);
	}

	// 左移動
	{
		ActModule act;
		act.state = _oprtState;
		act.vel = Vec2{ -5,0 };
		act.action = ACTION::RUN_L;
		act.button = BUTTON::LEFT;
		act.checkPoint1 = Vec2{ -charSize.x/2, charSize.y/2 };	// 左上
		act.checkPoint2 = Vec2{ -charSize.x/2,  15 };			// 左下
		act.touch = TOUCH_TIMMING::TOUCHING;    // 押しっぱなし
		act.jumpFlg = false;

		//act.blackList.emplace_back(ACTION::FALLING);

		//act.whiteList.emplace_back(ACTION::JUMPING);
		act.blackList.emplace_back(ACTION::ATTACK);
		_actCtl.ActCtl("左移動", act);
	}

	// 右向き反転
	{
		ActModule flipAct;
		flipAct.state = _oprtState;
		flipAct.flipFlg = false;
		flipAct.action = ACTION::NON;
		flipAct.button = BUTTON::RIGHT;
		flipAct.touch = TOUCH_TIMMING::TOUCHING; // 押しっぱなし
		flipAct.jumpFlg = false;

		//flipAct.blackList.emplace_back(ACTION::FALLING);

		flipAct.blackList.emplace_back(ACTION::ATTACK);
		_actCtl.ActCtl("右向き", flipAct);
	}

	// 左向き反転
	{
		ActModule flipAct;
		flipAct.state = _oprtState;
		flipAct.flipFlg = true;
		flipAct.action = ACTION::NON;
		flipAct.button = BUTTON::LEFT;
		flipAct.touch = TOUCH_TIMMING::TOUCHING; // 押しっぱなし
		flipAct.jumpFlg = false;

		//flipAct.blackList.emplace_back(ACTION::FALLING);

		flipAct.blackList.emplace_back(ACTION::ATTACK);
		_actCtl.ActCtl("左向き", flipAct);
	}

	// 落下
	{
		// checkkeylistに離している間の設定もしたけど特に効果なし
		ActModule act;
		act.action = ACTION::FALLING;
		act.state = _oprtState;
		act.button = BUTTON::DOWN;
		//act.checkPoint1 = Vec2{ 0,-10 };			// 左下
		//act.checkPoint2 = Vec2{ 0,-10 };			// 右下
		act.checkPoint1 = Vec2{ 0,0 };				// 左下
		act.checkPoint2 = Vec2{ 0,0 };				// 右下
		act.gravity = Vec2{ 0.0f,-5.0f };
		act.touch = TOUCH_TIMMING::RELEASED;	// ずっと離している
		act.jumpFlg = false;
		act.blackList.emplace_back(ACTION::JUMPING);	// ジャンプ中に落下してほしくない
		//act.blackList.emplace_back(ACTION::JUMP);	// ジャンプ中に落下してほしくない
		_actCtl.ActCtl("落下", act);
	}

	// ジャンプ
	{
		ActModule act;
		act.state = _oprtState;
		act.button = BUTTON::UP;
		act.action = ACTION::JUMP;
		act.checkPoint1 = Vec2{ -charSize.x/3, charSize.y };		// 左上
		act.checkPoint2 = Vec2{ charSize.x/3, charSize.y };			// 右上
		//act.checkPoint1 = Vec2{ -10, 30 };						// 左上
		//act.checkPoint2 = Vec2{ +10, 30 };						// 右上
		act.jumpVel = Vec2{ 0.0f,20.0f };
		act.touch = TOUCH_TIMMING::ON_TOUCH;	// 押した瞬間

		// これをコメントアウトしていると、左右押しながらのジャンプができる
		// でも連続でジャンプして上昇し続けるようになる
		// しかもFALLとJUMPが混ざって高さが出ない
		act.blackList.emplace_back(ACTION::FALLING);	// 落下中にジャンプしてほしくない
		act.jumpFlg = true;
		act.blackList.emplace_back(ACTION::ATTACK);

		//act.whiteList.emplace_back(ACTION::RUN);

		_actCtl.ActCtl("ジャンプ", act);
	}

	// ジャンピング
	{
		ActModule act;
		act.state = _oprtState;
		act.button = BUTTON::UP;
		act.action = ACTION::JUMPING;
		act.checkPoint1 = Vec2{ -charSize.x/3, charSize.y };	// 左上
		act.checkPoint2 = Vec2{ charSize.x/3, charSize.y };		// 右上
		//act.checkPoint1 = Vec2{ -10, 30 };					// 左上
		//act.checkPoint2 = Vec2{ +10, 30 };					// 右上
		act.jumpVel = Vec2{ 0.0f,20.0f };
		act.touch = TOUCH_TIMMING::TOUCHING;	// 押しっぱなし
		act.jumpFlg = true;

		act.blackList.emplace_back(ACTION::FALLING);	// 落下中にジャンプしてほしくない
		act.blackList.emplace_back(ACTION::IDLE);
		act.blackList.emplace_back(ACTION::RUN_L);
		act.blackList.emplace_back(ACTION::RUN_R);
		act.blackList.emplace_back(ACTION::ATTACK);
		act.blackList.emplace_back(ACTION::NON);

		act.whiteList.emplace_back(ACTION::JUMP);

		_actCtl.ActCtl("ジャンピング", act);
		//_actCtl.ActCtl("ジャンプ", act);
	}

	// 攻撃
	{
		ActModule act;
		act.state = _oprtState;
		//act.button = BUTTON::ATTACK;
		act.button = BUTTON::DOWN;
		act.action = ACTION::ATTACK;
		//act.checkPoint1 = Vec2{ 0, 0 };		
		//act.checkPoint2 = Vec2{ 0, 0 };
		act.touch = TOUCH_TIMMING::ON_TOUCH;	// 押した瞬間
		_actCtl.ActCtl("攻撃", act);
	}
}