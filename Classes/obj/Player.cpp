#include "Player.h"
#include "anim/ActionCtl.h"
#include "input/DIR_ID.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
#include "input/OPRT_key.h"
#else
#include "input/OPRT_touch.h"
#endif

//#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
//#include "input/OPRT_touch.h"
//#else
//#include "input/OPRT_touch.h"
//#endif


USING_NS_CC;

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
	_oprtState = new OPRT_key(this);
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

	Anim_Registration((Sprite*)this);			// アニメーションの登録
}

Player::~Player()
{
	// 動的なインスタンスをしたらdeleteを忘れないように!
	delete _oprtState;
}

// 毎フレーム更新
void Player::update(float sp)
{
	_actCtl.update(sp,*this);

	// 実行する入力先(keyかtouch)のupdateへ移動
	_oprtState->update();		  

	// 範囲外check
	OutOfMapCheck();	

	if (_action_Now != _action_Old)
	{
		AnimCheck(this);
	}
	_action_Old = _action_Now;
}

// アニメーション切り替え指定
void Player::AnimCheck(cocos2d::Sprite * delta)
{
	// 現在のアクション状態と比べて、一致しているものに切り替える
	for (ACTION _act = begin(ACTION()); _act <= end(ACTION()); ++_act)
	{
		if (GetAction() == _act)
		{
			AnimMng::ChangeAnim(delta, _animTable[static_cast<int>(_act)]);
			return;							// 一致したときにそれ以上for文を回す必要がないからreturnする
		}
	}
}

// ("plistの名前","plistにあるpngの名前","つけたい名前",開始番号, 終了番号,反転するか,描画速度)
void Player::Anim_Registration(Sprite* delta)
{
	// アニメーションをキャッシュに登録
	// idle
	//AnimMng::addAnimationCache("image/Sprites/player/player-idle/idle-big.plist","player-idle-%d-big.png" , "idle", 1, 4,  false, (float)0.1);
	// run
	//AnimMng::addAnimationCache("image/Sprites/player/player-run/run-big.plist", "player-run-%d-big.png", "run", 1, 10, false, (float)0.08);
	// jump
	//AnimMng::addAnimationCache("image/Sprites/player/player-jump/jump-big.plist", "player-jump-%d-big.png", "jump", 1, 6, false, (float)0.05);
	// duck
	//AnimMng::addAnimationCache("image/Sprites/player/player-duck/duck-big.plist", "player-duck-big.png", "duck", 0, 0, false, (float)1.0);

	// idle
	AnimMng::addAnimationCache("image/PlayerAnimetionAsset/Light/Light_Look_Intro.plist", "look_intro%d.png", "idle", 0, 6, false, (float)0.3);

	// run
	AnimMng::addAnimationCache("image/PlayerAnimetionAsset/Light/Light_Run.plist"      , "run%d.png"  , "run" , 0, 9, false, (float)0.08);

	// duck
	AnimMng::addAnimationCache("image/PlayerAnimetionAsset/Light/Light_Fall.plist"        , "fall%d.png"    , "duck"    , 0, 3,  false, (float)1.0);

	// shoot-up
	AnimMng::addAnimationCache("image/Sprites/player/player-shoot-up/shoot-up-big.plist", "player-shoot-up-big.png", "shoot-up", 0, 0,  false, (float)1.0);

	// jump
	AnimMng::addAnimationCache("image/PlayerAnimetionAsset/Light/Light_Jump.plist"        , "jump%d.png" , "jump"    , 0, 3,  false, (float)0.05);

	AnimMng::anim_action(delta);
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

// playerがmapの範囲外に出ていないかの確認
void Player::OutOfMapCheck(void)
{
	auto director = Director::getInstance();
	CharCallData(director, "player");
	CollisionMapCallData(director, "MapData", "col");
	_plPos = { _player->getPosition()+ _outOfMapCheckVel };
	_ColLayerSize = _CollisionData->getLayerSize();

	// 上
	if (_plPos.y > _ColLayerSize.height * _tileChip - 50)
	{
		setPosition(getPosition().x , _ColLayerSize.height * _tileChip - 50);
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
	// 右移動
	{
		ActModule act;
		act.state = _oprtState;
		act.vel = Vec2{ 5,0 };
		act.action = ACTION::RUN;
		act.button = BUTTON::RIGHT;
		//act.checkPoint1 = Vec2{ 30, 40 };		// 右上
		//act.checkPoint2 = Vec2{ 30,-55 };		// 右下
		act.checkPoint1 = Vec2{ 30, 40 };		// 右上
		act.checkPoint2 = Vec2{ 30,  -10 };		// 右下
		act.touch = TOUCH_TIMMING::TOUCHING;	// 押しっぱなし
		//act.blackList.emplace_back(ACTION::FALL);	// 落下中に右移動してほしくないときの追加の仕方
		_actCtl.ActCtl("右移動", act);
	}

	// 左移動
	{
		ActModule act;
		act.state = _oprtState;
		act.vel = Vec2{ -5,0 };
		act.action = ACTION::RUN;
		act.button = BUTTON::LEFT;
		//act.checkPoint1 = Vec2{ -30, 40 };		// 左上
		//act.checkPoint2 = Vec2{ -30,-55 };		// 左下
		act.checkPoint1 = Vec2{ -30, 40 };			// 左上
		act.checkPoint2 = Vec2{ -30,  -10 };		// 左下
		act.touch = TOUCH_TIMMING::TOUCHING;    // 押しっぱなし
		//act.blackList.emplace_back(ACTION::FALL);
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
		//flipAct.blackList.emplace_back(ACTION::FALL);
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
		//flipAct.blackList.emplace_back(ACTION::FALL);
		_actCtl.ActCtl("左向き", flipAct);
	}

	// 落下
	{
		ActModule act;
		act.action = ACTION::FALLING;
		//act.checkPoint1 = Vec2{ 0,-70 };		// 左下
		//act.checkPoint2 = Vec2{ 0,-70 };		// 右下
		act.checkPoint1 = Vec2{ 0,-30 };		// 左下
		act.checkPoint2 = Vec2{ 0,-30 };		// 右下
		act.gravity = Vec2{ 0.0f,-5.0f };
		act.touch = TOUCH_TIMMING::RELEASED;	// ずっと離している
		act.blackList.emplace_back(ACTION::JUMPING);	// ジャンプ中に落下してほしくない
		_actCtl.ActCtl("落下", act);
	}

	// ジャンプ
	{
		ActModule act;
		act.state = _oprtState;
		act.button = BUTTON::UP;
		act.action = ACTION::JUMPING;
		//act.checkPoint1 = Vec2{ 0, 10 };		// 左上
		//act.checkPoint2 = Vec2{ 0, 10 };		// 右上
		act.checkPoint1 = Vec2{ -10, 30 };		// 左上
		act.checkPoint2 = Vec2{ +10, 30 };		// 右上
		act.jumpVel = Vec2{ 0.0f,20.0f };
		act.touch = TOUCH_TIMMING::TOUCHING;	// 押した瞬間
		act.blackList.emplace_back(ACTION::FALLING);	// 落下中にジャンプしてほしくない
		_actCtl.ActCtl("ジャンプ", act);
	}
}