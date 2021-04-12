#include "EffectManager.h"

USING_NS_CC;

EffectManager::~EffectManager()
{

}

void EffectManager::CreatePools(cocos2d::Layer& layer)
{
	// poolNo_�̏�����
	poolNo_ = 0;

	Sprite* sprite;
	// �v�[���̗e�ʂ܂ŉ�
	for (int num = 0; num < EffectMaxNum; num++)
	{
		// ��̃X�v���C�g�̐���
		sprite = Sprite::create();
		// �܂������Ȃ�����
		sprite->setVisible(false);
		// �G�t�F�N�g�p���C���[��addChild
		layer.addChild(sprite);
		// �X�v���C�g�v�[���ɋ�̃X�v���C�g������
		spritePool_.push_back(FXStruct(sprite, false, false, false, 0.0f, {0.0f,0.0f}));
	}
}

void EffectManager::SetFlip(bool flg)
{
	flipFlag_ = flg;
}

cocos2d::Vec2 EffectManager::GetFlipOffset(std::string effectName)
{
	Vec2 offset = {};
	// ���E���]���Ă���
	if (flipFlag_)
	{
		offset = offset_[effectName];
	}
	// ���]���Ă��Ȃ�
	else
	{
		offset = Vec2{ -offset_[effectName].x,offset_[effectName].y };
	}
	return offset;
}

void EffectManager::ClearPool()
{
	spritePool_.clear();
}

void EffectManager::Move(FXStruct fx)
{
	const auto move = MoveBy::create(0.0f, Vec2(fx.speed_.x,fx.speed_.y));
	fx.sprite_->runAction(move);
}

const FXStruct& EffectManager::PickUp(std::string effectName,
											cocos2d::Vec2 offset,cocos2d::Vec2 pos, cocos2d::Vec2 speed,
										    int frame, float duration, bool flipFlag, bool loop,bool animReset)
{
	// �o�^����Ă��Ȃ���΁A�A�j���[�V�����̓o�^������
	if (effectAnimation_.find(effectName) == effectAnimation_.end())
	{
		// �G�t�F�N�g�̉摜�p�X
		std::string effectPath = "effect/" + effectName;

		// �A�j���[�V�����L���b�V���̓V���O���g��
		AnimationCache* animationCache = AnimationCache::getInstance();

		//�X�v���C�g�V�[�g�̏���
		const auto cache = SpriteFrameCache::getInstance();

		// �p�X�w��
		cache->addSpriteFramesWithFile(effectPath + ".plist");
		// �A�j���[�V�����摜�ǉ�
		Animation* animation = Animation::create();

		for (int i = 0; i < frame; i++)
		{
			const auto string = effectName + "%d.png";		// plist�̒�������p�X����Ȃ�
			const auto str = StringUtils::format(string.c_str(), i);
			SpriteFrame* sprite = cache->getSpriteFrameByName(str);

			animation->addSpriteFrame(sprite);
		}

		// �A�j���[�V�����̊Ԋu
		animation->setDelayPerUnit(duration);

		// �A�j���[�V�����I����ɍŏ��ɖ߂����ǂ���
		animation->setRestoreOriginalFrame(animReset);

		// �o�����A�j���[�V�������L���b�V���ɓo�^
		animationCache->addAnimation(animation, effectName);

		// �G�t�F�N�g���̃A�j���[�V�����f�[�^�̕ۑ�
		effectAnimation_.emplace(effectName, animationCache->getAnimation(effectName));
		// �G�t�F�N�g���̃I�t�Z�b�g�l�̕ۑ�
		offset_.emplace(effectName, offset);
	}

	// �X�v���C�g�v�[���̒��̓���̔Ԗڂ̃|�C���^���擾
	while (spritePool_.at(poolNo_).isActive_)
	{
		// �v�[���̔ԍ������Z
		poolNo_++;
		// �v�[���̔ԍ����G�t�F�N�g�̍ő吔�܂ŒB������0�ɖ߂�
		if (poolNo_ >= EffectMaxNum)
		{
			poolNo_ = 0;
		}
	}

	spritePool_.at(poolNo_).isActive_ = true;
	// �v�[���̒�����A�N�e�B�u��Ԃł͂Ȃ���̒��o
	// ���]�t���O�̃Z�b�g
	SetFlip(flipFlag);
	// �A�j���[�V�����I���t���O�̏�����
	spritePool_.at(poolNo_).isAnimEnd_ = false;
	// ���[�v�t���O�̏�����
	spritePool_.at(poolNo_).isLoop_ = loop;
	// �G�t�F�N�g�A�j���[�V�����̃t���[���J�E���g�ϐ��̏�����
	spritePool_.at(poolNo_).frameCnt_ = 0.0f;
	// �X�v���C�g�̖��O�Z�b�g
	spritePool_.at(poolNo_).sprite_->setName(effectName);
	// ���o����poolNo_���^�O�Ƃ��ăZ�b�g
	spritePool_.at(poolNo_).sprite_->setTag(poolNo_);
	// ���E���]
	spritePool_.at(poolNo_).sprite_->setFlippedX(flipFlag_);
	// �v�[���ɒǉ������G�t�F�N�g�̃|�W�V�����ݒ�
	spritePool_.at(poolNo_).sprite_->setPosition(Vec2(pos.x + GetFlipOffset(effectName).x, pos.y + GetFlipOffset(effectName).y));
	// �v�[���ɒǉ������G�t�F�N�g��visible��true��
	spritePool_.at(poolNo_).sprite_->setVisible(true);
	// �X�s�[�h�̐ݒ�
	spritePool_.at(poolNo_).speed_ = speed;
	// �G�t�F�N�g�̃A�N�e�B�u��Ԃ�true��
	spritePool_.at(poolNo_).isActive_ = true;

	Play(effectName, spritePool_.at(poolNo_).sprite_, loop);
	return spritePool_.at(poolNo_);
}

void EffectManager::Play(std::string effectName,Sprite* sprite, bool loop)
{
	if (loop)
	{
		// �A�N�V�����̐ݒ�(loop)
		FiniteTimeAction* repeat = RepeatForever::create(Animate::create(effectAnimation_[effectName]));
		sprite->runAction(repeat);
	}
	else
	{
		// �A�N�V�����̐ݒ�(non-loop)
		FiniteTimeAction* repeat = Repeat::create(Animate::create(effectAnimation_[effectName]), 1);
		sprite->runAction(repeat);
	}
}

void EffectManager::Update(float delta)
{
	for (auto& member : spritePool_)
	{
		if (!member.isActive_)
		{
			continue;
		}

		Move(member);
		member.frameCnt_ += delta;
		if (member.frameCnt_ >= effectAnimation_[member.sprite_->getName()]->getDuration())
		{
			if (member.isLoop_)
			{
				member.frameCnt_ = 0.0f;
				member.isAnimEnd_ = true;
			}
			else
			{
				member.isActive_ = false;
			}
		}
	}
}

void EffectManager::Reset(const FXStruct& fx)
{
	for (auto& fxData : spritePool_)
	{
		// �G�t�F�N�g���Đ����̏ꍇ�͏������s��Ȃ�
		if (fx.isActive_)
		{
			continue;
		}

		if (fx.sprite_->getTag() == fxData.sprite_->getTag())
		{
			fxData.isActive_ = false;
		}
	}
}

void EffectManager::PlayWithOnce(FXStruct& fx,std::string effectName)
{
	// �A�N�V�����̐ݒ�
	FiniteTimeAction* repeat = Repeat::create(Animate::create(effectAnimation_[effectName]), 1);

	fx.sprite_->runAction(repeat);
}

void EffectManager::PlayWithLoop(FXStruct& fx,std::string effectName)
{
	// �A�N�V�����̐ݒ�
	FiniteTimeAction* repeat = RepeatForever::create(Animate::create(effectAnimation_[effectName]));

	fx.sprite_->runAction(repeat);
}

bool EffectManager::AnimEndChecker(FXStruct& fx,float delta)
{
	// �A�N�e�B�u��Ԃ���
	if (fx.isActive_)
	{
		// �G�t�F�N�g�̃t���[����delta�l�ŉ��Z
		fx.frameCnt_ += delta;

		// �G�t�F�N�g�̃t���[�������t���[�����𒴂��Ă��Ȃ��ꍇ�͏������s��Ȃ�
		if (fx.frameCnt_ < effectAnimation_[fx.sprite_->getName()]->getDuration())
		{
			return false;
		}

		// ���̃G�t�F�N�g�����[�v�Đ���p�̃G�t�F�N�g�Ȃ��
		if (fx.isLoop_)
		{
			// ���[�v�Đ��\�Ȃ悤��frame���[���N���A
			fx.frameCnt_ = 0.0f;
		}
		else
		{
			// 1��݂̂̍Đ��Ȃ�΃A�N�e�B�u��Ԃ�false��
			fx.isActive_ = false;
		}
		return true;
	}
	return false;
}