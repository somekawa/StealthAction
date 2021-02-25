#include "Scene/GameScene.h"
#include "Player.h"
#include "anim/ActionCtl.h"
#include "ActionRect.h"
#include "_Debug/_DebugConOut.h"
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

	lpSoundMng.AddSound("recovery", "BGM/Recovery.mp3", SoundType::SE);
	lpSoundMng.AddSound("burst", "BGM/s-burst01.mp3", SoundType::SE);
	lpSoundMng.AddSound("burst2", "BGM/s-burst02.mp3", SoundType::SE);
	lpSoundMng.AddSound("Transform", "BGM/se_maoudamashii_element_fire01.mp3", SoundType::SE);
	lpSoundMng.AddSound("Knife", "BGM/Knife.mp3", SoundType::SE);

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
	//plfile_ = fileLoad_["Player"];											// ���̒��ɍ��A�X�L���̏�񂪓ǂݍ��܂�Ă���(���O�Ƃ�)

	type_ = ActorType::Player;
	actModuleRegistration();

	currentAnimation_ = "Non";
	actionOld_ = "Run";
	direction_ = Direction::Right;

	AnimRegistrator();

	pos_ = { (int)visibleSize.width / 2 + (int)origin.x - 0,(int)visibleSize.height / 2 + (int)origin.y + 200 };
	setPosition(Vec2(pos_.x, pos_.y));
	myNo_ = no_;
	no_++;
	type_ = ActorType::Player;
	//oldPos_ = 0;

	oldPosKeepFlg_ = false;
	bitFlg_.FirstAttackFlg = false;
	bitFlg_.SecondAttackFlg = false;
	bitFlg_.ThirdAttackFlg = false;
	bitFlg_.TransfromFlg = false;

	gameOverFlg_ = false;
	deathFlg_ = false;
	hp_ = hp;
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


	// �c��
	resShadow_ = std::make_shared<ResidualShadow>();
	resShadow_->CreateResidualShadow(*this, myLayer, 2);
	this->SetIsAttacking(false);
}

Player::~Player()
{
	// ���I�ȃC���X�^���X��������delete��Y��Ȃ��悤��!
	delete oprtState_;
	delete skillBase_;
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
			gameOverSceneFlg_ = true;
			return;
		}
		return;
	}

	// �o�[�T�[�J�[���[�h(�U���͂͏オ�邪�AHP�͌�����������)
	if (playerColor == "player_Dark_")
	{
		// HP��������
		auto nowScene = ((Game*)Director::getInstance()->getRunningScene());
		auto hpGauge = (HPGauge*)nowScene->getChildByTag((int)zOlder::FRONT)->getChildByName("PL_HPgauge");
		hpGauge->SetHP(hpGauge->GetHP() - 0.05f);	// HP�����葱����悤�ɐݒ�
	}

	// �L�����N�^�[�ؑ�
	if (oprtState_->GetNowData()[static_cast<int>(BUTTON::Transfrom)] && !oprtState_->GetOldData()[static_cast<int>(BUTTON::Transfrom)])
	{
		playerColor == "player_Light_" ? playerColor = "player_Dark_" : playerColor = "player_Light_";
	}

	skillAction();

	// ���݂̃t���[���𐮐��l�Ŏ擾
	animationFrame_int_ = GetAnimationFrameInt(playerColor + currentAnimation_);
	//currentCol_ = collider_[currentAnimation_][animationFrame_int_];

	colliderVisible();

	// ActionCtl�N���X�̍X�V
	actCtl_.update(type_,delta,*this);

	// ���͏��(key��touch)�̍X�V
	oprtState_->update();	
	
	// �X���C�h���̂݃A���J�[�|�C���g���ړ�������(���̑��͉摜�̉�����)
	if (currentAnimation_ == "Wall_Slide")
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
	if (currentAnimation_ != "Wall_Slide" && actionOld_ == "Wall_Slide")
	{
		auto dir = direction_;
		direction_ == Direction::Right ? direction_ = Direction::Left : direction_ = Direction::Right;
		auto tmpdir = direction_;
	}

	// isHitAttack: �U��������������true�ɂȂ�܂��B
	// �U�����A���œ������Ă���悤�ɂȂ�AHP�̌������������B<- �����Ȃ��Ƃ����Ȃ��B
	onDamaged_ = isHitAttack_;

	// �_���[�W����������ۂ̏���(���̒���death�����������Ă��܂��ƁA�o�[�T�[�J�[���[�h�Ŏ��S�����Ƃ��ɑΉ��ł��Ȃ�)
	if (onDamaged_)
	{
		// HP��������
		auto nowScene = ((Game*)Director::getInstance()->getRunningScene());
		auto hpGauge = (HPGauge*)nowScene->getChildByTag((int)zOlder::FRONT)->getChildByName("PL_HPgauge");
		hpGauge->SetHP(hpGauge->GetHP() - 10);	// -10�Ȃǂ̃_���[�W�ʂ͓G�̍U���͂ɕς���΂���
		onDamaged_ = false;
		isHitAttack_ = false;
	}

	auto nowScene = ((Game*)Director::getInstance()->getRunningScene());
	auto hpGauge = (HPGauge*)nowScene->getChildByTag((int)zOlder::FRONT)->getChildByName("PL_HPgauge");
	// 0������������Ƀt���O��؂�ւ���
	if (hpGauge->GetHP() <= 0)
	{
		// �����׸�(Actor�׽�ɐ錾���Ă܂�)
		// ���m�ɂ͐����Ă��邩���m�F�����׸ނȂ̂�
		// ����: false �����Ă���:true�ƂȂ�
		isAlive_ = false;
		deathFlg_ = true;
		deathPos_ = getPosition();
	}

	attackMotion(delta);
	transformMotion(delta);
	dashMotion(delta);

	// �A�j���[�V�������؂�ւ��^�C�~���O�ŌĂ΂��Đ�����
	if (currentAnimation_ != actionOld_)
	{
		if (currentAnimation_ == "AttackFirst" && !bitFlg_.FirstAttackFlg)
		{
			lpSoundMng.PlayBySoundName("Knife");
			bitFlg_.FirstAttackFlg = true;
			// ���]�׸ނ̾��
			bool flip = false;
			direction_ == Direction::Right ? flip = true : flip = false;
			// �U���̪�Ă̒ǉ�
			auto attackSprite = lpEffectMng.PickUp("attack", { 55.0f, 50.0f }, getPosition(), {0.0f,0.0f}, 5, 0.04f,flip,false,true);

		}
		animationFrame_int_ = 0.0f;
		lpAnimMng.ChangeAnimation(*this, playerColor + currentAnimation_, true, ActorType::Player);
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
		if (oprtState_->GetNowData()[static_cast<int>(BUTTON::DOWN)] && !oprtState_->GetOldData()[static_cast<int>(BUTTON::DOWN)] && !flag)
		{
			return true;
		}
		return false;
	};

	if (bitFlg_.FirstAttackFlg)
	{
		currentAnimation_ = "AttackFirst";
		if (!oldPosKeepFlg_)
		{
			if (playerColor == "player_Dark_")
			{
				this->setPosition(this->getPosition().x, this->getPosition().y - 15);		// �ʒu�␳�����Ȃ��ƕ����Č�����
			}
			//oldPos_ = this->getPosition().x;
			oldPosKeepFlg_ = true;
		}
	}
	else if (bitFlg_.SecondAttackFlg)
	{
		currentAnimation_ = "AttackSecond";
	}
	else if (bitFlg_.ThirdAttackFlg)
	{
		currentAnimation_ = "AttackThird";
	}

	if (currentAnimation_ == "AttackFirst" && bitFlg_.FirstAttackFlg)
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
			auto col = collider_[playerColor + currentAnimation_];
			currentCol_ = col[animationFrame_int_];
			colliderVisible();
		}

		if (bitFlg_.FirstAttackFlg && animationFrame_ <= 0.5f)
		{
			currentAnimation_ = "AttackFirst";
			isAttacking_ = true;
		}
		else
		{
			if (!bitFlg_.SecondAttackFlg)
			{
				currentAnimation_ = "Look_Intro";
				this->SetIsAttacking(false);

				// dark��light�ŉ摜�T�C�Y���Ⴄ���炻�̕␳
				if (playerColor == "player_Dark_")
				{
					this->setPosition(this->getPosition().x, this->getPosition().y + 15);		// �ʒu�␳��߂�
				}
				else
				{
					//this->setPosition(oldPos_, this->getPosition().y);
					this->setPosition(this->getPosition().x, this->getPosition().y);
				}
			}
			else
			{
				currentAnimation_ = "AttackSecond";
				// ���]�׸ނ̾��
				bool flip = false;
				direction_ == Direction::Right ? flip = true : flip = false;
				// �U���̪�Ă̒ǉ�
				auto attackSprite = lpEffectMng.PickUp("attack", { 55.0f, 50.0f }, getPosition(), { 0.0f,0.0f }, 5, 0.04f, flip, false,true);
			}
			animationFrame_ = 0.0f;
			bitFlg_.FirstAttackFlg = false;
			isAttacking_ = false;
			oldPosKeepFlg_ = false;
		}
	}

	if ((currentAnimation_ == "AttackSecond" && bitFlg_.SecondAttackFlg))
	{
		bitFlg_.ThirdAttackFlg = keyLambda(bitFlg_.ThirdAttackFlg);

		// frame�v�Z
		animationFrame_ += delta;
		animationFrame_int_ = (int)(animationFrame_ * 100) / (int)(0.08 * 100);
		if (animationFrame_int_ < 10)
		{
			// 2�t���[���ڂ�data��2����A���̂����̕Е���type:0������U����`�ɂȂ��Ă�
			currentCol_ = collider_[playerColor + currentAnimation_][animationFrame_int_];
			colliderVisible();
		}

		if (bitFlg_.SecondAttackFlg && animationFrame_ <= 0.8f)
		{
			currentAnimation_ = "AttackSecond";
			isAttacking_ = true;
		}
		else
		{
			if (!bitFlg_.ThirdAttackFlg)
			{
				currentAnimation_ = "Look_Intro";
				// dark��light�ŉ摜�T�C�Y���Ⴄ���炻�̕␳
				if (playerColor == "player_Dark_")
				{
					this->setPosition(this->getPosition().x, this->getPosition().y + 15);		// �ʒu�␳��߂�
				}
				else
				{
					this->setPosition(this->getPosition().x, this->getPosition().y);
					//this->setPosition(oldPos_, this->getPosition().y);
				}
			}
			else
			{
				currentAnimation_ = "AttackThird";
				// ���]�׸ނ̾��
				bool flip = false;
				direction_ == Direction::Right ? flip = true : flip = false;
				// �U���̪�Ă̒ǉ�
				auto attackSprite = lpEffectMng.PickUp("attack", { 55.0f, 50.0f }, getPosition(), { 0.0f,0.0f }, 5, 0.04f, flip, false,true);
			}
			bitFlg_.SecondAttackFlg = false;
			isAttacking_ = false;
			animationFrame_ = 0.0f;
			//oldPosKeepFlg_ = false;
		}
	}

	if ((currentAnimation_ == "AttackThird" && bitFlg_.ThirdAttackFlg))
	{
		// frame�v�Z
		animationFrame_ += delta;
		animationFrame_int_ = (int)(animationFrame_ * 100) / (int)(0.08 * 100);
		if (animationFrame_int_ < 11)
		{
			// 2�t���[���ڂ�data��2����A���̂����̕Е���type:0������U����`�ɂȂ��Ă�
			currentCol_ = collider_[playerColor + currentAnimation_][animationFrame_int_];
			colliderVisible();
		}

		if (bitFlg_.ThirdAttackFlg && animationFrame_ <= 0.8f)
		{
			currentAnimation_ = "AttackThird";
			isAttacking_ = true;
		}
		else
		{
			bitFlg_.ThirdAttackFlg = false;
			isAttacking_ = false;
			currentAnimation_ = "Look_Intro";
			// dark��light�ŉ摜�T�C�Y���Ⴄ���炻�̕␳
			if (playerColor == "player_Dark_")
			{
				this->setPosition(this->getPosition().x, this->getPosition().y + 15);		// �ʒu�␳��߂�
			}
			else
			{
				this->setPosition(this->getPosition().x, this->getPosition().y);
				//this->setPosition(oldPos_, this->getPosition().y);
			}
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
		lpSoundMng.PlayBySoundName("Transform",0.2f);
		currentAnimation_ = "Transform";
	}
	if (!bitFlg_.TransfromFlg)
	{
		if (oprtState_->GetNowData()[static_cast<int>(BUTTON::Transfrom)] && !oprtState_->GetOldData()[static_cast<int>(BUTTON::Transfrom)])
		{
			this->setPosition(this->getPosition().x, this->getPosition().y - 10);		// �ʒu�␳�����Ȃ��ƕ����Č�����
			currentAnimation_ = "Transform";
			bitFlg_.TransfromFlg = true;
		}
	}
	if (bitFlg_.TransfromFlg)
	{
		animationFrame_ += delta;
		if (animationFrame_ >= 1.85f)
		{
			bitFlg_.TransfromFlg = false;
			currentAnimation_ = "Look_Intro";
			animationFrame_ = 0.0f;
			this->setPosition(this->getPosition().x, this->getPosition().y + 10);	// �ʒu�␳���߂�
		}
		else
		{
			currentAnimation_ = "Transform";
		}
	}
}

void Player::dashMotion(float delta)
{
	// ����ŉ���̎��͂ł��Ă�
	//if (testnum >= -4.0f && testnum <= 4.0f)
	//{
	//	// 0�̂Ƃ����ő�l(1.0)�ɂȂ�
	//	auto move = exp(-pow(testnum, 2));
	//	TRACE("num:%f,pl_move:%f\n", testnum,move);
	//	testnum+=0.1f;
	//}
	//else
	//{
	//	int stop = 0;
	//}
	// 16�̂Ƃ����ő�l(1.0)�ɂȂ�?���Ȃ�Ȃ��B0��1.0�ōő�l�ɂȂ�d�g��
	// ��̎���dash�̃A�j���[�V����frame�Ōv�Z�����ꍇ�̏������e�X�g
	//if (testnum >= 0.00f && testnum <= 0.32f)
	//{
	//	auto tmpnum = testnum;
	//	tmpnum *= 100.0f;
	//	tmpnum -= 16.0f;
	//	tmpnum /= 4.0f;
	//	auto move = exp(-pow(tmpnum, 2));
	//	TRACE("num:%f,pl_move:%f\n", tmpnum, move);
	//	testnum+=delta;
	//}
	//else
	//{
	//	int stop = 0;
	//}

	// �����蔻��
	auto lambda = [&](Vec2 move) {
		auto director = Director::getInstance();
		auto plPos = this->getPosition();
		auto CollisionData = (TMXLayer*)director->getRunningScene()->getChildByTag((int)zOlder::BG)->getChildByName("MapData")->getChildByName("col");
		auto& ColSize = CollisionData->getLayerSize();
		const int chipSize = CollisionData->getMapTileSize().width;
		auto plCheckPoint1 = plPos + move;
		auto plCheckPoint1Chip = Vec2{ plCheckPoint1 } / chipSize;
		auto plCheckPoint1Pos = Vec2(plCheckPoint1Chip.x, ColSize.height - plCheckPoint1Chip.y);
		auto plCheckPoint1Gid = CollisionData->getTileGIDAt(plCheckPoint1Pos);
		if (plCheckPoint1Gid != 0)
		{
			return false;
		}
		return true;
	};

	if (bitFlg_.DashFlg)
	{
		resShadow_->Start(*this);
		currentAnimation_ = "Dash";
	}
	if (!bitFlg_.DashFlg)
	{
		if (oprtState_->GetNowData()[static_cast<int>(BUTTON::Dash)] && !oprtState_->GetOldData()[static_cast<int>(BUTTON::Dash)])
		{
			//this->setPosition(this->getPosition().x + 100, this->getPosition().y);		
			currentAnimation_ = "Dash";
			bitFlg_.DashFlg = true;
		}
	}
	if (bitFlg_.DashFlg)
	{
		//y = exp(-pow(x,2))
		// pow�c2��Ƃ��ł�����
		// exp�c�l�C�s�A��
		float tmpnum = animationFrame_;
		tmpnum = ((tmpnum * 100.0f) - 16.0f) / 4.0f;
		//tmpnum *= 100.0f;
		//tmpnum -= 16.0f;
		//tmpnum /= 4.0f;
		auto move = exp(-pow(tmpnum, 2));
		Vec2 dashSpeed;
		if (direction_ == Direction::Right)
		{
			dashSpeed = Vec2(move * 30, 0);

			resShadow_->Move(this->getPosition(), dashSpeed);
			// �����ړ�(��r�ł���悤�ɃR�����g�A�E�g�Œu���Ă���)
			//runAction(cocos2d::MoveBy::create(0.0f, cocos2d::Vec2(0.3 * 30, 0)));
			runAction(cocos2d::MoveBy::create(0.0f, dashSpeed));

			Vec2 charSize = { 15.0f * 3.0f,25.0f * 3.0f };
			if (!lambda(Vec2(move * 30 + charSize.x / 2, 0 + charSize.y / 2)))
			{
				resShadow_->ResShadowEnd();
				TRACE("move�I��\n");
				bitFlg_.DashFlg = false;
				currentAnimation_ = "Look_Intro";
				animationFrame_ = 0.0f;
			}
		}
		else
		{
			dashSpeed = Vec2(move * -30, 0);

			resShadow_->Move(this->getPosition(), dashSpeed);
			runAction(cocos2d::MoveBy::create(0.0f, dashSpeed));
			Vec2 charSize = { 15.0f * 3.0f,25.0f * 3.0f };
			if (!lambda(Vec2(move * 30 - charSize.x / 2, 0 + charSize.y / 2)))
			{

				resShadow_->ResShadowEnd();
				TRACE("move�I��\n");
				bitFlg_.DashFlg = false;
				currentAnimation_ = "Look_Intro";
				animationFrame_ = 0.0f;
			}
		}
		TRACE("num:%f,pl_move:%f\n", tmpnum, move * 50);

		animationFrame_ += delta;
		//runAction(cocos2d::MoveBy::create(0.0f, cocos2d::Vec2(move, 0)));
		if (animationFrame_ >= 0.32f)
		{

			resShadow_->ResShadowEnd();
			bitFlg_.DashFlg = false;
			currentAnimation_ = "Look_Intro";
			animationFrame_ = 0.0f;
		}
		else
		{
			currentAnimation_ = "Dash";
		}
	}
}

void Player::colliderVisible(void)
{
	// �g�����X�t�H�[���̎��̓R���C�_�[�𖳎�����
	if (currentAnimation_ == "Transform" || currentAnimation_ == "Death")
	{
		return;
	}

	if (animationFrame_int_ <= 0)
	{
		animationFrame_int_ = 0;
	}
	//CCLOG("plHP:%d", hp_);
	if (collider_[playerColor + currentAnimation_].size() <= animationFrame_int_)
	{
		return;
	}
	currentCol_ = collider_[playerColor + currentAnimation_][animationFrame_int_];
	for (auto collider : this->getChildren())
	{
		if (playerColor + currentAnimation_ == collider->getName() &&
			animationFrame_int_ == collider->getTag())
		{
			// �U���̎������I�t�Z�b�g���K�v
			attackCollider("AttackFirst", collider, attackColOffset_);
			attackCollider("AttackSecond", collider, attackColOffset_);
			attackCollider("AttackThird", collider, attackColOffset_);

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

			//collider->setVisible(true);
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
				this->runAction(cocos2d::MoveTo::create(0.0f, cocos2d::Vec2(this->getPosition().x + AttackMove, this->getPosition().y)));
				//this->runAction(cocos2d::MoveTo::create(0.0f, cocos2d::Vec2(oldPos_ + AttackMove, this->getPosition().y)));
				this->SetIsAttacking(true);
			}
			else
			{
				this->SetAttackOffset(cocos2d::Vec2(0.0f, 0.0f));
				this->runAction(cocos2d::MoveTo::create(0.0f, cocos2d::Vec2(this->getPosition().x - AttackMove, this->getPosition().y)));
				//this->runAction(cocos2d::MoveTo::create(0.0f, cocos2d::Vec2(oldPos_ - AttackMove, this->getPosition().y)));
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
	lpAnimMng.AnimDataClear();	// �o�^�f�[�^�̏�����

	std::string path_light = "image/PlayerAnimationAsset/player/Light/player_Light";
	std::string path_dark = "image/PlayerAnimationAsset/player/Dark/player_Dark";

	// �A�j���[�V�������L���b�V���ɓo�^
	// non
	lpAnimMng.addAnimationCache(path_light, "Non", 6, (float)0.3, ActorType::Player,false);

	// idle
	lpAnimMng.addAnimationCache(path_light, "Look_Intro", 6, (float)0.3, ActorType::Player, false);

	// run
	lpAnimMng.addAnimationCache(path_light, "Run", 9, (float)0.08, ActorType::Player, false);

	// fall
	lpAnimMng.addAnimationCache(path_light, "Fall", 3, (float)1.0, ActorType::Player, false);

	// jump
	lpAnimMng.addAnimationCache(path_light, "Jump", 3, (float)0.05, ActorType::Player, false);

	lpAnimMng.addAnimationCache(path_light, "Jumping", 3, (float)0.05, ActorType::Player, false);

	// AttackFirst
	lpAnimMng.addAnimationCache(path_light, "AttackFirst", 9, (float)0.05, ActorType::Player, false);

	// AttackSecond
	lpAnimMng.addAnimationCache(path_light, "AttackSecond", 9, (float)0.08, ActorType::Player, false);

	// AttackThird
	lpAnimMng.addAnimationCache(path_light, "AttackThird", 10, (float)0.08, ActorType::Player, false);

	// wallslide
	lpAnimMng.addAnimationCache(path_light, "Wall_Slide", 3, (float)0.3, ActorType::Player, false);

	// Transform
	lpAnimMng.addAnimationCache(path_light, "Transform", 37, (float)0.05, ActorType::Player, false);

	// death
	lpAnimMng.addAnimationCache(path_light, "Death", 10, (float)0.08, ActorType::Player, false);

	// dash(���)
	lpAnimMng.addAnimationCache(path_light, "Dash", 4, (float)0.08, ActorType::Player, false);

	// non
	lpAnimMng.addAnimationCache(path_dark, "Non", 6, (float)0.3, ActorType::Player, false);

	// idle
	lpAnimMng.addAnimationCache(path_dark, "Look_Intro", 6, (float)0.3, ActorType::Player, false);

	// run
	lpAnimMng.addAnimationCache(path_dark, "Run", 9, (float)0.08, ActorType::Player, false);

	// fall
	lpAnimMng.addAnimationCache(path_dark, "Fall", 3, (float)1.0, ActorType::Player, false);

	// jump
	lpAnimMng.addAnimationCache(path_dark, "Jump", 3, (float)0.05, ActorType::Player, false);

	lpAnimMng.addAnimationCache(path_dark, "Jumping", 3, (float)0.05, ActorType::Player, false);

	// AttackFirst
	lpAnimMng.addAnimationCache(path_dark, "AttackFirst", 9, (float)0.05, ActorType::Player, false);

	// AttackSecond
	lpAnimMng.addAnimationCache(path_dark, "AttackSecond", 9, (float)0.08, ActorType::Player, false);

	// AttackThird
	lpAnimMng.addAnimationCache(path_dark, "AttackThird", 10, (float)0.08, ActorType::Player, false);

	// wallslide
	lpAnimMng.addAnimationCache(path_dark, "Wall_Slide", 3, (float)0.3, ActorType::Player, false);

	// Transform
	lpAnimMng.addAnimationCache(path_dark, "Transform", 37, (float)0.05, ActorType::Player, false);

	// death
	lpAnimMng.addAnimationCache(path_dark, "Death", 12, (float)0.08, ActorType::Player, false);

	// dash(���)
	lpAnimMng.addAnimationCache(path_dark, "Dash", 4, (float)0.08, ActorType::Player, false);

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

bool Player::GetGameOverFlg(void)
{
	return gameOverSceneFlg_;
}


void Player::actModuleRegistration(void)
{
	Vec2 charSize = { 15.0f * 3.0f,25.0f * 3.0f };
	// �E�ړ�
	{
		ActModule act;
		act.state = oprtState_;
		act.vel = Vec2{ 5,0 };
		act.actName = "Run";
		act.button = BUTTON::RIGHT;
		act.checkPoint1 = Vec2{ charSize.x/2, charSize.y/2 };	// �E��
		act.checkPoint2 = Vec2{ charSize.x/2,  15 };			// �E��
		act.touch = TOUCH_TIMMING::TOUCHING;	// �������ςȂ�
		//act.blackList.emplace_back(ACTION::FALLING);	// �������ɉE�ړ����Ăق����Ȃ��Ƃ��̒ǉ��̎d��

		//act.whiteList.emplace_back(ACTION::JUMPING);
		act.blackList.emplace_back("AttackFirst");
		act.blackList.emplace_back("AttackSecond");
		act.blackList.emplace_back("AttackThird");
		act.blackList.emplace_back("Wall_Slide");
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
		act.actName = "Run";
		act.button = BUTTON::LEFT;
		act.checkPoint1 = Vec2{ -charSize.x/2, charSize.y/2 };	// ����
		act.checkPoint2 = Vec2{ -charSize.x/2,  15 };			// ����
		act.touch = TOUCH_TIMMING::TOUCHING;    // �������ςȂ�

		//act.blackList.emplace_back(ACTION::FALLING);

		//act.whiteList.emplace_back(ACTION::JUMPING);
		act.blackList.emplace_back("AttackFirst");
		act.blackList.emplace_back("AttackSecond");
		act.blackList.emplace_back("AttackThird");
		act.blackList.emplace_back("Wall_Slide");
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
		flipAct.actName = "Non";
		flipAct.button = BUTTON::RIGHT;
		flipAct.touch = TOUCH_TIMMING::TOUCHING; // �������ςȂ�

		//flipAct.blackList.emplace_back(ACTION::FALLING);

		flipAct.blackList.emplace_back("AttackFirst");
		flipAct.blackList.emplace_back("AttackSecond");
		flipAct.blackList.emplace_back("AttackThird");
		flipAct.blackList.emplace_back("Wall_Slide");
		actCtl_.RunInitializeActCtl(type_,"�E����", flipAct);
	}

	// ���������]
	{
		ActModule flipAct;
		flipAct.state = oprtState_;
		flipAct.flipFlg = true;
		flipAct.actName = "Non";
		flipAct.button = BUTTON::LEFT;
		flipAct.touch = TOUCH_TIMMING::TOUCHING; // �������ςȂ�

		//flipAct.blackList.emplace_back(ACTION::FALLING);

		flipAct.blackList.emplace_back("AttackFirst");
		flipAct.blackList.emplace_back("AttackSecond");
		flipAct.blackList.emplace_back("AttackThird");
		flipAct.blackList.emplace_back("Wall_Slide");
		actCtl_.RunInitializeActCtl(type_,"������", flipAct);
	}

	// ����
	{
		// checkkeylist�ɗ����Ă���Ԃ̐ݒ���������Ǔ��Ɍ��ʂȂ�
		ActModule act;
		act.actName = "Fall";
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
		act.blackList.emplace_back("Jumping");	// �W�����v���ɗ������Ăق����Ȃ�
		act.blackList.emplace_back("Wall_Slide");	
		//act.blackList.emplace_back(ACTION::JUMP);	// �W�����v���ɗ������Ăق����Ȃ�

		actCtl_.RunInitializeActCtl(type_,"����", act);
	}

	// �W�����v
	{
		ActModule act;
		act.state = oprtState_;
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
		act.blackList.emplace_back("AttackFirst");
		act.blackList.emplace_back("AttackSecond");
		act.blackList.emplace_back("AttackThird");
		//act.whiteList.emplace_back(ACTION::RUN);

		actCtl_.RunInitializeActCtl(type_,"�W�����v", act);
	}

	// �W�����s���O
	{
		ActModule act;
		act.state = oprtState_;
		act.button = BUTTON::UP;
		act.actName = "Jumping";
		act.checkPoint1 = Vec2{ -charSize.x/3 + 5, charSize.y };	// ����
		act.checkPoint2 = Vec2{ charSize.x/3 - 5, charSize.y };		// �E��
		//act.checkPoint1 = Vec2{ -10, 30 };					// ����
		//act.checkPoint2 = Vec2{ +10, 30 };					// �E��
		act.jumpVel = Vec2{ 0.0f,20.0f };
		act.touch = TOUCH_TIMMING::TOUCHING;	// �������ςȂ�

		act.blackList.emplace_back("Fall");	// �������ɃW�����v���Ăق����Ȃ�
		act.blackList.emplace_back("Look_Intro");
		act.blackList.emplace_back("Run");
		act.blackList.emplace_back("AttackFirst");
		act.blackList.emplace_back("AttackSecond");
		act.blackList.emplace_back("AttackThird");
		act.blackList.emplace_back("Non");
		act.blackList.emplace_back("Wall_Slide");
		act.blackList.emplace_back("Transform");
		act.blackList.emplace_back("Dash");

		act.whiteList.emplace_back("Jump");
		actCtl_.RunInitializeActCtl(type_,"�W�����s���O", act);
		//actCtl_.ActCtl("�W�����v", act);
	}

	// �U��
	{
		ActModule act;
		act.state = oprtState_;
		//act.button = BUTTON::ATTACK;
		act.button = BUTTON::DOWN;
		act.actName = "AttackFirst";
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
		act.actName = "AttackSecond";
		act.blackList.emplace_back("Fall");
		act.touch = TOUCH_TIMMING::ON_TOUCH;	// �������u��
		actCtl_.RunInitializeActCtl(type_,"�U��", act);
	}

	// �U��3
	{
		ActModule act;
		act.state = oprtState_;
		act.button = BUTTON::DOWN;
		act.actName = "AttackThird";
		act.blackList.emplace_back("Fall");
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
		act.actName = "Wall_Slide";
		act.touch = TOUCH_TIMMING::TOUCHING;	
		act.blackList.emplace_back("Jumping");	
		act.blackList.emplace_back("Run");	
		act.blackList.emplace_back("Fall");
		act.blackList.emplace_back("Intro");
		act.blackList.emplace_back("Non");
		act.blackList.emplace_back("AttackFirst");
		act.blackList.emplace_back("AttackSecond");
		act.blackList.emplace_back("AttackThird");
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
		act.actName = "Wall_Slide";
		act.touch = TOUCH_TIMMING::TOUCHING;
		act.blackList.emplace_back("Jumping");
		act.blackList.emplace_back("Run");
		act.blackList.emplace_back("Fall");
		act.blackList.emplace_back("Intro");
		act.blackList.emplace_back("Non");
		act.blackList.emplace_back("AttackFirst");
		act.blackList.emplace_back("AttackSecond");
		act.blackList.emplace_back("AttackThird");
		actCtl_.RunInitializeActCtl(type_,"���ǃX���C�h", act);
	}

	// �X�V�֐��̓o�^
	actCtl_.InitUpdater(type_);
}

void Player::skillAction(void)
{
	// �����̃X�L����active�ł���Ƃ��́A�{�^�����������Ȃ��悤�ɂ���
	auto director = Director::getInstance();
	auto list = ((SkillBase*)director)->GetSkillList();
	bool flag = ((SkillBase*)director)->GetAllSkillActivate();

	if (!flag)
	{
		// �Ȃ񂩂ƂԂ��
		if (oprtState_->GetNowData()[static_cast<int>(BUTTON::SkillA)] && !oprtState_->GetOldData()[static_cast<int>(BUTTON::SkillA)])
		{
			bool skillFlg = ((SkillBase*)director)->GetSkillCT("magic");
			if (skillFlg)
			{
				lpSoundMng.PlayBySoundName("burst2");
				auto director = Director::getInstance();
				skillSprite = (SkillBase*)director->getRunningScene()->getChildByTag((int)zOlder::FRONT)->getChildByName("skillSprite");		//FLT_MAX : float �̍ő�̗L���l
				skillSprite->SetPlayerPos(getPosition());
				skillSprite->SetPlayerDirection(direction_);
				SkillBase* skill_t = SkillA::CreateSkillA(skillSprite);
				skillSprite->AddActiveSkill(skill_t);
				skillSprite->addChild(skill_t);
			}
			else
			{
				// �N�[���^�C����
			}
			//lpSkillMng.SkillActivate(plfile_[0]);
		}

		// ��(SkillB�ɂ���\��̂��)
		if (oprtState_->GetNowData()[static_cast<int>(BUTTON::SkillB)] && !oprtState_->GetOldData()[static_cast<int>(BUTTON::SkillB)])
		{
			bool skillFlg = ((SkillBase*)director)->GetSkillCT("enemySpawn");
			if (skillFlg)
			{
				lpSoundMng.PlayBySoundName("burst");
				auto director = Director::getInstance();
				skillSprite = (SkillBase*)director->getRunningScene()->getChildByTag((int)zOlder::FRONT)->getChildByName("skillSprite");		//FLT_MAX : float �̍ő�̗L���l
				auto ppos = getPosition();
				auto minpos = Vec2(0, 0);
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
				SkillBase* skill_t = SkillB::CreateSkillB(skillSprite);
				skillSprite->AddActiveSkill(skill_t);
				skillSprite->addChild(skill_t);
			}
			else
			{
				// �N�[���^�C����
			}

			//lpSkillMng.SkillActivate(plfile_[1]);
		}

		// HP��
		if (oprtState_->GetNowData()[static_cast<int>(BUTTON::SkillC)] && !oprtState_->GetOldData()[static_cast<int>(BUTTON::SkillC)])
		{
			bool skillFlg = ((SkillBase*)director)->GetSkillCT("heal");
			if (skillFlg)
			{
				lpSoundMng.PlayBySoundName("recovery");
				auto director = Director::getInstance();
				skillSprite = (SkillBase*)director->getRunningScene()->getChildByTag((int)zOlder::FRONT)->getChildByName("skillSprite");		//FLT_MAX : float �̍ő�̗L���l
				skillSprite->SetPlayerPos(getPosition());
				SkillBase* skill_t = SkillC::CreateSkillC(skillSprite);
				skillSprite->AddActiveSkill(skill_t);
				skillSprite->addChild(skill_t);

				// �񕜏���
				auto nowScene = ((Game*)Director::getInstance()->getRunningScene());
				auto hpGauge = (HPGauge*)nowScene->getChildByTag((int)zOlder::FRONT)->getChildByName("PL_HPgauge");
				hpGauge->SetHP(hpGauge->GetHP() + 50);	// �Ƃ肠��������
			}
			else
			{
				// �N�[���^�C����
			}

			//lpSkillMng.SkillActivate(plfile_[2]);
		}

		if (skillSprite != nullptr)
		{
			skillSprite->RemoveActiveSkill();
		}
	}
	else
	{
		// �����ɓ����Ă�����Aactive���̃X�L��������Ƃ������Ƃ�����A���̃X�L���������s�ɂȂ�
	}
}

bool Player::gameOverAction(void)
{
	if (!gameOverFlg_)
	{
		// ���̓������~�߂�
		stopAllActions();

		this->setPosition(deathPos_);	// ���S�����ʒu�Őݒ肵�Ă����Ȃ��ƁA���������Ȃ�
		AnimationCache* animationCache = AnimationCache::getInstance();
		Animation* animation = animationCache->getAnimation(playerColor + "Death");
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