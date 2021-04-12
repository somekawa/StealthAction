// �S�ĒS��
#include <functional>
#include "anim/AnimMng.h"
#include "_Debug/_DebugConOut.h"
#include "Scene/GameScene.h"
#include "anim/ActionCtl.h"
#include "ActionRect.h"
#include "Loader/CollisionLoader.h"
#include "../Skill/SkillMng.h"
#include "../Skill/SkillBase.h"
#include "../Skill/SkillCode/SkillA.h"
#include "../Skill/SkillCode/SkillB.h"
#include "../Skill/SkillCode/SkillC.h"
#include "../Loader/FileLoder.h"
#include "../HPGauge.h"
#include "SoundMng.h"
#include "ResidualShadow.h"
#include "Player.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "input/OPRT_key.h"
#else
#include "input/OPRT_touch.h"
#endif

USING_NS_CC;

int Player::no_ = 0;

// �摜�T�C�Y���擾����֐����������悤�Ȃ�������
// getContentSize�����������ǎ擾�s�\������
Player::Player(int hp,Layer& myLayer, Layer& enemyLayer, SkillBase* skillBasePtr):
	Actor(hp,myLayer),enemyList_(enemyLayer), myNo_(no_),charSize_(15.0f * 3.0f,25.0f * 3.0f)
{
	skillBase_ = skillBasePtr;
	std::list<std::string> path;
	path.push_back("skill_data");
	skillSprite_ = nullptr;

	const auto visibleSize = Director::getInstance()->getVisibleSize();
	const Vec2 origin	   = Director::getInstance()->getVisibleOrigin();

	lpSoundMng.AddSound("recovery" , "BGM/Recovery.mp3" , SoundType::SE);
	lpSoundMng.AddSound("burst"	   , "BGM/s-burst01.mp3", SoundType::SE);
	lpSoundMng.AddSound("burst2"   , "BGM/s-burst02.mp3", SoundType::SE);
	lpSoundMng.AddSound("Transform", "BGM/se_maoudamashii_element_fire01.mp3", SoundType::SE);
	lpSoundMng.AddSound("Knife"	   , "BGM/Knife.mp3"    , SoundType::SE);
	lpSoundMng.AddSound("Dash"	   , "BGM/Dash.mp3"     , SoundType::SE);

	// �L�[���͂��^�b�`���삩���f
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	// this�̈Ӗ�
	oprtState_ = new OPRT_key(this);
#else
	oprtState_ = new OPRT_touch(this);
#endif

	type_ = ActorType::Player;
	ActModuleRegistration();

	currentAnimation_ = "Non";
	actionOld_ = "Run";
	direction_ = Direction::Right;

	AnimRegistrator();

	//@@@�C�j�V�����C�Y����{ }��( )�̈Ⴂ�ɂ��Ē��ׂāA
	//����ɍ��킹���g�����ɏ��������邱��!!
	setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y + 200));
	no_++;
	type_ = ActorType::Player;
	playerMode_ = "player_Light_";

	attackMotion_ = 00000000;

	// �p�����[�^�̃f�[�^��ǂݍ���
	plParam_ = lpAnimMng.XmlPlParamRead();
	int tmpNum = 0;
	for (auto data : plParam_)
	{
		// ���ꂼ��̃��[�h�̍ő�A�����Ń��T�C�Y���A�A�j���[�V�������̏�������
		attackMotionName_[plParam_[tmpNum].modeName].resize(plParam_[tmpNum].attackChain);
		for (int num = 0; num < data.attackChain; num++)
		{
			attackMotionName_[plParam_[tmpNum].modeName][num] = data.chainAnimName[num].first;
		}
		tmpNum++;
	}

	bitFlg_ = { 0,0,0,0,0,0,0 };

	hp_ = hp;
	for (auto anim : lpAnimMng.GetAnimations(type_))
	{
		// colliderBox��Load
		lpCol.ReadData(collider_, anim);
	}

	gameOverAction_ = nullptr;

	// �U����`�̃T�C�Y�ݒ�
	attackRect_.size_ = Size(30.0f, 30.0f);

	SkillTbl_   = { "magic"  , "fireBall" ,"heal" };
	SkillSETbl_ = { "burst2" , "burst"    ,"recovery" };

	// �c��
	resShadow_ = std::make_shared<ResidualShadow>();
	resShadow_->CreateResidualShadow(*this, myLayer, 2);
	this->SetIsAttacking(false);
}

Player::~Player()
{
	// ���I�ȃC���X�^���X��������delete��Y��Ȃ��悤��
	delete oprtState_;
	delete skillBase_;
}

Player* Player::CreatePlayer(int hp, Layer& myLayer, Layer& enemyLayer, SkillBase* skillBasePtr)
{
	Player* pRet = new(std::nothrow) Player(hp, myLayer, enemyLayer, skillBasePtr);
	if (pRet && pRet->init())
	{
		pRet->autorelease();
	}
	else
	{
		delete pRet;
		pRet = nullptr;
	}
	return pRet;
}

// ���t���[���X�V
void Player::update(float delta)
{
	// getname��gamescene�łȂ��ꍇ�A��������return����
	if (Director::getInstance()->getRunningScene()->getName() != "GameScene")
	{
		return;
	}

	// ���S��Ԃ̍X�V�Ɗm�F(deathFlg_��true�Ȃ�A�b�v�f�[�g������������悤�ɂ���)
	if (bitFlg_.deathFlg)
	{
		if (GameOverAction())
		{
			bitFlg_.gameOverSceneFlg = true;
			return;
		}
		return;
	}

	// �L�����N�^�[�ؑ�
	if (oprtState_->GetNowData()[static_cast<int>(BUTTON::Transfrom)] && !oprtState_->GetOldData()[static_cast<int>(BUTTON::Transfrom)])
	{
		lpSoundMng.PlayBySoundName("Transform", 0.5f);
	}

	SkillAction();

	// ���݂̃t���[���𐮐��l�Ŏ擾
	animationFrame_int_ = GetAnimationFrameInt(playerMode_ + currentAnimation_);

	// ActionCtl�N���X�̍X�V
	actCtl_.Update(type_,*this);

	// ���͏��(key��touch)�̍X�V
	oprtState_->update();	
	
	// �ǃX���C�h���̂݃A���J�[�|�C���g���ړ�������(���̑��͉摜�̉�����)
	if (currentAnimation_ == "Wall_Slide")
	{
		// �����F�� = �A���J�[�E�[,�����F�E = �A���J�[���[
		this->setAnchorPoint(direction_ == Direction::Left ? Vec2(1.0f, 0.0f) : Vec2(0.0f, 0.0f));
	}
	else
	{
		// �A���J�[�|�C���g������
		this->setAnchorPoint(Vec2(0.5f, 0.0f));
	}

	// �X���C�h�I�����ɍ��E�����𔽓]������
	if (currentAnimation_ != "Wall_Slide" && actionOld_ == "Wall_Slide")
	{
		direction_ = (direction_ == Direction::Right ? Direction::Left : Direction::Right);
	}

	// �N���X����HPBar�̕����p�ꌗ�ł͈�ʓI�A�C�����Ă�������!
	const auto nowScene = dynamic_cast<Game*>(Director::getInstance()->getRunningScene());
	const auto hpBar    = dynamic_cast<HPGauge*>(nowScene->getChildByTag(static_cast<int>(zOrder::FRONT))->getChildByName("PL_HPgauge"));

	// dark���[�h(�U���͂͏オ�邪�AHP�͌�����������)
	if (playerMode_ == "player_Dark_")
	{
		// HP��������
		hpBar->SetHP(hpBar->GetHP() - 0.05f);	// �X���b�v�_���[�W�ݒ�
	}

	// �_���[�W����������ۂ̏���(���̒���death�����������Ă��܂��ƁAdark���[�h�Ŏ��S�����Ƃ��ɑΉ��ł��Ȃ�)
	if (isHitAttack_)
	{
		// HP��������
		hpBar->SetHP(hpBar->GetHP() - 10.0f);
		isHitAttack_ = false;
	}

	// 0������������ɐ����t���O�Ǝ��S�t���O��؂�ւ���
	if (hpBar->GetHP() <= 0.0f)
	{
		isAlive_  = false;
		bitFlg_.deathFlg = true;
		deathPos_ = getPosition();
	}

	AttackMotion(delta);
	TransformMotion(delta);
	DashMotion(delta);

	// �A�j���[�V�������؂�ւ��^�C�~���O�ŌĂ΂��Đ�����
	if (currentAnimation_ != actionOld_)
	{
		if (currentAnimation_ == "AttackFirst")
		{
			lpSoundMng.PlayBySoundName("Knife");
			attackMotion_ |= ATTAKFLAG;	// AttackFirst�̃t���O���I��
			// �U���G�t�F�N�g�̒ǉ�
			lpEffectMng.PickUp("attack", { 55.0f, 50.0f }, getPosition(), { 0.0f,0.0f }, 5, 0.04f,
							   (direction_ == Direction::Right ? true : false), false, true);
		}

		animationFrame_int_ = 0;
		lpAnimMng.ChangeAnimation(*this, playerMode_ + currentAnimation_, true, ActorType::Player);
	}
	actionOld_ = currentAnimation_;
	SetCollider();
}

void Player::AttackMotion(float delta)
{
	auto keyLambda = [&](bool flag) {
		if (oprtState_->GetNowData()[static_cast<int>(BUTTON::Attack)] && !oprtState_->GetOldData()[static_cast<int>(BUTTON::Attack)])
		{
			return true;
		}
		return flag;
	};

	auto frameLambda = [&]() {
		// frame�v�Z
		animationFrame_ += delta;
		animationFrame_int_ = GetAnimationFrameInt(playerMode_ + currentAnimation_);
		if (animationFrame_int_ <= lpAnimMng.GetAnimationMaxNum(ActorType::Player, playerMode_ + currentAnimation_))
		{
			currentCol_ = collider_[playerMode_ + currentAnimation_][animationFrame_int_];
		}
	};

	// �U���A�j���[�V�����̐ݒ�
	for (unsigned int num = 0; num < attackMotionName_[playerMode_].size(); num++)
	{
		// �t���O�̏�Ԃ��m�F
		if (attackMotion_ & (ATTAKFLAG << num))
		{
			currentAnimation_ = attackMotionName_[playerMode_][num];
			if (bitFlg_.oldPosKeepFlg_)
			{
				break;
			}
			if (playerMode_ == "player_Dark_")
			{
				const Vec2 tmpPos(this->getPosition());
				this->setPosition(tmpPos.x, tmpPos.y - 15.0f);	// �ʒu�␳�����Ȃ��ƕ����Č�����
			}
			bitFlg_.oldPosKeepFlg_ = true;
			break;
		}
	}

	// �A�������ςɂł���悤�ɁA�ő吔�����߂ă��[�v�ŏ������s����悤�ɂ����ق�������
	for (unsigned int i = 0; i < attackMotionName_[playerMode_].size(); i++)
	{
		// ���݂̃A�j���[�V�������ƈ�v���Ă��Ȃ��Ƃ���continue
		if (currentAnimation_ != attackMotionName_[playerMode_][i])
		{
			continue;
		}

		// �ŏI�A�j���[�V�����ł̓L�[�`�F�b�N���s��Ȃ��悤�ɂ���
		if (i < attackMotionName_[playerMode_].size() - 1)
		{
			// �L�[�������Ɏ��̍U�����[�V�����̃r�b�g�𗧂Ă�
			if (keyLambda(attackMotion_ & (ATTAKFLAG << (i + 1))))
			{
				attackMotion_ |= ATTAKFLAG << (i + 1);
			}
		}

		frameLambda();

		if (animationFrame_ <= lpAnimMng.GetAnimationMaxFrame(ActorType::Player, playerMode_ + currentAnimation_))
		{
			currentAnimation_ = attackMotionName_[playerMode_][i];
			isAttacking_ = true;
		}
		else
		{
			// �U���t���O�������Ă��Ȃ��Ƃ�
			if (!(attackMotion_ & (ATTAKFLAG << (i + 1))))
			{
				currentAnimation_ = "Look_Intro";
				this->SetIsAttacking(false);
			}
			else
			{
				lpSoundMng.PlayBySoundName("Knife");
				if (i < attackMotionName_[playerMode_].size() - 1)
				{
					currentAnimation_ = attackMotionName_[playerMode_][i + 1];
					// �U���G�t�F�N�g�̒ǉ�
					lpEffectMng.PickUp("attack", { 55.0f, 50.0f }, getPosition(), { 0.0f,0.0f }, 5, 0.04f,
										(direction_ == Direction::Right ? true : false), false, true);
				}
			}

			// dark��light�ŉ摜�T�C�Y���Ⴄ����␳
			if (playerMode_ == "player_Dark_")
			{
				const Vec2 tmpPos(this->getPosition());
				this->setPosition(tmpPos.x, tmpPos.y + 15.0f); // �ʒu�␳��߂�
			}
			animationFrame_ = 0.0f;
			attackMotion_  &= ~(ATTAKFLAG << i);	// �t���O�̃I�t
			isAttacking_    = false;
			bitFlg_.oldPosKeepFlg_  = false;
		}
	}
}

void Player::TransformMotion(float delta)
{
	if (bitFlg_.transformFlg)
	{
		animationFrame_ += delta;
		if (animationFrame_ < lpAnimMng.GetAnimationMaxFrame(ActorType::Player, playerMode_ + currentAnimation_))
		{
			currentAnimation_ = "Transform";
		}
		else
		{
			currentAnimation_ = "Look_Intro";
			bitFlg_.transformFlg = false;
			const Vec2 tmpPos(this->getPosition());
			this->setPosition(tmpPos.x, tmpPos.y + 10.0f);	// �ʒu�␳���߂�
			playerMode_ = (playerMode_ == "player_Light_" ? "player_Dark_" : "player_Light_");
			animationFrame_ = 0.0f;
		}
	}
	else
	{
		if (oprtState_->GetNowData()[static_cast<int>(BUTTON::Transfrom)] && !oprtState_->GetOldData()[static_cast<int>(BUTTON::Transfrom)])
		{
			currentAnimation_    = "Transform";
			bitFlg_.transformFlg = true;
			const Vec2 tmpPos(this->getPosition());
			this->setPosition(tmpPos.x, tmpPos.y - 10.0f);	// �ʒu�␳�����Ȃ��ƕ����Č�����
		}
	}
}

void Player::DashMotion(float delta)
{
	// �����蔻��
	auto colLambda = [&](Vec2 move) {
		const auto CollisionData = dynamic_cast<TMXLayer*>(Director::getInstance()->getRunningScene()->getChildByTag(static_cast<int>(zOrder::BG))->getChildByName("MapData")->getChildByName("col"));
		const Vec2 plCheckPoint1(this->getPosition() + move);
		const Vec2 plCheckPoint1Chip(plCheckPoint1 / CollisionData->getMapTileSize().width);
		const Vec2 plCheckPoint1Pos (plCheckPoint1Chip.x, CollisionData->getLayerSize().height - plCheckPoint1Chip.y);
		if ((CollisionData->getTileGIDAt(plCheckPoint1Pos)) != 0)
		{
			currentAnimation_ = "Look_Intro";
			bitFlg_.dashFlg   = false;
			animationFrame_   = 0.0f;
			resShadow_->ResShadowEnd();
		}
	};

	if (!bitFlg_.dashFlg)
	{
		// �_�b�V���A�N�V�����J�n
		if (oprtState_->GetNowData()[static_cast<int>(BUTTON::Dash)] && !oprtState_->GetOldData()[static_cast<int>(BUTTON::Dash)])
		{
			currentAnimation_ = "Dash";
			bitFlg_.dashFlg   = true;
			lpSoundMng.PlayBySoundName("Dash");
		}
	}
	else
	{
		resShadow_->Start(*this);
		currentAnimation_ = "Dash";
		animationFrame_  += delta;

		// �ړ��l�v�Z����(���� : y = exp(-pow(x,2)))
		// exp�c�l�C�s�A��
		const auto move = exp(-pow(Calculation(animationFrame_), 2));
		const Vec2 dashSpeed(move * DashMove, 0);
		if (direction_ == Direction::Right)
		{
			resShadow_->Move(this->getPosition(), dashSpeed);
			runAction(cocos2d::MoveBy::create(0.0f, dashSpeed));
			colLambda({ dashSpeed.x + charSize_.x, dashSpeed.y + charSize_.y / 2.0f });
		}
		else
		{
			resShadow_->Move(this->getPosition(), -dashSpeed);
			runAction(cocos2d::MoveBy::create(0.0f, -dashSpeed));
			colLambda({ dashSpeed.x - charSize_.x, dashSpeed.y + charSize_.y / 2.0f });
		}

		if (animationFrame_ >= lpAnimMng.GetAnimationMaxFrame(ActorType::Player, playerMode_ + currentAnimation_))
		{
			// �_�b�V���A�N�V�����I��
			currentAnimation_ = "Look_Intro";
			bitFlg_.dashFlg   = false;
			animationFrame_   = 0.0f;
			resShadow_->ResShadowEnd();
		}
	}
}

std::string Player::GetAction(void)
{
	return currentAnimation_;
}

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
	oprtState_->ResetKey();
}

void Player::AnimRegistrator(void)
{
	// �o�^�f�[�^�̏�����
	lpAnimMng.AnimDataClear();	

	lpAnimMng.XmlAnimDataRead("playerAnim_Light", ActorType::Player);
	lpAnimMng.XmlAnimDataRead("playerAnim_Dark" , ActorType::Player);

	// �����A�j���[�V�����̐ݒ�
	lpAnimMng.InitAnimation(*this, ActorType::Player, "player_Light_Non");
}

const AttackRect& Player::GetAttackRect(void)
{
	return attackRect_;
}

bool Player::GetGameOverFlg(void)
{
	return bitFlg_.gameOverSceneFlg;
}

int Player::GetGiveDamageNum(void)
{
	// �A���񐔂ɂ���ĉΗ͂����������
	float damageUp = 0.0f;
	bool  breakFlg = false;
	// ��ɁA���݂̃A�j���[�V�����ƏƂ炵���킹�āA�{�����擾����
	for (auto data : plParam_)
	{
		if (breakFlg)
		{
			break;
		}

		// ���[�h�s��v�̏ꍇ�A�������Ƃ΂�
		if (data.modeName != playerMode_)
		{
			continue;
		}

		// �{�����ꎞ�ϐ��ɕۑ�����for���𔲂���
		for (auto animName : data.chainAnimName)
		{
			// ���݂̃A�j���[�V�����ƕs��v�̏ꍇ�A�������Ƃ΂�
			if (animName.first != currentAnimation_)
			{
				continue;
			}
			damageUp = animName.second;
			breakFlg = true;
			break;
		}
	}

	// ���݂̃��[�h���m�F���A��̔{�������ƂɍŏI�_���[�W���Z�o����
	int tmpNum = 0;
	for (auto data : plParam_)
	{
		if (data.modeName == playerMode_)
		{
			return static_cast<int>(plParam_[tmpNum].attackDef + (plParam_[tmpNum].attackDef * damageUp));
		}
		tmpNum++;
	}
	return 0;
}

std::string Player::GetPlayerColor(void)
{
	return playerMode_;
}

float Player::GetRunSpeedUp(void)
{
	// ���݂̃��[�h����ړ����x�̉��Z�l���擾����
	int tmpNum = 0;
	for (auto data : plParam_)
	{
		if (data.modeName == playerMode_)
		{
			return plParam_[tmpNum].runSpeedUp;
		}
		tmpNum++;
	}
	return 0.0f;
}

void Player::ActModuleRegistration(void)
{
	Actor::XmlActDataRead("playerModule",oprtState_);
}

void Player::SkillAction(void)
{
	// �����̃X�L����active�ł���Ƃ��́A�{�^�����������Ȃ��悤�ɂ��Ă���
	const auto director = Director::getInstance();

	if (dynamic_cast<SkillBase*>(director)->GetAllSkillActivate())
	{
		// �����ɓ����Ă�����Aactive���̃X�L��������Ƃ������Ƃ�����A���̃X�L���������s�ɂȂ�
		return;
	} 

	// �X�L���̐ݒ�
	auto SetSkillLambda = [&](std::string seName) {
		skillSprite_ = dynamic_cast<SkillBase*>(director->getRunningScene()->getChildByTag(static_cast<int>(zOrder::FRONT))->getChildByName("skillSprite"));
		skillSprite_->SetPlayerPos(getPosition());
		skillSprite_->SetPlayerDirection(direction_);
		SkillBase* skill = nullptr;
		if (seName == "burst2")
		{
			skill = SkillA::CreateSkillA(skillSprite_);
		}
		else if (seName == "burst")
		{
			const auto playerPos = getPosition();
			auto minPos = Vec2(0.0f, 0.0f);
			auto minLen = FLT_MAX;			// FLT_MAX�Ffloat�̍ő�̗L���l
			for (auto enemy : enemyList_.getChildren())
			{
				const auto enemyPos = enemy->getPosition();
				const auto vecComp  = enemyPos - playerPos;
				const auto length   = sqrt(vecComp.x * vecComp.x + vecComp.y * vecComp.y);
				if (minLen > length)
				{
					minLen = length;
					minPos = enemyPos;
				}
			}
			skillSprite_->SetTargetPos(minPos);
			skill = SkillB::CreateSkillB(skillSprite_);
		}
		else if (seName == "recovery")
		{
			// dark���[�h�ł͎g�p�s�ɂ���
			if (playerMode_ == "player_Dark_")
			{
				return;
			}

			// �񕜏���
			const auto hpGauge = dynamic_cast<HPGauge*>(director->getRunningScene()->getChildByTag(static_cast<int>(zOrder::FRONT))->getChildByName("PL_HPgauge"));
			hpGauge->SetHP(hpGauge->GetHP() + 50.0f);
			skill = SkillC::CreateSkillC(skillSprite_);
		}
		else
		{
			return;
		}
		skillSprite_->AddActiveSkill(skill);
		skillSprite_->addChild(skill);
		lpSoundMng.PlayBySoundName(seName);	
	};

	// �X�L����������
	for (int i = static_cast<int>(BUTTON::SkillA); i <= static_cast<int>(BUTTON::SkillC); i++)
	{
		if (oprtState_->GetNowData()[static_cast<int>(i)] && !oprtState_->GetOldData()[static_cast<int>(i)])
		{
			if (dynamic_cast<SkillBase*>(director)->GetSkillCT(SkillTbl_[i - static_cast<int>(BUTTON::SkillA)]))
			{
				SetSkillLambda(SkillSETbl_[i - static_cast<int>(BUTTON::SkillA)]);
			}
			else
			{
				// �N�[���^�C����
			}
		}
	}

	if (skillSprite_ != nullptr)
	{
		skillSprite_->RemoveActiveSkill();
	}
}

bool Player::GameOverAction(void)
{
	if (!bitFlg_.gameOverFlg)
	{
		// ���̓������~�߂�
		stopAllActions();

		this->setPosition(deathPos_);	// ���S�����ʒu�Őݒ肷��
		AnimationCache* animationCache = AnimationCache::getInstance();
		Animation* animation = animationCache->getAnimation(playerMode_ + "Death");
		animation->setRestoreOriginalFrame(false);
		//�A�j���[�V���������s
		gameOverAction_ = runAction(
			Sequence::create(
				Animate::create(animation),
				nullptr
			));
		CC_SAFE_RETAIN(gameOverAction_);
		bitFlg_.gameOverFlg = true;
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