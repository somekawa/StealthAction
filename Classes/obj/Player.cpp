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

	currentAnimation_ = "player_Non";
	actionOld_ = "player_Run";
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

	for (auto anim : lpAnimMng.GetAnimations(type_))
	{
		// colliderBox��Load
		lpCol.Load(collider_, anim);
		for (auto col : collider_[anim])
		{
			for (int colNum = 0; colNum < col.size(); colNum++)
			{
				// colliderBox�����g�̎q�ɂ���
				auto draw = col[colNum]->create();
				draw->setContentSize(Size{ (float)col[colNum]->GetData().size_.x,(float)col[colNum]->GetData().size_.y });
				draw->drawRect(Vec2(0, 0), Vec2{ (float)col[colNum]->GetData().size_.x,(float)col[colNum]->GetData().size_.y }, col[colNum]->GetColor());
				draw->setTag(col[colNum]->GetData().frame_);
				// ������setLocalOrder�����Ȃ��Ƃ����Ȃ�
				// �U����`����0�ɁA��Ұ�ދ�`����1��
				// �Ȃ̂ŁA��������col->GetData().type_�݂����Ȃ��Ƃ����΂��ꂼ��̋�`��Zorder�ɂȂ�B
				this->addChild(draw, 0, anim);
			}
		}
	}


	// ����������player�����̒i�Ɉړ������Ƃ��ɂ������Ă��镶���ɂȂ�
	// update�Ō��邾�����āA1�x�����ǂݍ��݂����ĕ�����`��ł���悤�ɂ��Ă݂�Ƃ�
	//auto label = Label::createWithTTF("Text_Yomikomitai", "fonts/Marker Felt.ttf", 24);
	//if (label == nullptr)
	//{
	//	int a = 0;
	//}
	//label->setPosition(Vec2(100,100));
	//this->addChild(label, 1);

	// �U����`�̃T�C�Y�ݒ�
	attackRect_.size_ = Size(30.0f, 30.0f);

	attackColOffset_ = 0.0f;

	this->SetIsAttacking(false);
}

Player::~Player()
{
	// ���I�ȃC���X�^���X��������delete��Y��Ȃ��悤��!
	delete _oprtState;
}

// ���t���[���X�V
void Player::update(float delta)
{
	// getname��gamescene�łȂ��ꍇ�A�������Ȃ�return����
	if (Director::getInstance()->getRunningScene()->getName() != "GameScene")
	{
		return;
	}

	// ���S��Ԃ̍X�V�Ɗm�F(gameOverAction��true�ɂȂ�����A�b�v�f�[�g������������悤�ɂ���)
	if (deathFlg_)
	{
		if (gameOverAction())
		{
			return;
		}
	}

	// ���݂̃t���[���𐮐��l�Ŏ擾
	animationFrame_int_ = GetAnimationFrameInt();
	colliderVisible();

	Action();

	_actCtl.update(delta,*this);

	// ���s������͐�(key��touch)��update�ֈړ�
	_oprtState->update();	
	
	if (currentAnimation_ == "player_Wall_Slide")
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

	// �X���C�h�I�����ɍ��E�����𔽓]������
	if (currentAnimation_ != "player_Wall_Slide" && actionOld_ == "player_Wall_Slide")
	{
		auto dir = direction_;
		direction_ == Direction::Right ? direction_ = Direction::Left : direction_ = Direction::Right;
		auto tmpdir = direction_;
	}

	if (onDamaged_)
	{
		// HP�����̃e�X�g�R�[�h
		auto a = ((Game*)Director::getInstance()->getRunningScene());
		auto b = (PL_HPgauge*)a->getChildByTag((int)zOlder::FRONT)->getChildByName("PL_HPgauge");
		b->SetHP(b->GetHP() - 10);	// -10�Ȃǂ̃_���[�W�ʂ͓G�̍U���͂ɕς���΂���
		onDamaged_ = false;

		if (b->GetHP() <= 0)
		{
			// ���S��Ԃɐ؂�ւ�
			deathFlg_ = true;
		}
	}

	//if (deathFlg_)
	//{
	//	this->setPosition(deathPos_);	// ���S�����ʒu�Őݒ肵�Ă����Ȃ��ƁA���������Ȃ�
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

	// �g�����X�t�H�[��
	if (_oprtState->GetNowData()[static_cast<int>(BUTTON::Transfrom)] && !_oprtState->GetOldData()[static_cast<int>(BUTTON::Transfrom)])
	{
		this->setPosition(this->getPosition().x, this->getPosition().y - 10);	// �ʒu�␳�����Ȃ��ƕ����Č�����
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
			this->setPosition(this->getPosition().x, this->getPosition().y + 10);	// �ʒu�␳���߂�
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
		// �t���[�����̎擾�e�X�g
		//auto a = animationFrame_ * 100;
		//auto b = 0.05 * 100;
		//auto c = (int)a / (int)b;
		//TRACE("%d\n", c);

		bitFlg_.SecondAttackFlg = keyLambda(bitFlg_.SecondAttackFlg);

		animationFrame_ += sp;

		// frame�v�Z
		animationFrame_int_ = (int)(animationFrame_ * 100) / (int)(0.05 * 100);
		if (animationFrame_int_ < 10)
		{
			// 2�t���[���ڂ�data��2����A���̂����̕Е���type:0������U����`�ɂȂ��Ă�
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
				// HP�����̃e�X�g�R�[�h
				{
					auto a = ((Game*)Director::getInstance()->getRunningScene());
					auto b = (PL_HPgauge*)a->getChildByTag((int)zOlder::FRONT)->getChildByName("PL_HPgauge");
					b->SetHP(b->GetHP() - 100);	// -10�Ȃǂ̃_���[�W�ʂ͓G�̍U���͂ɕς���΂���
					if (b->GetHP() <= 0)
					{
						// ���S��Ԃɐ؂�ւ�
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

		// frame�v�Z
		animationFrame_int_ = (int)(animationFrame_ * 100) / (int)(0.08 * 100);
		if (animationFrame_int_ < 10)
		{
			// 2�t���[���ڂ�data��2����A���̂����̕Е���type:0������U����`�ɂȂ��Ă�
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

		// frame�v�Z
		animationFrame_int_ = (int)(animationFrame_ * 100) / (int)(0.08 * 100);
		if (animationFrame_int_ < 11)
		{
			// 2�t���[���ڂ�data��2����A���̂����̕Е���type:0������U����`�ɂȂ��Ă�
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
	// �g�����X�t�H�[���̎��̓R���C�_�[�𖳎�����
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
			// �U���̎������I�t�Z�b�g���K�v
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
	// ���łɃI�t�Z�b�g�l��0�ȏ�Őݒ肳��Ă���Ƃ���true�ɂ���
	//bool tmpFlg = false;
	//if (pos > 0)
	//{
	//	tmpFlg = true;
	//}

	// �_���[�W��`�����������������ǁA�ł��Ă��Ȃ�
	// current��type�݂銴���ɂ��邽�߂�autofor���Q�Ƃŉ񂵂Ă��ʂŐݒ�ł��Ȃ�����
	// �U���̎������I�t�Z�b�g���K�v
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
				// ��`�̖��O��attack�Ȃ��
				if (col->getName() == this->GetAction())
				{
					// ��`�����߂�attack�̂�݂̂̒��o
					if (col->getLocalZOrder() == 0)
					{
						// ��`���߼޼�݂̕ύX
						// ��`���߼޼�݂�1�x�݂̂̕ύX�łȂ���
						// ���ڰщ���Ă��܂��Ƌ�`���ǂ��������̂ł����ž�Ă��Ă���B
						// ��`������Ă������A�����蔻��͖{���̈ʒu�ōs���Ă���
						col->setPosition(this->GetAttackRect().offset_.x, col->getPosition().y);
					}
				}
			}
		}

		//col->setPosition(pos, col->getPosition().y);
	}
}

// ���݂̃A�N�V���������擾����
std::string Player::GetAction(void)
{
	return currentAnimation_;
}

// ���݂̃A�N�V������Ԃ��Z�b�g����
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
	// �A�j���[�V�������L���b�V���ɓo�^
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
	// �E�ړ�
	{
		ActModule act;
		act.state = _oprtState;
		act.vel = Vec2{ 5,0 };
		act.actName = "player_Run";
		act.button = BUTTON::RIGHT;
		act.checkPoint1 = Vec2{ charSize.x/2, charSize.y/2 };	// �E��
		act.checkPoint2 = Vec2{ charSize.x/2,  15 };			// �E��
		act.touch = TOUCH_TIMMING::TOUCHING;	// �������ςȂ�
		//act.blackList.emplace_back(ACTION::FALLING);	// �������ɉE�ړ����Ăق����Ȃ��Ƃ��̒ǉ��̎d��

		//act.whiteList.emplace_back(ACTION::JUMPING);
		act.blackList.emplace_back("player_AttackFirst");
		act.blackList.emplace_back("player_AttackSecond");
		act.blackList.emplace_back("player_AttackThird");
		act.blackList.emplace_back("player_Wall_Slide");
		_actCtl.ActCtl("�E�ړ�", act);
	}

	// ���ړ�
	{
		ActModule act;
		act.state = _oprtState;
		act.vel = Vec2{ -5,0 };
		act.actName = "player_Run";
		act.button = BUTTON::LEFT;
		act.checkPoint1 = Vec2{ -charSize.x/2, charSize.y/2 };	// ����
		act.checkPoint2 = Vec2{ -charSize.x/2,  15 };			// ����
		act.touch = TOUCH_TIMMING::TOUCHING;    // �������ςȂ�

		//act.blackList.emplace_back(ACTION::FALLING);

		//act.whiteList.emplace_back(ACTION::JUMPING);
		act.blackList.emplace_back("player_AttackFirst");
		act.blackList.emplace_back("player_AttackSecond");
		act.blackList.emplace_back("player_AttackThird");
		act.blackList.emplace_back("player_Wall_Slide");
		_actCtl.ActCtl("���ړ�", act);
	}

	// �E�������]
	{
		ActModule flipAct;
		flipAct.state = _oprtState;
		flipAct.flipFlg = false;
		flipAct.actName = "player_Non";
		flipAct.button = BUTTON::RIGHT;
		flipAct.touch = TOUCH_TIMMING::TOUCHING; // �������ςȂ�

		//flipAct.blackList.emplace_back(ACTION::FALLING);

		flipAct.blackList.emplace_back("player_AttackFirst");
		flipAct.blackList.emplace_back("player_AttackSecond");
		flipAct.blackList.emplace_back("player_AttackThird");
		flipAct.blackList.emplace_back("player_Wall_Slide");
		_actCtl.ActCtl("�E����", flipAct);
	}

	// ���������]
	{
		ActModule flipAct;
		flipAct.state = _oprtState;
		flipAct.flipFlg = true;
		flipAct.actName = "player_Non";
		flipAct.button = BUTTON::LEFT;
		flipAct.touch = TOUCH_TIMMING::TOUCHING; // �������ςȂ�

		//flipAct.blackList.emplace_back(ACTION::FALLING);

		flipAct.blackList.emplace_back("player_AttackFirst");
		flipAct.blackList.emplace_back("player_AttackSecond");
		flipAct.blackList.emplace_back("player_AttackThird");
		flipAct.blackList.emplace_back("player_Wall_Slide");
		_actCtl.ActCtl("������", flipAct);
	}

	// ����
	{
		// checkkeylist�ɗ����Ă���Ԃ̐ݒ���������Ǔ��Ɍ��ʂȂ�
		ActModule act;
		act.actName = "player_Fall";
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
		act.blackList.emplace_back("player_Jumping");	// �W�����v���ɗ������Ăق����Ȃ�
		act.blackList.emplace_back("player_Wall_Slide");	
		//act.blackList.emplace_back(ACTION::JUMP);	// �W�����v���ɗ������Ăق����Ȃ�

		_actCtl.ActCtl("����", act);
	}

	// �W�����v
	{
		ActModule act;
		act.state = _oprtState;
		act.button = BUTTON::UP;
		act.actName = "player_Jump";
		act.checkPoint1 = Vec2{ -charSize.x/3 + 5, charSize.y };		// ����
		act.checkPoint2 = Vec2{ charSize.x/3 - 5, charSize.y };			// �E��
		//act.checkPoint1 = Vec2{ -10, 30 };						// ����
		//act.checkPoint2 = Vec2{ +10, 30 };						// �E��
		act.jumpVel = Vec2{ 0.0f,20.0f };
		act.touch = TOUCH_TIMMING::ON_TOUCH;	// �������u��

		// ������R�����g�A�E�g���Ă���ƁA���E�����Ȃ���̃W�����v���ł���
		// �ł��A���ŃW�����v���ď㏸��������悤�ɂȂ�
		// ������FALL��JUMP���������č������o�Ȃ�
		act.blackList.emplace_back("player_Fall");	// �������ɃW�����v���Ăق����Ȃ�
		act.blackList.emplace_back("player_AttackFirst");
		act.blackList.emplace_back("player_AttackSecond");
		act.blackList.emplace_back("player_AttackThird");
		//act.whiteList.emplace_back(ACTION::RUN);

		_actCtl.ActCtl("�W�����v", act);
	}

	// �W�����s���O
	{
		ActModule act;
		act.state = _oprtState;
		act.button = BUTTON::UP;
		act.actName = "player_Jumping";
		act.checkPoint1 = Vec2{ -charSize.x/3 + 5, charSize.y };	// ����
		act.checkPoint2 = Vec2{ charSize.x/3 - 5, charSize.y };		// �E��
		//act.checkPoint1 = Vec2{ -10, 30 };					// ����
		//act.checkPoint2 = Vec2{ +10, 30 };					// �E��
		act.jumpVel = Vec2{ 0.0f,20.0f };
		act.touch = TOUCH_TIMMING::TOUCHING;	// �������ςȂ�

		act.blackList.emplace_back("player_Fall");	// �������ɃW�����v���Ăق����Ȃ�
		act.blackList.emplace_back("player_Look_Intro");
		act.blackList.emplace_back("player_Run");
		act.blackList.emplace_back("player_AttackFirst");
		act.blackList.emplace_back("player_AttackSecond");
		act.blackList.emplace_back("player_AttackThird");
		act.blackList.emplace_back("player_Non");
		act.blackList.emplace_back("player_Wall_Slide");
		act.blackList.emplace_back("player_Transform");

		act.whiteList.emplace_back("player_Jump");
		_actCtl.ActCtl("�W�����s���O", act);
		//_actCtl.ActCtl("�W�����v", act);
	}

	// �U��
	{
		ActModule act;
		act.state = _oprtState;
		//act.button = BUTTON::ATTACK;
		act.button = BUTTON::DOWN;
		act.actName = "player_AttackFirst";
		//act.checkPoint1 = Vec2{ 0, 0 };		
		//act.checkPoint2 = Vec2{ 0, 0 };
		//act.blackList.emplace_back("Fall");	// �ꎞ�I�R�����g�A�E�g
		act.touch = TOUCH_TIMMING::ON_TOUCH;	// �������u��
		_actCtl.ActCtl("�U��", act);
	}

	// �U��2
	{
		ActModule act;
		act.state = _oprtState;
		act.button = BUTTON::DOWN;
		act.actName = "player_AttackSecond";
		act.blackList.emplace_back("player_Fall");
		act.touch = TOUCH_TIMMING::ON_TOUCH;	// �������u��
		_actCtl.ActCtl("�U��", act);
	}

	// �U��3
	{
		ActModule act;
		act.state = _oprtState;
		act.button = BUTTON::DOWN;
		act.actName = "player_AttackThird";
		act.blackList.emplace_back("player_Fall");
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
		_actCtl.ActCtl("���ǃX���C�h", act);
	}
}

bool Player::gameOverAction(void)
{
	if (!_gameOverFlg)
	{
		// ���̓������~�߂�
		stopAllActions();

		this->setPosition(deathPos_);	// ���S�����ʒu�Őݒ肵�Ă����Ȃ��ƁA���������Ȃ�
		AnimationCache* animationCache = AnimationCache::getInstance();
		Animation* animation = animationCache->getAnimation("player_Death2");
		animation->setRestoreOriginalFrame(false);
		//�A�j���[�V���������s
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
		// �A�N�V�������I����Ă���
		return true;
	}
	if (_gameOverAction->isDone())
	{
		// �A�N�V�������I�����
		CC_SAFE_RELEASE_NULL(_gameOverAction);
		return true;
	}
	return false;
}


// �A���J�[�|�C���g��ύX�����ɍU�����[�V�����ǉ����Ȃ��Ƃ����Ȃ�
// ��MoveTo�g�p

// �@collider��position��ۑ����Ȃ��Ƃ����Ȃ�
// �A�ۑ������ϐ���dir����offset��������

// hp��0�ɂȂ����Ƃ��Ƀ��[�V������؂�ւ���Ƃ��낾������Ă���