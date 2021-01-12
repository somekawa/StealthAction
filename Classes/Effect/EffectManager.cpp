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
	// sprite����萔���߂Ă����߰ق̍쐬
	spritePool_ = new Vector<Sprite*>(EffectMaxNum);
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
		spritePool_->pushBack(sprite);
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

// effectManager�̓V���O���g����
void EffectManager::Update(void)
{

	//for (auto sprite = (*spritePool_).begin(); sprite != (*spritePool_).end(); sprite++)
	//{
	//	// isAnimEnd�̐ݒ�𖾓�����
	//	if ((*sprite)->)
	//	{

	//	}
	//}
}

void EffectManager::AddEffect(std::string effectName, int frame, float duration, Vec2 offset, Layer& layer, bool isMove)
{
	// �o�^����Ă��Ȃ���΁A��Ұ��݂̓o�^������
	if(effectAnimation_.find(effectName) == effectAnimation_.end())
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

}

void EffectManager::AddEffect(std::string effectName, int frame, float duration, cocos2d::Vec2 offset)
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
		Animation * animation = Animation::create();

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

}

void EffectManager::Play(std::string effectName,Vec2 pos)
{
	// ���ײ��߰ق̒��̓���̔Ԗڂ��߲�����擾
	auto curEffect = spritePool_->at(poolNo_);
	// �A�N�V�����̐ݒ�
	FiniteTimeAction* repeat = Repeat::create(Animate::create(effectAnimation_[effectName]), 1);

	// �R�[���o�b�N
	auto remove = CallFunc::create([&]() {
		CCLOG("animation remove");
		// ��Ұ��ݏI���̔���
		isAnimEnd_ = true;
		});

	// �A�N�V�����ƃR�[���o�b�N���V�[�P���X
	auto seq = Sequence::create(repeat, remove, nullptr);
	// �߰ق̔ԍ������Z
	poolNo_++;
	// �߰ق̔ԍ����̪�Ă̍ő吔�܂ŒB������0�ɖ߂�
	if (poolNo_ >= EffectMaxNum)
	{
		poolNo_ = 0;
	}
	// ���E���]
	curEffect->setFlippedX(flipFlag_);
	// �߰قɒǉ������̪�Ă��߼޼�ݐݒ�
	curEffect->setPosition(Vec2(pos.x + GetFlipOffset(effectName).x,pos.y + GetFlipOffset(effectName).y));

	// �߰قɒǉ������̪�Ė��̱���݂̎��s
	curEffect->runAction(seq);
	// �߰قɒǉ������̪�Ă�visible��true��
	curEffect->setVisible(true);
}


// �̪�Ă��Đ�����offset�������Ŏ��R�Ɍ��߂鎖���\�Ȃ悤��
//void EffectManager::Play(const EffectType& eType, cocos2d::Vec2 pos)
//{
//	// �߼޼�ݾ��
//	// autoRelease()�������\������
//	pos_ = pos;
//	setPosition(pos);
//	// ���߾��
//	type_ = eType;
//	// ��Ұ��ݺϐ����ĕϐ�������
//	animFrame_ = 0.0f;
//	// ��Ұ��݂��I�����������׸ޏ�����
//	isAnimEnd_ = false;
//	// ���̴݂̪�ı�Ұ����ް��̊i�[
//	animate_ = Animate::create(effectAnimation_[eType]);
//	// ���݂̱�Ұ����ް���runAction������
//	auto action = Repeat::create(Animate::create(effectAnimation_[eType]), 1);
//	runAction(action);
//}

//void EffectManager::Load(EffectType eType,int frame,float duration)
//{
//	std::string effectPath = "";
//	std::string effectName = "";
//	switch (eType)
//	{
//	case EffectType::PlayerSpawn:
//		// �̪�Ă�����path��
//		effectPath = "";
//		// �̪�ČŗL�̖��O
//		effectName = "playerSpawn";
//		break;
//	case EffectType::EnemySpawn:
//		// �̪�Ă�����path��
//		effectPath = "effect/enemySpawn";
//		// �̪�ČŗL�̖��O
//		effectName = "enemySpawn";
//		break;
//	case EffectType::PlayerAttack1:
//		effectPath = "";
//
//		effectName = "playerAttack1";
//		break;
//	case EffectType::PlayerAttack2:
//		effectPath = "";
//
//		effectName = "playerAttack2";
//		break;
//	case EffectType::Max:
//		break;
//	default:
//		break;
//	}
//	// �A�j���[�V�����L���b�V���̓V���O���g��
//	AnimationCache* animationCache = AnimationCache::getInstance();
//
//	//�X�v���C�g�V�[�g�̏���
//	auto cache = SpriteFrameCache::getInstance();
//
//	// �p�X�w��
//	cache->addSpriteFramesWithFile(effectPath + ".plist");
//
//	// �A�j���[�V�����摜�ǉ�
//	Animation* animation = Animation::create();
//
//	for (int i = 0; i < frame; i++)
//	{
//		auto string = effectName + "%d.png";		// plist�̒�������p�X����Ȃ�
//		auto str = StringUtils::format(string.c_str(), i);
//		SpriteFrame* sprite = cache->getSpriteFrameByName(str);
//
//		animation->addSpriteFrame(sprite);
//	}
//
//	// �A�j���[�V�����̊Ԋu
//	animation->setDelayPerUnit(duration);
//
//	// �A�j���[�V�����I����ɍŏ��ɖ߂����ǂ���
//	animation->setRestoreOriginalFrame(true);
//
//	// �o�����A�j���[�V�������L���b�V���ɓo�^
//	animationCache->addAnimation(animation,effectName);
//	// effectAnimation_�ɓo�^
//	effectAnimation_.try_emplace(eType, animation);
//}
