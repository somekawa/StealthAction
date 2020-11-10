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
	// ��̂��Z������version��
	//setPosition(Vec2{ Vec2(visibleSize) / 2 + origin - Vec2(0,-200) });

	// �L�[���͂��^�b�`���삩���f
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	// this�̈Ӗ�
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

	Anim_Registration((Sprite*)this);			// �A�j���[�V�����̓o�^

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
	//TRACE("%f,%f\n",getPosition().x, getPosition().y);
	Action();

	_actCtl.update(sp,*this);

	// ���s������͐�(key��touch)��update�ֈړ�
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
	//TRACE("%f\n", plpos.y);
	//TRACE("%d\n", _action_Now);
	// �͈͊Ocheck
	//OutOfMapCheck();	

	if (actionNow_ == "Wall_Slide")
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
	// flag��true�̎��͋����I��AttackSecond�֐ؑ�
	if (attackflg)
	{
		actionNow_ = "AttackSecond";
	}

	if (actionNow_ == "AttackFirst" || actionOld_ == "AttackFirst")
	{
		// �t���[�����̎擾�e�X�g
		//auto a = cntTest * 100;
		//auto b = 0.05 * 100;
		//auto c = (int)a / (int)b;
		//TRACE("%d\n", c);

		// �U�����ɂ�����x�U���{�^���������ꂽ��AttackFirst���I����AAttackSecond�ֈڍs����悤�ɂ���
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
				// �A���J�[�|�C���g�E�[
				this->setAnchorPoint(Vec2(1.0f, 0.0f));
			}
			else if (_dir_Now == DIR::RIGHT)
			{
				// �A���J�[�|�C���g���[
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

			// HP�����̃e�X�g�R�[�h
			// �U�����邽�т�HP��10����悤�ɂ��Ă���
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
				// �A���J�[�|�C���g�E�[
				this->setAnchorPoint(Vec2(1.0f, 0.0f));
				// ���ݍ��݈ړ�
				this->runAction(cocos2d::MoveTo::create(0.0f, cocos2d::Vec2(oldPos_ - 30, this->getPosition().y)));
				this->setPosition(Vec2(oldPos_ - 30, this->getPosition().y));
			}
			else if (_dir_Now == DIR::RIGHT)
			{
				// �A���J�[�|�C���g���[
				this->setAnchorPoint(Vec2(0.0f, 0.0f));
				// ���ݍ��݈ړ�
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

// ("plist�̖��O","plist�ɂ���png�̖��O","���������O",�J�n�ԍ�, �I���ԍ�,���]���邩,�`�摬�x)
void Player::Anim_Registration(Sprite* delta)
{
	// �A�j���[�V�������L���b�V���ɓo�^
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

// ���݂̃A�N�V���������擾����
std::string Player::GetAction(void)
{
	return actionNow_;
}

// ���݂̃A�N�V������Ԃ��Z�b�g����
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
		act.actName = "Run";
		act.button = BUTTON::RIGHT;
		act.checkPoint1 = Vec2{ charSize.x/2, charSize.y/2 };	// �E��
		act.checkPoint2 = Vec2{ charSize.x/2,  15 };			// �E��
		act.touch = TOUCH_TIMMING::TOUCHING;	// �������ςȂ�
		act.jumpFlg = false;
		//act.blackList.emplace_back(ACTION::FALLING);	// �������ɉE�ړ����Ăق����Ȃ��Ƃ��̒ǉ��̎d��

		//act.whiteList.emplace_back(ACTION::JUMPING);
		act.blackList.emplace_back("AttackFirst");
		act.blackList.emplace_back("AttackSecond");
		act.blackList.emplace_back("Wall_Slide");
		_actCtl.ActCtl("�E�ړ�", act);
	}

	// ���ړ�
	{
		ActModule act;
		act.state = _oprtState;
		act.vel = Vec2{ -5,0 };
		act.actName = "Run";
		act.button = BUTTON::LEFT;
		act.checkPoint1 = Vec2{ -charSize.x/2, charSize.y/2 };	// ����
		act.checkPoint2 = Vec2{ -charSize.x/2,  15 };			// ����
		act.touch = TOUCH_TIMMING::TOUCHING;    // �������ςȂ�
		act.jumpFlg = false;

		//act.blackList.emplace_back(ACTION::FALLING);

		//act.whiteList.emplace_back(ACTION::JUMPING);
		act.blackList.emplace_back("AttackFirst");
		act.blackList.emplace_back("AttackSecond");
		act.blackList.emplace_back("Wall_Slide");
		_actCtl.ActCtl("���ړ�", act);
	}

	// �E�������]
	{
		ActModule flipAct;
		flipAct.state = _oprtState;
		flipAct.flipFlg = false;
		flipAct.actName = "NON";
		flipAct.button = BUTTON::RIGHT;
		flipAct.touch = TOUCH_TIMMING::TOUCHING; // �������ςȂ�
		flipAct.jumpFlg = false;

		//flipAct.blackList.emplace_back(ACTION::FALLING);

		flipAct.blackList.emplace_back("AttackFirst");
		flipAct.blackList.emplace_back("AttackSecond");
		flipAct.blackList.emplace_back("Wall_Slide");
		_actCtl.ActCtl("�E����", flipAct);
	}

	// ���������]
	{
		ActModule flipAct;
		flipAct.state = _oprtState;
		flipAct.flipFlg = true;
		flipAct.actName = "NON";
		flipAct.button = BUTTON::LEFT;
		flipAct.touch = TOUCH_TIMMING::TOUCHING; // �������ςȂ�
		flipAct.jumpFlg = false;

		//flipAct.blackList.emplace_back(ACTION::FALLING);

		flipAct.blackList.emplace_back("AttackFirst");
		flipAct.blackList.emplace_back("AttackSecond");
		flipAct.blackList.emplace_back("Wall_Slide");
		_actCtl.ActCtl("������", flipAct);
	}

	// ����
	{
		// checkkeylist�ɗ����Ă���Ԃ̐ݒ���������Ǔ��Ɍ��ʂȂ�
		ActModule act;
		act.actName = "Fall";
		act.state = _oprtState;
		act.button = BUTTON::DOWN;
		//act.checkPoint1 = Vec2{ 0,-10 };			// ����
		//act.checkPoint2 = Vec2{ 0,-10 };			// �E��
		act.checkPoint1 = Vec2{ 0,0 };				// ����
		act.checkPoint2 = Vec2{ 0,0 };				// �E��

		act.checkPoint3 = Vec2{ charSize.x / 2, charSize.y / 2 };  // �E��
		act.checkPoint4 = Vec2{ -charSize.x / 2, charSize.y / 2 }; // ����

		act.gravity = Vec2{ 0.0f,-5.0f };
		act.touch = TOUCH_TIMMING::RELEASED;	// �����Ɨ����Ă���
		act.jumpFlg = false;
		act.blackList.emplace_back("Jumping");	// �W�����v���ɗ������Ăق����Ȃ�
		act.blackList.emplace_back("Wall_Slide");	
		//act.blackList.emplace_back(ACTION::JUMP);	// �W�����v���ɗ������Ăق����Ȃ�

		_actCtl.ActCtl("����", act);
	}

	// �W�����v
	{
		ActModule act;
		act.state = _oprtState;
		act.button = BUTTON::UP;
		act.actName = "Jump";
		act.checkPoint1 = Vec2{ -charSize.x/3 + 5, charSize.y };		// ����
		act.checkPoint2 = Vec2{ charSize.x/3 - 5, charSize.y };			// �E��
		//act.checkPoint1 = Vec2{ -10, 30 };						// ����
		//act.checkPoint2 = Vec2{ +10, 30 };						// �E��
		act.jumpVel = Vec2{ 0.0f,20.0f };
		act.touch = TOUCH_TIMMING::ON_TOUCH;	// �������u��

		// ������R�����g�A�E�g���Ă���ƁA���E�����Ȃ���̃W�����v���ł���
		// �ł��A���ŃW�����v���ď㏸��������悤�ɂȂ�
		// ������FALL��JUMP���������č������o�Ȃ�
		act.blackList.emplace_back("Fall");	// �������ɃW�����v���Ăق����Ȃ�
		act.jumpFlg = true;
		act.blackList.emplace_back("AttackFirst");
		act.blackList.emplace_back("AttackSecond");

		//act.whiteList.emplace_back(ACTION::RUN);

		_actCtl.ActCtl("�W�����v", act);
	}

	// �W�����s���O
	{
		ActModule act;
		act.state = _oprtState;
		act.button = BUTTON::UP;
		act.actName = "Jumping";
		act.checkPoint1 = Vec2{ -charSize.x/3 + 5, charSize.y };	// ����
		act.checkPoint2 = Vec2{ charSize.x/3 - 5, charSize.y };		// �E��
		//act.checkPoint1 = Vec2{ -10, 30 };					// ����
		//act.checkPoint2 = Vec2{ +10, 30 };					// �E��
		act.jumpVel = Vec2{ 0.0f,20.0f };
		act.touch = TOUCH_TIMMING::TOUCHING;	// �������ςȂ�
		act.jumpFlg = true;

		act.blackList.emplace_back("Fall");	// �������ɃW�����v���Ăق����Ȃ�
		act.blackList.emplace_back("Look_Intro");
		act.blackList.emplace_back("Run");
		act.blackList.emplace_back("AttackFirst");
		act.blackList.emplace_back("AttackSecond");
		act.blackList.emplace_back("NON");
		act.blackList.emplace_back("Wall_Slide");

		act.whiteList.emplace_back("Jump");
		_actCtl.ActCtl("�W�����s���O", act);
		//_actCtl.ActCtl("�W�����v", act);
	}

	// �U��
	{
		ActModule act;
		act.state = _oprtState;
		//act.button = BUTTON::ATTACK;
		act.button = BUTTON::DOWN;
		act.actName = "AttackFirst";
		//act.checkPoint1 = Vec2{ 0, 0 };		
		//act.checkPoint2 = Vec2{ 0, 0 };
		act.touch = TOUCH_TIMMING::ON_TOUCH;	// �������u��
		_actCtl.ActCtl("�U��", act);
	}

	// �U��2
	{
		ActModule act;
		act.state = _oprtState;
		act.button = BUTTON::DOWN;
		act.actName = "AttackSecond";
		act.touch = TOUCH_TIMMING::ON_TOUCH;	// �������u��
		_actCtl.ActCtl("�U��", act);
	}

	// �E�ǃX���C�h
	{
		ActModule act;
		act.state = _oprtState;
		act.gravity = Vec2{ 0.0f,-1.0f };
		act.checkPoint1 = Vec2{ charSize.x / 2, charSize.y / 2 };	// �E��
		act.checkPoint2 = Vec2{ charSize.x / 2, 0 };				// �E��
		act.button = BUTTON::RIGHT;
		act.flipFlg = true;
		act.actName = "Wall_Slide";
		act.touch = TOUCH_TIMMING::TOUCHING;	
		act.blackList.emplace_back("Jumping");	// �W�����v���ɗ������Ăق����Ȃ�
		act.blackList.emplace_back("Run");	
		act.blackList.emplace_back("Fall");
		act.blackList.emplace_back("Look_Intro");
		act.blackList.emplace_back("NON");
		act.num = 95;
		_actCtl.ActCtl("�E�ǃX���C�h", act);
	}

	// ���ǃX���C�h
	{
		ActModule act;
		act.state = _oprtState;
		act.gravity = Vec2{ 0.0f,-1.0f };
		act.checkPoint1 = Vec2{-charSize.x / 2, charSize.y / 2 };	// ����
		act.checkPoint2 = Vec2{-charSize.x / 2, 0 };				// ����
		act.button = BUTTON::LEFT;
		act.flipFlg = false;
		act.actName = "Wall_Slide";
		act.touch = TOUCH_TIMMING::TOUCHING;
		act.blackList.emplace_back("Jumping");	// �W�����v���ɗ������Ăق����Ȃ�
		act.blackList.emplace_back("Run");
		act.blackList.emplace_back("Fall");
		act.blackList.emplace_back("Look_Intro");
		act.blackList.emplace_back("NON");
		act.num = 95;
		_actCtl.ActCtl("���ǃX���C�h", act);
	}
}