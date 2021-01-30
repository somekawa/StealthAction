#include "Scene/GameScene.h"
#include "PL_HPgauge.h"
#include "Player.h"
#include "anim/ActionCtl.h"
#include "ActionRect.h"
#include "_Debug/_DebugConOut.h"
#include "Loader/CollisionLoader.h"
#include "../Skill/SkillMng.h"
#include "../Skill/SkillBase.h"
#include "../Skill/SkillCode/TestSkill.h"
#include "../Loader/FileLoder.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
#include "input/OPRT_key.h"
#else
#include "input/OPRT_touch.h"
#endif

// skillmng��SkillActivate���^�b�`���삵���Ƃ��ɌĂ�
// testskill�N���X:������skillBase_

USING_NS_CC;

int Player::no_ = 0;

Player::Player(int hp,Layer& myLayer, Layer& enemyLayer, SkillBase* skillBasePtr):
	Actor(hp,myLayer),enemyList_(enemyLayer)
{
	skillBase_ = skillBasePtr;
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// �L�[���͂��^�b�`���삩���f
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	// this�̈Ӗ�
	oprtState_ = new OPRT_key(this);
#else
	oprtState_ = new OPRT_touch(this);
#endif

	std::list<std::string> path;
	path.push_back("skill_data");
	//Actor.cpp�œǂݍ���Player�p��Skill�f�[�^�̎󂯎��
	plfile_ = fileLoad_["Player"];											// ���̒��ɍ��A�X�L���̏�񂪓ǂݍ��܂�Ă���(���O�Ƃ�)

	type_ = ActorType::Player;
	actModuleRegistration();

	currentAnimation_ = "player_Light_Non";
	actionOld_ = "player_Light_Run";
	direction_ = Direction::Right;

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

	gameOverFlg_ = false;
	deathFlg_ = false;

	for (auto anim : lpAnimMng.GetAnimations(type_))
	{
		// colliderBox��Load
		lpCol.ReadData(collider_, anim);
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
	delete oprtState_;
}

// ���t���[���X�V
void Player::update(float delta)
{
	// getname��gamescene�łȂ��ꍇ�A�������Ȃ�return����
	if (Director::getInstance()->getRunningScene()->getName() != "GameScene")
	{
		return;
	}

	// ���S��Ԃ̍X�V�Ɗm�F(deathFlg_��true�Ȃ�A�b�v�f�[�g������������悤�ɂ���)
	if (deathFlg_)
	{
		if (gameOverAction())
		{
			return;
		}
		return;
	}
	// �X�L���e�X�g�R�[�h&effect�e�X�g�R�[�h
	if (oprtState_->GetNowData()[static_cast<int>(BUTTON::Transfrom)] && !oprtState_->GetOldData()[static_cast<int>(BUTTON::Transfrom)])
	{
		auto director = Director::getInstance();
		skillSprite = (SkillBase*)director->getRunningScene()->getChildByTag((int)zOlder::FRONT)->getChildByName("skillSprite");
		//FLT_MAX : float �̍ő�̗L���l
		auto ppos = getPosition();
		auto minpos = Vec2(0,0);
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
		SkillBase* skill_t = TestSkill::CreateTestSkill(skillSprite);
		skillSprite->AddActiveSkill(skill_t);
		skillSprite->addChild(skill_t);

		lpSkillMng.SkillActivate(plfile_[0]);
	}
	if (skillSprite != nullptr)
	{
		skillSprite->RemoveActiveSkill();
	}
	// ���݂̃t���[���𐮐��l�Ŏ擾
	animationFrame_int_ = GetAnimationFrameInt();
	//currentCol_ = collider_[currentAnimation_][animationFrame_int_];

	colliderVisible();

	// ActionCtl�N���X�̍X�V
	actCtl_.update(type_,delta,*this);

	// ���͏��(key��touch)�̍X�V
	oprtState_->update();	
	
	// �X���C�h���̂݃A���J�[�|�C���g���ړ�������(���̑��͉摜�̉�����)
	if (currentAnimation_ == "player_Light_Wall_Slide")
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
		// �A���J�[�|�C���g������
		this->setAnchorPoint(Vec2(0.5f, 0.0f));
	}

	// �X���C�h�I�����ɍ��E�����𔽓]������
	if (currentAnimation_ != "player_Light_Wall_Slide" && actionOld_ == "player_Light_Wall_Slide")
	{
		auto dir = direction_;
		direction_ == Direction::Right ? direction_ = Direction::Left : direction_ = Direction::Right;
		auto tmpdir = direction_;
	}

	// �_���[�W����������ۂ̏���
	if (onDamaged_)
	{
		// HP��������
		auto nowScene = ((Game*)Director::getInstance()->getRunningScene());
		auto hpGauge = (PL_HPgauge*)nowScene->getChildByTag((int)zOlder::FRONT)->getChildByName("PL_HPgauge");
		hpGauge->SetHP(hpGauge->GetHP() - 10);	// -10�Ȃǂ̃_���[�W�ʂ͓G�̍U���͂ɕς���΂���
		onDamaged_ = false;

		// 0������������Ƀt���O��؂�ւ���
		if (hpGauge->GetHP() <= 0)
		{
			deathFlg_ = true;
			deathPos_ = getPosition();
		}
	}

	attackMotion(delta);
	transformMotion(delta);
	// �A�j���[�V�������؂�ւ��^�C�~���O�ŌĂ΂��Đ�����
	if (currentAnimation_ != actionOld_)
	{
		if (currentAnimation_ == "player_Light_AttackFirst" && !bitFlg_.FirstAttackFlg)
		{
			bitFlg_.FirstAttackFlg = true;
			// ���]�׸ނ̾��
			auto flip = false;
			if (direction_ == Direction::Right)
			{
				flip = true;
			}
			if (direction_ == Direction::Left)
			{
				flip = false;
			}

			// �U���̪�Ă̒ǉ�
			auto attackSprite = lpEffectMng.createEffect("attack", 5, 0.04f, { 55.0f, 50.0f },getPosition(),flip);
			lpEffectMng.PlayWithOnce(attackSprite, "attack");
		}
		animationFrame_int_ = 0.0f;
		lpAnimMng.ChangeAnimation(*this, currentAnimation_, true, ActorType::Player);
	}
	actionOld_ = currentAnimation_;
	// �e�ײ�ް���߼޼�݂ƻ��ނ̐ݒ�
	SetCollider();
	//TRACE("playerPos:(%f,%f)", getPosition().x, getPosition().y);
}

// Actor�ɂ��邪���g�p
void Player::Action(void)
{
	int a = 0;
}

// Actor�ɂ��邪���g�p
void Player::ChangeDirection(float delta)
{
	int a = 0;
}

void Player::attackMotion(float delta)
{
	auto keyLambda = [&](bool flag) {
		// ���ł�true�ɂȂ��Ă���true�Ŕ�����
		if (flag)
		{
			return true;
		}
		if (oprtState_->GetNowData()[1] && !oprtState_->GetOldData()[1] && !flag)
		{
			return true;
		}
		return false;
	};

	if (bitFlg_.FirstAttackFlg)
	{
		currentAnimation_ = "player_Light_AttackFirst";
		if (!oldPosKeepFlg_)
		{
			oldPos_ = this->getPosition().x;
			oldPosKeepFlg_ = true;
		}
	}
	else if (bitFlg_.SecondAttackFlg)
	{
		currentAnimation_ = "player_Light_AttackSecond";
	}
	else if (bitFlg_.ThirdAttackFlg)
	{
		currentAnimation_ = "player_Light_AttackThird";
	}

	if (currentAnimation_ == "player_Light_AttackFirst" && bitFlg_.FirstAttackFlg)
	{
		// �t���[�����̎擾�e�X�g
		//auto a = animationFrame_ * 100;
		//auto b = 0.05 * 100;
		//auto c = (int)a / (int)b;
		//TRACE("%d\n", c);

		bitFlg_.SecondAttackFlg = keyLambda(bitFlg_.SecondAttackFlg);

		// frame�v�Z
		animationFrame_ += delta;
		animationFrame_int_ = (int)(animationFrame_ * 100) / (int)(0.05 * 100);
		if (animationFrame_int_ < 10)
		{
			// 2�t���[���ڂ�data��2����A���̂����̕Е���type:0������U����`�ɂȂ��Ă�
			currentCol_ = collider_[currentAnimation_][animationFrame_int_];
			colliderVisible();
		}

		if (bitFlg_.FirstAttackFlg && animationFrame_ <= 0.5f)
		{
			currentAnimation_ = "player_Light_AttackFirst";
			isAttacking_ = true;
		}
		else
		{
			if (!bitFlg_.SecondAttackFlg)
			{
				currentAnimation_ = "player_Light_Look_Intro";
				this->SetIsAttacking(false);
				this->setPosition(oldPos_, this->getPosition().y);
			}
			else
			{
				currentAnimation_ = "player_Light_AttackSecond";
			}
			animationFrame_ = 0.0f;
			bitFlg_.FirstAttackFlg = false;
			isAttacking_ = false;
			oldPosKeepFlg_ = false;
		}
	}

	if ((currentAnimation_ == "player_Light_AttackSecond" && bitFlg_.SecondAttackFlg))
	{
		bitFlg_.ThirdAttackFlg = keyLambda(bitFlg_.ThirdAttackFlg);

		// frame�v�Z
		animationFrame_ += delta;
		animationFrame_int_ = (int)(animationFrame_ * 100) / (int)(0.08 * 100);
		if (animationFrame_int_ < 10)
		{
			// 2�t���[���ڂ�data��2����A���̂����̕Е���type:0������U����`�ɂȂ��Ă�
			currentCol_ = collider_[currentAnimation_][animationFrame_int_];
			colliderVisible();
		}

		if (bitFlg_.SecondAttackFlg && animationFrame_ <= 0.8f)
		{
			currentAnimation_ = "player_Light_AttackSecond";
			isAttacking_ = true;
		}
		else
		{
			if (!bitFlg_.ThirdAttackFlg)
			{
				currentAnimation_ = "player_Light_Look_Intro";
				this->setPosition(oldPos_, this->getPosition().y);
			}
			else
			{
				currentAnimation_ = "player_Light_AttackThird";
			}
			bitFlg_.SecondAttackFlg = false;
			isAttacking_ = false;
			animationFrame_ = 0.0f;
			//oldPosKeepFlg_ = false;
		}
	}

	if ((currentAnimation_ == "player_Light_AttackThird" && bitFlg_.ThirdAttackFlg))
	{
		// frame�v�Z
		animationFrame_ += delta;
		animationFrame_int_ = (int)(animationFrame_ * 100) / (int)(0.08 * 100);
		if (animationFrame_int_ < 11)
		{
			// 2�t���[���ڂ�data��2����A���̂����̕Е���type:0������U����`�ɂȂ��Ă�
			currentCol_ = collider_[currentAnimation_][animationFrame_int_];
			colliderVisible();
		}

		if (bitFlg_.ThirdAttackFlg && animationFrame_ <= 0.8f)
		{
			currentAnimation_ = "player_Light_AttackThird";
			isAttacking_ = true;
		}
		else
		{
			bitFlg_.ThirdAttackFlg = false;
			isAttacking_ = false;
			currentAnimation_ = "player_Light_Look_Intro";
			this->setPosition(oldPos_, this->getPosition().y);
			animationFrame_ = 0.0f;
			//oldPosKeepFlg_ = false;
		}
	}
}

void Player::transformMotion(float delta)
{
	// �g�����X�t�H�[��
	if (bitFlg_.TransfromFlg)
	{
		currentAnimation_ = "player_Light_Transform";
	}
	if (!bitFlg_.TransfromFlg)
	{
		if (oprtState_->GetNowData()[static_cast<int>(BUTTON::Transfrom)] && !oprtState_->GetOldData()[static_cast<int>(BUTTON::Transfrom)])
		{
			this->setPosition(this->getPosition().x, this->getPosition().y - 10);		// �ʒu�␳�����Ȃ��ƕ����Č�����
			currentAnimation_ = "player_Light_Transform";
			bitFlg_.TransfromFlg = true;
		}
	}
	if (bitFlg_.TransfromFlg)
	{
		animationFrame_ += delta;
		if (animationFrame_ >= 1.85f)
		{
			bitFlg_.TransfromFlg = false;
			currentAnimation_ = "player_Light_Look_Intro";
			animationFrame_ = 0.0f;
			this->setPosition(this->getPosition().x, this->getPosition().y + 10);	// �ʒu�␳���߂�
		}
		else
		{
			currentAnimation_ = "player_Light_Transform";
		}
	}
}

void Player::colliderVisible(void)
{
	// �g�����X�t�H�[���̎��̓R���C�_�[�𖳎�����
	if (currentAnimation_ == "player_Light_Transform" || currentAnimation_ == "player_Light_Death2")
	{
		return;
	}

	if (animationFrame_int_ <= 0)
	{
		animationFrame_int_ = 0;
	}
	CCLOG("plHP:%d", hp_);
	if (collider_[currentAnimation_].size() <= animationFrame_int_)
	{
		return;
	}
	currentCol_ = collider_[currentAnimation_][animationFrame_int_];
	for (auto collider : this->getChildren())
	{
		if (currentAnimation_ == collider->getName() &&
			animationFrame_int_ == collider->getTag())
		{
			// �U���̎������I�t�Z�b�g���K�v
			attackCollider("player_Light_AttackFirst", collider, attackColOffset_);
			attackCollider("player_Light_AttackSecond", collider, attackColOffset_);
			attackCollider("player_Light_AttackThird", collider, attackColOffset_);

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
	if (!oldPosKeepFlg_)
	{
		return;
	}

	// �U���̎������I�t�Z�b�g���K�v
	if (currentAnimation_ == str.c_str())
	{
		if (!this->IsAttacking())	// 2�E3���ڂ͂�����attacking��true������R���C�_�[�ʒu�̐ݒ�̂��Ȃ����ɂȂ�Ȃ�
		{
			if (direction_ == Direction::Right)
			{
				this->SetAttackOffset(Vec2(col->getPosition().x + 30.0f, col->getPosition().y));
				this->runAction(cocos2d::MoveTo::create(0.0f, cocos2d::Vec2(oldPos_ + AttackMove, this->getPosition().y)));
				this->SetIsAttacking(true);
			}
			else
			{
				this->SetAttackOffset(cocos2d::Vec2(0.0f, 0.0f));
				this->runAction(cocos2d::MoveTo::create(0.0f, cocos2d::Vec2(oldPos_ - AttackMove, this->getPosition().y)));
				this->SetIsAttacking(true);
			}

			//for (auto col : this->getChildren())
			//{
			//	// ��`�̖��O��attack�Ȃ��
			//	if (col->getName() == this->GetAction())
			//	{
			//		// ��`�����߂�attack�̂�݂̂̒��o
			//		if (col->getLocalZOrder() == 0)
			//		{
			//			// ��`���߼޼�݂̕ύX
			//			// ��`���߼޼�݂�1�x�݂̂̕ύX�łȂ���
			//			// ���ڰщ���Ă��܂��Ƌ�`���ǂ��������̂ł����ž�Ă��Ă���B
			//			// ��`������Ă������A�����蔻��͖{���̈ʒu�ōs���Ă���
			//			col->setPosition(this->GetAttackRect().offset_.x, col->getPosition().y);
			//		}
			//	}
			//}
		}
		col->setPosition(this->GetAttackRect().offset_.x, col->getPosition().y);
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
	oprtState_->KeyReset();
}

void Player::AnimRegistrator(void)
{
	std::string path = "image/PlayerAnimationAsset/player/Light/player_Light";

	// �A�j���[�V�������L���b�V���ɓo�^
	// non
	lpAnimMng.addAnimationCache(path, "Non", 6, (float)0.3, ActorType::Player,false);

	// idle
	lpAnimMng.addAnimationCache(path, "Look_Intro", 6, (float)0.3, ActorType::Player, false);

	// run
	lpAnimMng.addAnimationCache(path, "Run", 9, (float)0.08, ActorType::Player, false);

	// fall
	lpAnimMng.addAnimationCache(path, "Fall", 3, (float)1.0, ActorType::Player, false);

	// jump
	lpAnimMng.addAnimationCache(path, "Jump", 3, (float)0.05, ActorType::Player, false);

	lpAnimMng.addAnimationCache(path, "Jumping", 3, (float)0.05, ActorType::Player, false);

	// AttackFirst
	lpAnimMng.addAnimationCache(path, "AttackFirst", 9, (float)0.05, ActorType::Player, false);

	// AttackSecond
	lpAnimMng.addAnimationCache(path, "AttackSecond", 9, (float)0.08, ActorType::Player, false);

	// AttackThird
	lpAnimMng.addAnimationCache(path, "AttackThird", 10, (float)0.08, ActorType::Player, false);

	// wallslide
	lpAnimMng.addAnimationCache(path, "Wall_Slide", 3, (float)0.3, ActorType::Player, false);

	// Transform
	lpAnimMng.addAnimationCache(path, "Transform", 37, (float)0.05, ActorType::Player, false);

	// death
	lpAnimMng.addAnimationCache(path, "Death2", 10, (float)0.08, ActorType::Player, false);

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


void Player::actModuleRegistration(void)
{
	Vec2 charSize = { 15.0f * 3.0f,25.0f * 3.0f };
	// �E�ړ�
	{
		ActModule act;
		act.state = oprtState_;
		act.vel = Vec2{ 5,0 };
		act.actName = "player_Light_Run";
		act.button = BUTTON::RIGHT;
		act.checkPoint1 = Vec2{ charSize.x/2, charSize.y/2 };	// �E��
		act.checkPoint2 = Vec2{ charSize.x/2,  15 };			// �E��
		act.touch = TOUCH_TIMMING::TOUCHING;	// �������ςȂ�
		//act.blackList.emplace_back(ACTION::FALLING);	// �������ɉE�ړ����Ăق����Ȃ��Ƃ��̒ǉ��̎d��

		//act.whiteList.emplace_back(ACTION::JUMPING);
		act.blackList.emplace_back("player_Light_AttackFirst");
		act.blackList.emplace_back("player_Light_AttackSecond");
		act.blackList.emplace_back("player_Light_AttackThird");
		act.blackList.emplace_back("player_Light_Wall_Slide");
		actCtl_.RunInitializeActCtl(type_,"�E�ړ�", act);

		auto anchor1 = DrawNode::create();
		anchor1->drawDot(act.checkPoint1, 3.0f, Color4F::BLUE);
		this->addChild(anchor1, 5);
		auto anchor2 = DrawNode::create();
		anchor2->drawDot(act.checkPoint2, 3.0f, Color4F::BLUE);
		this->addChild(anchor2, 5);
	}

	// ���ړ�
	{
		ActModule act;
		act.state = oprtState_;
		act.vel = Vec2{ -5,0 };
		act.actName = "player_Light_Run";
		act.button = BUTTON::LEFT;
		act.checkPoint1 = Vec2{ -charSize.x/2, charSize.y/2 };	// ����
		act.checkPoint2 = Vec2{ -charSize.x/2,  15 };			// ����
		act.touch = TOUCH_TIMMING::TOUCHING;    // �������ςȂ�

		//act.blackList.emplace_back(ACTION::FALLING);

		//act.whiteList.emplace_back(ACTION::JUMPING);
		act.blackList.emplace_back("player_Light_AttackFirst");
		act.blackList.emplace_back("player_Light_AttackSecond");
		act.blackList.emplace_back("player_Light_AttackThird");
		act.blackList.emplace_back("player_Light_Wall_Slide");
		actCtl_.RunInitializeActCtl(type_,"���ړ�", act);
		auto anchor3 = DrawNode::create();
		anchor3->drawDot(act.checkPoint1, 3.0f, Color4F::GREEN);
		this->addChild(anchor3, 5);
		auto anchor4 = DrawNode::create();
		anchor4->drawDot(act.checkPoint2, 3.0f, Color4F::GREEN);
		this->addChild(anchor4, 5);
	}

	// �E�������]
	{
		ActModule flipAct;
		flipAct.state = oprtState_;
		flipAct.flipFlg = false;
		flipAct.actName = "player_Light_Non";
		flipAct.button = BUTTON::RIGHT;
		flipAct.touch = TOUCH_TIMMING::TOUCHING; // �������ςȂ�

		//flipAct.blackList.emplace_back(ACTION::FALLING);

		flipAct.blackList.emplace_back("player_Light_AttackFirst");
		flipAct.blackList.emplace_back("player_Light_AttackSecond");
		flipAct.blackList.emplace_back("player_Light_AttackThird");
		flipAct.blackList.emplace_back("player_Light_Wall_Slide");
		actCtl_.RunInitializeActCtl(type_,"�E����", flipAct);
	}

	// ���������]
	{
		ActModule flipAct;
		flipAct.state = oprtState_;
		flipAct.flipFlg = true;
		flipAct.actName = "player_Light_Non";
		flipAct.button = BUTTON::LEFT;
		flipAct.touch = TOUCH_TIMMING::TOUCHING; // �������ςȂ�

		//flipAct.blackList.emplace_back(ACTION::FALLING);

		flipAct.blackList.emplace_back("player_Light_AttackFirst");
		flipAct.blackList.emplace_back("player_Light_AttackSecond");
		flipAct.blackList.emplace_back("player_Light_AttackThird");
		flipAct.blackList.emplace_back("player_Light_Wall_Slide");
		actCtl_.RunInitializeActCtl(type_,"������", flipAct);
	}

	// ����
	{
		// checkkeylist�ɗ����Ă���Ԃ̐ݒ���������Ǔ��Ɍ��ʂȂ�
		ActModule act;
		act.actName = "player_Light_Fall";
		act.state = oprtState_;
		act.button = BUTTON::DOWN;
		//act.checkPoint1 = Vec2{ 0,-10 };			// ����
		//act.checkPoint2 = Vec2{ 0,-10 };			// �E��
		act.checkPoint1 = Vec2{ 0,0 };				// ����
		act.checkPoint2 = Vec2{ 0,0 };				// �E��

		act.checkPoint3 = Vec2{ charSize.x / 2, charSize.y / 2 };  // �E��
		act.checkPoint4 = Vec2{ -charSize.x / 2, charSize.y / 2 }; // ����

		act.gravity = Vec2{ 0.0f,-5.0f };
		act.touch = TOUCH_TIMMING::RELEASED;	// �����Ɨ����Ă���
		act.blackList.emplace_back("player_Light_Jumping");	// �W�����v���ɗ������Ăق����Ȃ�
		act.blackList.emplace_back("player_Light_Wall_Slide");	
		//act.blackList.emplace_back(ACTION::JUMP);	// �W�����v���ɗ������Ăق����Ȃ�

		actCtl_.RunInitializeActCtl(type_,"����", act);
	}

	// �W�����v
	{
		ActModule act;
		act.state = oprtState_;
		act.button = BUTTON::UP;
		act.actName = "player_Light_Jump";
		act.checkPoint1 = Vec2{ -charSize.x/3 + 5, charSize.y };		// ����
		act.checkPoint2 = Vec2{ charSize.x/3 - 5, charSize.y };			// �E��
		//act.checkPoint1 = Vec2{ -10, 30 };						// ����
		//act.checkPoint2 = Vec2{ +10, 30 };						// �E��
		act.jumpVel = Vec2{ 0.0f,20.0f };
		act.touch = TOUCH_TIMMING::ON_TOUCH;	// �������u��

		// ������R�����g�A�E�g���Ă���ƁA���E�����Ȃ���̃W�����v���ł���
		// �ł��A���ŃW�����v���ď㏸��������悤�ɂȂ�
		// ������FALL��JUMP���������č������o�Ȃ�
		act.blackList.emplace_back("player_Light_Fall");	// �������ɃW�����v���Ăق����Ȃ�
		act.blackList.emplace_back("player_Light_AttackFirst");
		act.blackList.emplace_back("player_Light_AttackSecond");
		act.blackList.emplace_back("player_Light_AttackThird");
		//act.whiteList.emplace_back(ACTION::RUN);

		actCtl_.RunInitializeActCtl(type_,"�W�����v", act);
	}

	// �W�����s���O
	{
		ActModule act;
		act.state = oprtState_;
		act.button = BUTTON::UP;
		act.actName = "player_Light_Jumping";
		act.checkPoint1 = Vec2{ -charSize.x/3 + 5, charSize.y };	// ����
		act.checkPoint2 = Vec2{ charSize.x/3 - 5, charSize.y };		// �E��
		//act.checkPoint1 = Vec2{ -10, 30 };					// ����
		//act.checkPoint2 = Vec2{ +10, 30 };					// �E��
		act.jumpVel = Vec2{ 0.0f,20.0f };
		act.touch = TOUCH_TIMMING::TOUCHING;	// �������ςȂ�

		act.blackList.emplace_back("player_Light_Fall");	// �������ɃW�����v���Ăق����Ȃ�
		act.blackList.emplace_back("player_Light_Look_Intro");
		act.blackList.emplace_back("player_Light_Run");
		act.blackList.emplace_back("player_Light_AttackFirst");
		act.blackList.emplace_back("player_Light_AttackSecond");
		act.blackList.emplace_back("player_Light_AttackThird");
		act.blackList.emplace_back("player_Light_Non");
		act.blackList.emplace_back("player_Light_Wall_Slide");
		act.blackList.emplace_back("player_Light_Transform");

		act.whiteList.emplace_back("player_Light_Jump");
		actCtl_.RunInitializeActCtl(type_,"�W�����s���O", act);
		//actCtl_.ActCtl("�W�����v", act);
	}

	// �U��
	{
		ActModule act;
		act.state = oprtState_;
		//act.button = BUTTON::ATTACK;
		act.button = BUTTON::DOWN;
		act.actName = "player_Light_AttackFirst";
		//act.checkPoint1 = Vec2{ 0, 0 };		
		//act.checkPoint2 = Vec2{ 0, 0 };
		//act.blackList.emplace_back("Fall");	// �ꎞ�I�R�����g�A�E�g
		act.touch = TOUCH_TIMMING::ON_TOUCH;	// �������u��
		actCtl_.RunInitializeActCtl(type_,"�U��", act);
	}

	// �U��2
	{
		ActModule act;
		act.state = oprtState_;
		act.button = BUTTON::DOWN;
		act.actName = "player_Light_AttackSecond";
		act.blackList.emplace_back("player_Light_Fall");
		act.touch = TOUCH_TIMMING::ON_TOUCH;	// �������u��
		actCtl_.RunInitializeActCtl(type_,"�U��", act);
	}

	// �U��3
	{
		ActModule act;
		act.state = oprtState_;
		act.button = BUTTON::DOWN;
		act.actName = "player_Light_AttackThird";
		act.blackList.emplace_back("player_Light_Fall");
		act.touch = TOUCH_TIMMING::ON_TOUCH;	// �������u��
		actCtl_.RunInitializeActCtl(type_,"�U��", act);
	}

	// �E�ǃX���C�h
	{
		ActModule act;
		act.state = oprtState_;
		act.gravity = Vec2{ 0.0f,-1.0f };
		act.checkPoint1 = Vec2{ charSize.x / 2, charSize.y / 2 };	// �E��
		act.checkPoint2 = Vec2{ charSize.x / 2, 0 };				// �E��
		act.button = BUTTON::RIGHT;
		act.flipFlg = true;
		act.actName = "player_Light_Wall_Slide";
		act.touch = TOUCH_TIMMING::TOUCHING;	
		act.blackList.emplace_back("player_Light_Jumping");	
		act.blackList.emplace_back("player_Light_Run");	
		act.blackList.emplace_back("player_Light_Fall");
		act.blackList.emplace_back("player_Light_Look_Intro");
		act.blackList.emplace_back("player_Light_Non");
		act.blackList.emplace_back("player_Light_AttackFirst");
		act.blackList.emplace_back("player_Light_AttackSecond");
		act.blackList.emplace_back("player_Light_AttackThird");
		actCtl_.RunInitializeActCtl(type_,"�E�ǃX���C�h", act);
	}

	// ���ǃX���C�h
	{
		ActModule act;
		act.state = oprtState_;
		act.gravity = Vec2{ 0.0f,-1.0f };
		act.checkPoint1 = Vec2{-charSize.x / 2, charSize.y / 2 };	// ����
		act.checkPoint2 = Vec2{-charSize.x / 2, 0 };				// ����
		act.button = BUTTON::LEFT;
		act.flipFlg = false;
		act.actName = "player_Light_Wall_Slide";
		act.touch = TOUCH_TIMMING::TOUCHING;
		act.blackList.emplace_back("player_Light_Jumping");	
		act.blackList.emplace_back("player_Light_Run");
		act.blackList.emplace_back("player_Light_Fall");
		act.blackList.emplace_back("player_Light_Look_Intro");
		act.blackList.emplace_back("player_Light_Non");
		act.blackList.emplace_back("player_Light_AttackFirst");
		act.blackList.emplace_back("player_Light_AttackSecond");
		act.blackList.emplace_back("player_Light_AttackThird");
		actCtl_.RunInitializeActCtl(type_,"���ǃX���C�h", act);
	}
	// �X�V�֐��̓o�^
	actCtl_.InitUpdater(type_);
}

bool Player::gameOverAction(void)
{
	if (!gameOverFlg_)
	{
		// ���̓������~�߂�
		stopAllActions();

		this->setPosition(deathPos_);	// ���S�����ʒu�Őݒ肵�Ă����Ȃ��ƁA���������Ȃ�
		AnimationCache* animationCache = AnimationCache::getInstance();
		Animation* animation = animationCache->getAnimation("player_Light_Death2");
		animation->setRestoreOriginalFrame(false);
		//�A�j���[�V���������s
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
		// �A�N�V�������I����Ă���
		return true;
	}
	if (gameOverAction_->isDone())
	{
		// �A�N�V�������I�����
		CC_SAFE_RELEASE_NULL(gameOverAction_);
		return true;
	}
	return false;
}