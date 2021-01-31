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
	// �߰ق̗e�ʂ܂ŉ�
	for (int num = 0; num < EffectMaxNum; num++)
	{
		// ��̽��ײĂ̐���
		sprite = Sprite::create();
		// �܂������Ȃ�����
		sprite->setVisible(false);
		// �̪�ėpڲ԰��addChild
		layer.addChild(sprite);
		// ���ײ��߰قɋ�̽��ײĂ�����
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

void EffectManager::Move(FXStruct fx)
{
	auto move = MoveBy::create(0.0f, Vec2(fx.speed_.x,fx.speed_.y));
	fx.sprite_->runAction(move);
}

const FXStruct& EffectManager::PickUp(std::string effectName,
											cocos2d::Vec2 offset,cocos2d::Vec2 pos, cocos2d::Vec2 speed,
										    int frame, float duration, bool flipFlag, bool loop)
{
	// �o�^����Ă��Ȃ���΁A��Ұ��݂̓o�^������
	if (effectAnimation_.find(effectName) == effectAnimation_.end())
	{
		// �̪�Ẳ摜�߽
		std::string effectPath = "effect/" + effectName;

		// �A�j���[�V�����L���b�V���̓V���O���g��
		AnimationCache* animationCache = AnimationCache::getInstance();

		//�X�v���C�g�V�[�g�̏���
		auto cache = SpriteFrameCache::getInstance();

		// �p�X�w��
		cache->addSpriteFramesWithFile(effectPath + ".plist");
		// �A�j���[�V�����摜�ǉ�
		Animation* animation = Animation::create();

		for (int i = 0; i < frame; i++)
		{
			auto string = effectName + "%d.png";		// plist�̒�������p�X����Ȃ�
			auto str = StringUtils::format(string.c_str(), i);
			SpriteFrame* sprite = cache->getSpriteFrameByName(str);

			animation->addSpriteFrame(sprite);
		}

		// �A�j���[�V�����̊Ԋu
		animation->setDelayPerUnit(duration);

		// �A�j���[�V�����I����ɍŏ��ɖ߂����ǂ���
		animation->setRestoreOriginalFrame(true);

		// �o�����A�j���[�V�������L���b�V���ɓo�^
		animationCache->addAnimation(animation, effectName);

		// �̪�Ė��̱�Ұ����ް��̕ۑ�
		effectAnimation_.emplace(effectName, animationCache->getAnimation(effectName));
		// �̪�Ė��̵̾�Ēl�̕ۑ�
		offset_.emplace(effectName, offset);
	}

	// ���ײ��߰ق̒��̓���̔Ԗڂ��߲�����擾
	while (spritePool_.at(poolNo_).isActive_)
	{
		// �߰ق̔ԍ������Z
		poolNo_++;
		// �߰ق̔ԍ����̪�Ă̍ő吔�܂ŒB������0�ɖ߂�
		if (poolNo_ >= EffectMaxNum)
		{
			poolNo_ = 0;
		}
	}

	spritePool_.at(poolNo_).isActive_ = true;
	// �߰ق̒����籸è�ޏ�Ԃł͂Ȃ���̒��o
	// ���]�׸ނ̾��
	SetFlip(flipFlag);
	// ��Ұ��ݏI���׸ނ̏�����
	spritePool_.at(poolNo_).isAnimEnd_ = false;
	// ٰ���׸ނ̏�����
	spritePool_.at(poolNo_).isLoop_ = loop;
	// �̪�ı�Ұ��݂��ڰѶ��ĕϐ��̏�����
	spritePool_.at(poolNo_).frameCnt_ = 0.0f;
	// ���ײĂ̖��O���
	spritePool_.at(poolNo_).sprite_->setName(effectName);
	// ���o����poolNo_����ނƂ��ľ��
	spritePool_.at(poolNo_).sprite_->setTag(poolNo_);
	// ���E���]
	spritePool_.at(poolNo_).sprite_->setFlippedX(flipFlag_);
	// �߰قɒǉ������̪�Ă��߼޼�ݐݒ�
	spritePool_.at(poolNo_).sprite_->setPosition(Vec2(pos.x + GetFlipOffset(effectName).x, pos.y + GetFlipOffset(effectName).y));
	// �߰قɒǉ������̪�Ă�visible��true��
	spritePool_.at(poolNo_).sprite_->setVisible(true);
	// ��߰�ނ̐ݒ�
	spritePool_.at(poolNo_).speed_ = speed;
	// �̪�Ă̱�è�ޏ�Ԃ�true��
	spritePool_.at(poolNo_).isActive_ = true;

	Play(effectName, spritePool_.at(poolNo_).sprite_, loop);
	return spritePool_.at(poolNo_);
}

void EffectManager::Play(std::string effectName,Sprite* sprite, bool loop)
{
	if (loop)
	{
		// ����݂̐ݒ�(loop)
		FiniteTimeAction* repeat = RepeatForever::create(Animate::create(effectAnimation_[effectName]));
		sprite->runAction(repeat);
	}
	else
	{
		// ����݂̐ݒ�(non-loop)
		FiniteTimeAction* repeat = Repeat::create(Animate::create(effectAnimation_[effectName]), 1);
		sprite->runAction(repeat);
	}
}

void EffectManager::Update(float delta)
{
	for (auto& member : spritePool_)
	{
		if (member.isActive_)
		{
			Move(member);
			member.frameCnt_ += delta;
			auto duration = effectAnimation_[member.sprite_->getName()]->getDuration();
			if (member.frameCnt_ >= duration)
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
}

void EffectManager::Reset(const FXStruct& fx)
{
	for (auto& fxData : spritePool_)
	{
		if (!fx.isActive_)
		{
			if (fx.sprite_->getTag() == fxData.sprite_->getTag())
			{
				fxData.isActive_ = false;
			}
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
	// ��è�ޏ�Ԃ���
	if (fx.isActive_)
	{
		// �̪�Ă��ڰт�delta�l�ŉ��Z
		fx.frameCnt_ += delta;
		// �e�̪�Ă̑��ڰѐ��̎擾
		auto duration = effectAnimation_[fx.sprite_->getName()]->getDuration();
		// �̪�Ă��ڰт����ڰѐ��𒴂��Ă�����
		if (fx.frameCnt_ >= duration)
		{
			// ���̴̪�Ă�ٰ�ߍĐ���p�̴̪�ĂȂ��
			if (fx.isLoop_)
			{
				// ٰ�ߍĐ��\�Ȃ悤��frame���۸�
				fx.frameCnt_ = 0.0f;
			}
			// 1��݂̂̍Đ��Ȃ��
			else
			{
				// ��è�ޏ�Ԃ�false��
				fx.isActive_ = false;
			}
			return true;
		}

	}
	return false;
}
