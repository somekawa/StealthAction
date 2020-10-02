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
	// ��̂��Z������version��
	setPosition( Vec2{ Vec2(visibleSize) / 2 + origin - Vec2(0,-200)} );

	// �L�[���͂��^�b�`���삩���f
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	// this�̈Ӗ�
	_oprtState = new OPRT_key(this);
#else
	_oprtState = new OPRT_touch(this);
#endif

//#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
//// this�̈Ӗ�
//	_oprtState = new OPRT_touch(this);
//#else
//	_oprtState = new OPRT_touch(this);
//#endif
	
	actModuleRegistration();

	_action_Now = ACTION::IDLE;
	_action_Old = ACTION::IDLE;

	Anim_Registration((Sprite*)this);			// �A�j���[�V�����̓o�^
}

Player::~Player()
{
	// ���I�ȃC���X�^���X��������delete��Y��Ȃ��悤��!
	delete _oprtState;
}

// ���t���[���X�V
void Player::update(float sp)
{
	_actCtl.update(sp,*this);

	// ���s������͐�(key��touch)��update�ֈړ�
	_oprtState->update();		  

	// �͈͊Ocheck
	OutOfMapCheck();	

	if (_action_Now != _action_Old)
	{
		AnimCheck(this);
	}
	_action_Old = _action_Now;
}

// �A�j���[�V�����؂�ւ��w��
void Player::AnimCheck(cocos2d::Sprite * delta)
{
	// ���݂̃A�N�V������ԂƔ�ׂāA��v���Ă�����̂ɐ؂�ւ���
	for (ACTION _act = begin(ACTION()); _act <= end(ACTION()); ++_act)
	{
		if (GetAction() == _act)
		{
			AnimMng::ChangeAnim(delta, _animTable[static_cast<int>(_act)]);
			return;							// ��v�����Ƃ��ɂ���ȏ�for�����񂷕K�v���Ȃ�����return����
		}
	}
}

// ("plist�̖��O","plist�ɂ���png�̖��O","���������O",�J�n�ԍ�, �I���ԍ�,���]���邩,�`�摬�x)
void Player::Anim_Registration(Sprite* delta)
{
	// �A�j���[�V�������L���b�V���ɓo�^
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

// ���݂̃A�N�V���������擾����
ACTION Player::GetAction(void)
{
	return _action_Now;
}

// ���݂̃A�N�V������Ԃ��Z�b�g����
void Player::SetAction(ACTION action)
{
	_action_Now = action;
}

// player��map�͈̔͊O�ɏo�Ă��Ȃ����̊m�F
void Player::OutOfMapCheck(void)
{
	auto director = Director::getInstance();
	CharCallData(director, "player");
	CollisionMapCallData(director, "MapData", "col");
	_plPos = { _player->getPosition()+ _outOfMapCheckVel };
	_ColLayerSize = _CollisionData->getLayerSize();

	// ��
	if (_plPos.y > _ColLayerSize.height * _tileChip - 50)
	{
		setPosition(getPosition().x , _ColLayerSize.height * _tileChip - 50);
	}

	// ��
	if (_plPos.y < 90)
	{
		setPosition(getPosition() + Vec2{ 0.0f,5.0f });
	}

	// ��
	if (_plPos.x < 40)
	{
		setPosition(getPosition() - _outOfMapCheckVel+ Vec2{ 5.0f,0.0f });
	}

	// �E
	if (_plPos.x > _ColLayerSize.width  * _tileChip - 70)
	{
		setPosition(getPosition() - _outOfMapCheckVel - Vec2{ 5.0f,0.0f });
	}
}

// player�̏��
void Player::CharCallData(cocos2d::Director * director, const char * charName)
{
	// ���̒H���(Director->GameScene->CharLayer->PlSprite)
	_player = (TMXLayer*)director->getRunningScene()->getChildByName("CHAR_LAYER")->getChildByName(charName);
}

// �����蔻�肵����map�̏��
void Player::CollisionMapCallData(cocos2d::Director * director, const char * mapName, const char * collisionName)
{
	// ���̒H���(Director->GameScene->bgLayer->TMX��mapData->����ɂ��̒���collision���C���[)
	_CollisionData = (TMXLayer*)director->getRunningScene()->getChildByName("BG_BACK")->getChildByName(mapName)->getChildByName(collisionName);
}

void Player::actModuleRegistration(void)
{
	// �E�ړ�
	{
		ActModule act;
		act.state = _oprtState;
		act.vel = Vec2{ 5,0 };
		act.action = ACTION::RUN;
		act.button = BUTTON::RIGHT;
		//act.checkPoint1 = Vec2{ 30, 40 };		// �E��
		//act.checkPoint2 = Vec2{ 30,-55 };		// �E��
		act.checkPoint1 = Vec2{ 30, 40 };		// �E��
		act.checkPoint2 = Vec2{ 30,  -10 };		// �E��
		act.touch = TOUCH_TIMMING::TOUCHING;	// �������ςȂ�
		//act.blackList.emplace_back(ACTION::FALL);	// �������ɉE�ړ����Ăق����Ȃ��Ƃ��̒ǉ��̎d��
		_actCtl.ActCtl("�E�ړ�", act);
	}

	// ���ړ�
	{
		ActModule act;
		act.state = _oprtState;
		act.vel = Vec2{ -5,0 };
		act.action = ACTION::RUN;
		act.button = BUTTON::LEFT;
		//act.checkPoint1 = Vec2{ -30, 40 };		// ����
		//act.checkPoint2 = Vec2{ -30,-55 };		// ����
		act.checkPoint1 = Vec2{ -30, 40 };			// ����
		act.checkPoint2 = Vec2{ -30,  -10 };		// ����
		act.touch = TOUCH_TIMMING::TOUCHING;    // �������ςȂ�
		//act.blackList.emplace_back(ACTION::FALL);
		_actCtl.ActCtl("���ړ�", act);
	}

	// �E�������]
	{
		ActModule flipAct;
		flipAct.state = _oprtState;
		flipAct.flipFlg = false;
		flipAct.action = ACTION::NON;
		flipAct.button = BUTTON::RIGHT;
		flipAct.touch = TOUCH_TIMMING::TOUCHING; // �������ςȂ�
		//flipAct.blackList.emplace_back(ACTION::FALL);
		_actCtl.ActCtl("�E����", flipAct);
	}

	// ���������]
	{
		ActModule flipAct;
		flipAct.state = _oprtState;
		flipAct.flipFlg = true;
		flipAct.action = ACTION::NON;
		flipAct.button = BUTTON::LEFT;
		flipAct.touch = TOUCH_TIMMING::TOUCHING; // �������ςȂ�
		//flipAct.blackList.emplace_back(ACTION::FALL);
		_actCtl.ActCtl("������", flipAct);
	}

	// ����
	{
		ActModule act;
		act.action = ACTION::FALLING;
		//act.checkPoint1 = Vec2{ 0,-70 };		// ����
		//act.checkPoint2 = Vec2{ 0,-70 };		// �E��
		act.checkPoint1 = Vec2{ 0,-30 };		// ����
		act.checkPoint2 = Vec2{ 0,-30 };		// �E��
		act.gravity = Vec2{ 0.0f,-5.0f };
		act.touch = TOUCH_TIMMING::RELEASED;	// �����Ɨ����Ă���
		act.blackList.emplace_back(ACTION::JUMPING);	// �W�����v���ɗ������Ăق����Ȃ�
		_actCtl.ActCtl("����", act);
	}

	// �W�����v
	{
		ActModule act;
		act.state = _oprtState;
		act.button = BUTTON::UP;
		act.action = ACTION::JUMPING;
		//act.checkPoint1 = Vec2{ 0, 10 };		// ����
		//act.checkPoint2 = Vec2{ 0, 10 };		// �E��
		act.checkPoint1 = Vec2{ -10, 30 };		// ����
		act.checkPoint2 = Vec2{ +10, 30 };		// �E��
		act.jumpVel = Vec2{ 0.0f,20.0f };
		act.touch = TOUCH_TIMMING::TOUCHING;	// �������u��
		act.blackList.emplace_back(ACTION::FALLING);	// �������ɃW�����v���Ăق����Ȃ�
		_actCtl.ActCtl("�W�����v", act);
	}
}