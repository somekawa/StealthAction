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

Player::Player()
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
	direction_ = Direction::Right;

	// �A�j���[�V�����̓o�^
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

	// �t�@�C������ύX���Ă���Load���g����悤�ɂ��Ȃ��Ƃ����Ȃ�
	//for (auto anim : lpAnimMng.GetAnimations(type_))
	//{
	//	lpCol.Load(collider_, anim, "player");
	//}

	// �����ǉ����悤�Ƃ���ƁATiledMap�̃N���X�ŃG���[���ł�
	//for (auto anim : lpAnimMng.GetAnimations(type_))
	//{
	//	lpCol.Load(collider_, anim, "player");
	//	auto a = lpAnimMng.GetFrameNum(type_, anim);
	//	for (int i = 0; i < lpAnimMng.GetFrameNum(type_, anim); i++)
	//	{
	//		for (auto c : collider_[anim][i])
	//		{
	//			auto col = c->CreateCollider();
	//			col->drawRect(Vec2(0, 0), Vec2(c->GetSize().x, c->GetSize().y), c->GetColor());
	//			col->setName(anim);
	//			this->addChild(col);
	//		}
	//	}
	//}

	// �����ǉ�����ƑS���̃R���W�����{�b�N�X���łĂ���!!
	//for (auto anim : lpAnimMng.GetAnimations(type_))
	//{
	//	// colliderBox��Load
	//	lpCol.Load(collider_, anim, "player");
	//	for (auto col : collider_[anim])
	//	{
	//		for (int colNum = 0; colNum < col.size(); colNum++)
	//		{
	//			// colliderBox�����g�̎q�ɂ���
	//			auto draw = col[colNum]->create();
	//			draw->setContentSize(Size{ (float)col[colNum]->GetData().size_.x,(float)col[colNum]->GetData().size_.y });
	//			draw->drawRect(Vec2(0, 0), Vec2{ (float)col[colNum]->GetData().size_.x,(float)col[colNum]->GetData().size_.y }, col[colNum]->GetColor());
	//			draw->setTag(col[colNum]->GetData().frame_);
	//			this->addChild(draw, 0, anim);
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
		if (direction_ == Direction::Left)
		{
			// �A���J�[�|�C���g�E�[
			this->setAnchorPoint(Vec2(1.0f, 0.0f));
		}
		else if (direction_ == Direction::Right)
		{
			// �A���J�[�|�C���g���[
			this->setAnchorPoint(Vec2(0.0f, 0.0f));
		}
	}
	else
	{
		this->setAnchorPoint(Vec2(0.5f, 0.0f));
	}

	attackMotion(sp);

	// �g�����X�t�H�[��
	if (_oprtState->GetNowData()[static_cast<int>(BUTTON::Transfrom)] && !_oprtState->GetOldData()[static_cast<int>(BUTTON::Transfrom)])
	{
		this->setPosition(this->getPosition().x, this->getPosition().y - 10);	// �ʒu�␳�����Ȃ��ƕ����Č�����
		actionNow_ = "Transform";
		bitFlg_.TransfromFlg = true;
	}
	if (bitFlg_.TransfromFlg)
	{
		animationFrame_ += sp;
		if (animationFrame_ >= 1.85f)
		{
			bitFlg_.TransfromFlg = false;
			animationFrame_ = 0.0f;
			this->setPosition(this->getPosition().x, this->getPosition().y + 10);	// �ʒu�␳���߂�
		}
		else
		{
			actionNow_ = "Transform";
		}
	}

	if (actionNow_ != actionOld_)
	{
		if (actionNow_ == "AttackFirst" && !bitFlg_.FirstAttackFlg)
		{
			bitFlg_.FirstAttackFlg = true;
		}
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
	auto moveLambda = [&](int sign) {
		//if (direction_ == Direction::Left)
		//{
		//	// �A���J�[�|�C���g�E�[
		//	//this->setAnchorPoint(Vec2(1.0f, 0.0f));
		//	this->runAction(cocos2d::MoveTo::create(0.0f, cocos2d::Vec2(oldPos_ - AttackMove, this->getPosition().y)));
		//	this->setPosition(Vec2(oldPos_ - AttackMove, this->getPosition().y));
		//}
		//else if (direction_ == Direction::Right)
		//{
		//	// �A���J�[�|�C���g���[
		//	//this->setAnchorPoint(Vec2(0.0f, 0.0f));
		//	this->runAction(cocos2d::MoveTo::create(0.0f, cocos2d::Vec2(oldPos_ + AttackMove, this->getPosition().y)));
		//	this->setPosition(Vec2(oldPos_ + AttackMove, this->getPosition().y));
		//}

		// �����ŕǂ��蔲���h�~������
		auto director = Director::getInstance();
		auto CollisionData = (TMXLayer*)director->getRunningScene()->getChildByTag((int)zOlder::BG)->getChildByName("MapData")->getChildByName("col");
		auto ColSize = CollisionData->getLayerSize();
		auto plCheckPoint1 = Vec2(oldPos_ + (AttackMove * sign), this->getPosition().y + 37);	// �����̏c�T�C�Y/2�����Z���Ă���(�A���J�[�|�C���g������������)
		auto plCheckPoint1Pos = Vec2(plCheckPoint1.x / 48, ColSize.height - (plCheckPoint1.y / 48));
		// �͈͊Ocheck(���߂�check��)
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
		// ���ł�true�ɂȂ��Ă���true�Ŕ�����
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

	// flag��true�̎��͋����I��AttackSecond�֐ؑ�
	if (bitFlg_.FirstAttackFlg)
	{
		actionNow_ = "AttackFirst";
	}
	else if (bitFlg_.SecondAttackFlg)
	{
		actionNow_ = "AttackSecond";
	}
	else if (bitFlg_.ThirdAttackFlg)
	{
		actionNow_ = "AttackThird";
	}

	if (actionNow_ == "AttackFirst" && bitFlg_.FirstAttackFlg)
	{
		// �t���[�����̎擾�e�X�g
		//auto a = animationFrame_ * 100;
		//auto b = 0.05 * 100;
		//auto c = (int)a / (int)b;
		//TRACE("%d\n", c);

		// �U�����ɂ�����x�U���{�^���������ꂽ��AttackFirst���I����AAttackSecond�ֈڍs����悤�ɂ���
		//auto keyN = _oprtState->GetNowData();
		//auto keyO = _oprtState->GetOldData();
		//if (keyN[1] && !keyO[1])
		//{
		//	if (!SecondAttackFlg_)
		//	{
		//		SecondAttackFlg_ = true;
		//	}
		//}

		bitFlg_.SecondAttackFlg = keyLambda(bitFlg_.SecondAttackFlg);

		animationFrame_ += sp;
		if (bitFlg_.FirstAttackFlg && animationFrame_ <= 0.5f)
		{
			if (!oldPosKeepFlg_)
			{
				oldPos_ = this->getPosition().x;
				oldPosKeepFlg_ = true;
				direction_ == Direction::Left ? moveLambda(-1) : moveLambda(1);
			}
			auto a = sizeof(bitFlg_);
			actionNow_ = "AttackFirst";
		}
		else
		{
			if (!bitFlg_.SecondAttackFlg)
			{
				actionNow_ = "Look_Intro";
			}
			else
			{
				actionNow_ = "AttackSecond";
				oldPos_ = this->getPosition().x;
			}
			animationFrame_ = 0.0f;
			oldPosKeepFlg_ = false;
			bitFlg_.FirstAttackFlg = false;

			// HP�����̃e�X�g�R�[�h
			// �U�����邽�т�HP��10����悤�ɂ��Ă���
			auto a = ((Game*)Director::getInstance()->getRunningScene());
			auto b = (PL_HPgauge*)a->getChildByTag((int)zOlder::FRONT)->getChildByName("PL_HPgauge");
			b->SetHP(b->GetHP() - 10);
		}
	}

	if ((actionNow_ == "AttackSecond" && bitFlg_.SecondAttackFlg))
	{
		// �U�����ɂ�����x�U���{�^���������ꂽ��AttackSecond���I����AAttackThird�ֈڍs����悤�ɂ���
		//auto keyN = _oprtState->GetNowData();
		//auto keyO = _oprtState->GetOldData();
		//if (keyN[1] && !keyO[1])
		//{
		//	if (!ThirdAttackFlg_)
		//	{
		//		ThirdAttackFlg_ = true;
		//	}
		//}

		bitFlg_.ThirdAttackFlg = keyLambda(bitFlg_.ThirdAttackFlg);

		animationFrame_ += sp;
		if (bitFlg_.SecondAttackFlg && animationFrame_ <= 0.8f)
		{
			direction_ == Direction::Left ? moveLambda(-1) : moveLambda(1);
			actionNow_ = "AttackSecond";
		}
		else
		{
			if (!bitFlg_.ThirdAttackFlg)
			{
				actionNow_ = "Look_Intro";
			}
			else
			{
				actionNow_ = "AttackThird";
				oldPos_ = this->getPosition().x;
			}

			bitFlg_.SecondAttackFlg = false;
			animationFrame_ = 0.0f;
		}
	}

	if ((actionNow_ == "AttackThird" && bitFlg_.ThirdAttackFlg))
	{
		animationFrame_ += sp;
		if (bitFlg_.ThirdAttackFlg && animationFrame_ <= 0.8f)
		{
			direction_ == Direction::Left ? moveLambda(-1) : moveLambda(1);
			actionNow_ = "AttackThird";
		}
		else
		{
			bitFlg_.ThirdAttackFlg = false;
			actionNow_ = "Look_Intro";
			animationFrame_ = 0.0f;
		}
	}
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
	// �A�j���[�V�������L���b�V���ɓo�^
	// non
	lpAnimMng.addAnimationCache("image/PlayerAnimationAsset/player/player", "NON", 6, (float)0.3, ActorType::Player,false);

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

	lpAnimMng.InitAnimation(*this, ActorType::Player, "NON");
}

Player* Player::CreatePlayer()
{
	Player* pRet = new(std::nothrow) Player();
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
		act.blackList.emplace_back("AttackThird");
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
		act.blackList.emplace_back("AttackThird");
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
		flipAct.blackList.emplace_back("AttackThird");
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
		flipAct.blackList.emplace_back("AttackThird");
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
		act.blackList.emplace_back("AttackThird");
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
		act.blackList.emplace_back("AttackThird");
		act.blackList.emplace_back("NON");
		act.blackList.emplace_back("Wall_Slide");
		act.blackList.emplace_back("Transform");

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
		act.blackList.emplace_back("Fall");
		act.touch = TOUCH_TIMMING::ON_TOUCH;	// �������u��
		_actCtl.ActCtl("�U��", act);
	}

	// �U��2
	{
		ActModule act;
		act.state = _oprtState;
		act.button = BUTTON::DOWN;
		act.actName = "AttackSecond";
		act.blackList.emplace_back("Fall");
		act.touch = TOUCH_TIMMING::ON_TOUCH;	// �������u��
		_actCtl.ActCtl("�U��", act);
	}

	// �U��3
	{
		ActModule act;
		act.state = _oprtState;
		act.button = BUTTON::DOWN;
		act.actName = "AttackThird";
		act.blackList.emplace_back("Fall");
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
		act.blackList.emplace_back("Jumping");	
		act.blackList.emplace_back("Run");	
		act.blackList.emplace_back("Fall");
		act.blackList.emplace_back("Look_Intro");
		act.blackList.emplace_back("NON");
		act.blackList.emplace_back("AttackFirst");
		act.blackList.emplace_back("AttackSecond");
		act.blackList.emplace_back("AttackThird");
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
		act.blackList.emplace_back("Jumping");	
		act.blackList.emplace_back("Run");
		act.blackList.emplace_back("Fall");
		act.blackList.emplace_back("Look_Intro");
		act.blackList.emplace_back("NON");
		act.blackList.emplace_back("AttackFirst");
		act.blackList.emplace_back("AttackSecond");
		act.blackList.emplace_back("AttackThird");
		act.num = 95;
		_actCtl.ActCtl("���ǃX���C�h", act);
	}
}

// �A���J�[�|�C���g��ύX�����ɍU�����[�V�����ǉ����Ȃ��Ƃ����Ȃ�