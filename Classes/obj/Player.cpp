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
	// ��̂��Z������version��
	setPosition( Vec2{ Vec2(visibleSize) / 2 + origin - Vec2(0,-200)} );

	// �L�[���͂��^�b�`���삩���f
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	// this�̈Ӗ�
	_oprtState = new OPRT_touch(this);
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
	_dir_Now = DIR::RIGHT;

	_attackCheckL = Vec2(0, 0);
	_attackCheckR = Vec2(0, 0);

	Anim_Registration((Sprite*)this);			// �A�j���[�V�����̓o�^

	myNo_ = no_;
	no_++;
	type_ = ActorType::Player;
}

Player::~Player()
{
	// ���I�ȃC���X�^���X��������delete��Y��Ȃ��悤��!
	delete _oprtState;
}

// ���t���[���X�V
void Player::update(float sp)
{
	// getname��gamescene�łȂ��ꍇ�A�������Ȃ�return����
	if (Director::getInstance()->getRunningScene()->getName() != "GameScene")
	{
		return;
	}
	
	Action();

	_actCtl.update(sp,*this);

	// ���s������͐�(key��touch)��update�ֈړ�
	_oprtState->update();	

	// A�L�[�ōU�����[�V����--------------------

	// ActModule�ɐݒ肷��΁A�����̃L�[�ݒ�͕s�v�ɂȂ�
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
				// �A���J�[�|�C���g�E�[
				this->setAnchorPoint(Vec2(1.0f, 0.0f));
			}
			else if (_dir_Now == DIR::RIGHT)
			{
				// �A���J�[�|�C���g���[
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
	// �����蔻����K��
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
	//		// �U�������u�Ԃɓ����蔻�������
	//		if (_action_Now == ACTION::ATTACK)
	//		{
	//			// �G��visible��false�ɂ��Č����Ȃ�����
	//			int a = 0;
	//			((Game*)Director::getInstance()->getRunningScene())->enemySprite->setVisible(false);
	//		}
	//	}
	//}


	// �G�Ƃ̓����蔻��e�X�g
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

	//		// �E�̎���offset+  ����offset-
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
	//			// �U�������u�Ԃɓ����蔻�������
	//			if (_action_Now == ACTION::ATTACK)
	//			{
	//				// �G��visible��false�ɂ��Č����Ȃ�����
	//				((Game*)Director::getInstance()->getRunningScene())->enemySprite->setVisible(false);
	//			}
	//		}
	//	}
	//}

	//auto plpos = this->getPosition();
	//TRACE("%f\n", plpos.x);
	TRACE("%d\n", _action_Now);
	// �͈͊Ocheck
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

// �A�j���[�V�����؂�ւ��w��
void Player::AnimCheck(cocos2d::Sprite * delta)
{
	//if (_action_Now == ACTION::ATTACK)
	//{
	//	AnimMng::ChangeAnim(delta, "attack");
	//	return;
	//}

	// ���݂̃A�N�V������ԂƔ�ׂāA��v���Ă�����̂ɐ؂�ւ���
	for (ACTION _act = begin(ACTION()); _act <= end(ACTION()); ++_act)
	{
		if (_action_Now == _act)
		{
			lpAnimMng.ChangeAnimation(*delta, _animTable[static_cast<int>(_act)],true,ActorType::Player);
			return;							// ��v�����Ƃ��ɂ���ȏ�for�����񂷕K�v���Ȃ�����return����
		}
	}
}

// ("plist�̖��O","plist�ɂ���png�̖��O","���������O",�J�n�ԍ�, �I���ԍ�,���]���邩,�`�摬�x)
void Player::Anim_Registration(Sprite* delta)
{
	//// �A�j���[�V�������L���b�V���ɓo�^
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

	// �A�j���[�V�������L���b�V���ɓo�^
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

void Player::SetDir(DIR dir)
{
	_dir_Now = dir;
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
	if (_plPos.y > _ColLayerSize.height * _tileChip - (_tileChip * 2))
	{
		setPosition(getPosition().x , _ColLayerSize.height * _tileChip - (_tileChip * 2));
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
	Vec2 charSize = { 15.0f * 3.0f,25.0f * 3.0f };
	// �E�ړ�
	{
		ActModule act;
		act.state = _oprtState;
		act.vel = Vec2{ 5,0 };
		act.action = ACTION::RUN_R;
		act.button = BUTTON::RIGHT;
		act.checkPoint1 = Vec2{ charSize.x/2, charSize.y/2 };	// �E��
		act.checkPoint2 = Vec2{ charSize.x/2,  15 };			// �E��
		act.touch = TOUCH_TIMMING::TOUCHING;	// �������ςȂ�
		act.jumpFlg = false;
		//act.blackList.emplace_back(ACTION::FALLING);	// �������ɉE�ړ����Ăق����Ȃ��Ƃ��̒ǉ��̎d��

		//act.whiteList.emplace_back(ACTION::JUMPING);
		act.blackList.emplace_back(ACTION::ATTACK);
		_actCtl.ActCtl("�E�ړ�", act);
	}

	// ���ړ�
	{
		ActModule act;
		act.state = _oprtState;
		act.vel = Vec2{ -5,0 };
		act.action = ACTION::RUN_L;
		act.button = BUTTON::LEFT;
		act.checkPoint1 = Vec2{ -charSize.x/2, charSize.y/2 };	// ����
		act.checkPoint2 = Vec2{ -charSize.x/2,  15 };			// ����
		act.touch = TOUCH_TIMMING::TOUCHING;    // �������ςȂ�
		act.jumpFlg = false;

		//act.blackList.emplace_back(ACTION::FALLING);

		//act.whiteList.emplace_back(ACTION::JUMPING);
		act.blackList.emplace_back(ACTION::ATTACK);
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
		flipAct.jumpFlg = false;

		//flipAct.blackList.emplace_back(ACTION::FALLING);

		flipAct.blackList.emplace_back(ACTION::ATTACK);
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
		flipAct.jumpFlg = false;

		//flipAct.blackList.emplace_back(ACTION::FALLING);

		flipAct.blackList.emplace_back(ACTION::ATTACK);
		_actCtl.ActCtl("������", flipAct);
	}

	// ����
	{
		// checkkeylist�ɗ����Ă���Ԃ̐ݒ���������Ǔ��Ɍ��ʂȂ�
		ActModule act;
		act.action = ACTION::FALLING;
		act.state = _oprtState;
		act.button = BUTTON::DOWN;
		//act.checkPoint1 = Vec2{ 0,-10 };			// ����
		//act.checkPoint2 = Vec2{ 0,-10 };			// �E��
		act.checkPoint1 = Vec2{ 0,0 };				// ����
		act.checkPoint2 = Vec2{ 0,0 };				// �E��
		act.gravity = Vec2{ 0.0f,-5.0f };
		act.touch = TOUCH_TIMMING::RELEASED;	// �����Ɨ����Ă���
		act.jumpFlg = false;
		act.blackList.emplace_back(ACTION::JUMPING);	// �W�����v���ɗ������Ăق����Ȃ�
		//act.blackList.emplace_back(ACTION::JUMP);	// �W�����v���ɗ������Ăق����Ȃ�
		_actCtl.ActCtl("����", act);
	}

	// �W�����v
	{
		ActModule act;
		act.state = _oprtState;
		act.button = BUTTON::UP;
		act.action = ACTION::JUMP;
		act.checkPoint1 = Vec2{ -charSize.x/3, charSize.y };		// ����
		act.checkPoint2 = Vec2{ charSize.x/3, charSize.y };			// �E��
		//act.checkPoint1 = Vec2{ -10, 30 };						// ����
		//act.checkPoint2 = Vec2{ +10, 30 };						// �E��
		act.jumpVel = Vec2{ 0.0f,20.0f };
		act.touch = TOUCH_TIMMING::ON_TOUCH;	// �������u��

		// ������R�����g�A�E�g���Ă���ƁA���E�����Ȃ���̃W�����v���ł���
		// �ł��A���ŃW�����v���ď㏸��������悤�ɂȂ�
		// ������FALL��JUMP���������č������o�Ȃ�
		act.blackList.emplace_back(ACTION::FALLING);	// �������ɃW�����v���Ăق����Ȃ�
		act.jumpFlg = true;
		act.blackList.emplace_back(ACTION::ATTACK);

		//act.whiteList.emplace_back(ACTION::RUN);

		_actCtl.ActCtl("�W�����v", act);
	}

	// �W�����s���O
	{
		ActModule act;
		act.state = _oprtState;
		act.button = BUTTON::UP;
		act.action = ACTION::JUMPING;
		act.checkPoint1 = Vec2{ -charSize.x/3, charSize.y };	// ����
		act.checkPoint2 = Vec2{ charSize.x/3, charSize.y };		// �E��
		//act.checkPoint1 = Vec2{ -10, 30 };					// ����
		//act.checkPoint2 = Vec2{ +10, 30 };					// �E��
		act.jumpVel = Vec2{ 0.0f,20.0f };
		act.touch = TOUCH_TIMMING::TOUCHING;	// �������ςȂ�
		act.jumpFlg = true;

		act.blackList.emplace_back(ACTION::FALLING);	// �������ɃW�����v���Ăق����Ȃ�
		act.blackList.emplace_back(ACTION::IDLE);
		act.blackList.emplace_back(ACTION::RUN_L);
		act.blackList.emplace_back(ACTION::RUN_R);
		act.blackList.emplace_back(ACTION::ATTACK);
		act.blackList.emplace_back(ACTION::NON);

		act.whiteList.emplace_back(ACTION::JUMP);

		_actCtl.ActCtl("�W�����s���O", act);
		//_actCtl.ActCtl("�W�����v", act);
	}

	// �U��
	{
		ActModule act;
		act.state = _oprtState;
		//act.button = BUTTON::ATTACK;
		act.button = BUTTON::DOWN;
		act.action = ACTION::ATTACK;
		//act.checkPoint1 = Vec2{ 0, 0 };		
		//act.checkPoint2 = Vec2{ 0, 0 };
		act.touch = TOUCH_TIMMING::ON_TOUCH;	// �������u��
		_actCtl.ActCtl("�U��", act);
	}
}