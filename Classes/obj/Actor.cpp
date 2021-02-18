#include "Actor.h"
#include "Scene/GameScene.h"
#include "Gravity.h"
#include "_Debug/_DebugConOut.h"
#include "ActionRect.h"
#include "Skill/SkillMng.h"

USING_NS_CC;

//Actor::Actor()
//{
//}

Actor::Actor(int hp,Layer& myLayer):hp_(hp),myLayer_(myLayer)
{
	deleteFlag_ = false;
	isAnimEnd_ = false;
	onFloor_ = false;
	isHitWall_ = false;
	isFire_ = false;
	isHitAttack_ = false;
	// �����Ă��邩���׸ނ̏�����
	isAlive_ = true;
	// �U�����Ă��邩�̃t���O�̏�����
	isAttacking_ = false;
	// �_���[�W����������t���O�̏�����
	onDamaged_ = false;
	animationFrame_ = 0.0f;
	// �������g���Q�Ƃœn���A����
	gravity_ = std::make_unique<Gravity>(*this);

	// �U���̍ۂɏo�������޼ު�Ă�ڲ԰
	attackLayer_ = Layer::create();
	// ��݂�ڲ԰��attackLayer_���Ԃ牺����
	myLayer.addChild(attackLayer_, 2, "attack");

	std::list<std::string> path;
	path.push_back("skill_data");
	fileLoad_ = lpFileLoder.Directory(path);							// player��enemy�̊K�w
	// �������̂��߂�changealldata���g�p����
	for (auto data : fileLoad_)
	{
		// �f�[�^�̃e�X�g
		lpSkillMng.ChangeAllData(data.first, { "dataTest",1,0,180 });
	}
}

Actor::~Actor()
{
}

void Actor::Update(void)
{
	Action();
	lpSkillMng.UpDate();
}

// �A�j���[�V�����̍X�V�֐�
void Actor::UpdateAnimation(float delta)
{
	// �A�j���[�V�����J�E���g�𖈃t���[��delta�l�����Z
	animationFrame_ += delta;
	// ����A�j���[�V�����̏I���܂ł̌p�����Ԃ̊i�[
	auto duration = lpAnimMng.GetAnimationCache(type_, currentAnimation_)->getDuration();
	// �A�j���[�V�����J�E���g���p�����Ԃ𒴂��Ă����
	if (animationFrame_ >= duration)
	{
		// ���[�v�t���O��true�̏ꍇ�̓��[�v�Đ�
		if (lpAnimMng.GetIsLoop(type_, currentAnimation_))
		{
			animationFrame_ = 0.0f;
			isAttacking_ = false;
		}
		else
		{
			// false�̏ꍇ��1��̍Đ�
			// 1�A�j���[�V�������I�������t���O�𗧂Ă�
			isAnimEnd_ = true;
		}
	}
	TRACE("animEndFlag:%d\n", isAnimEnd_);
}

void Actor::ChangeAnimation(std::string animName)
{
	// ���̓������~�߂�
	this->stopAllActions();

	// ���݂̃A�j���[�V������ύX��̃A�j���[�V�������ɕύX
	currentAnimation_ = animName;
	animationFrame_ = 0.0f;
	animationFrame_int_ = 0;
	// �A�j���[�V�����I���t���O��false��
	isAnimEnd_ = false;

	if (lpAnimMng.GetIsLoop(type_, currentAnimation_))
	{
		auto action = RepeatForever::create(Animate::create(lpAnimMng.GetAnimationCache(type_, currentAnimation_)));
		this->runAction(action);
	}
	else
	{
		this->runAction(Animate::create(lpAnimMng.GetAnimationCache(type_, currentAnimation_)));
	}
}

std::string Actor::GetAction(void)
{
	return currentAnimation_;
}

void Actor::SetAction(std::string actName)
{
	currentAnimation_ = actName;
}

void Actor::SetPos(Vector2I pos)
{
	pos_ = pos;
}

void Actor::SetDirection(Direction dir)
{
	direction_ = dir;
}

const int& Actor::GetAnimationFrameInt(void)
{
	// ���t���[�����Z�����l(animationFrame)��1�t���[���ɗv���鎞��(delayPerUnit)������
	// delayPerUnit�Ŋ���ƌ��݂̃t���[���l��int�Ŏ擾�\�@
	auto delay = lpAnimMng.GetAnimationCache(type_, currentAnimation_)->getDelayPerUnit();
	auto val = (int)(animationFrame_ * 100.0f) / (int)(delay * 100.0f);
	return val;
}

const int& Actor::GetAnimationFrameInt(std::string str)
{
	// ���t���[�����Z�����l(animationFrame)��1�t���[���ɗv���鎞��(delayPerUnit)������
	// delayPerUnit�Ŋ���ƌ��݂̃t���[���l��int�Ŏ擾�\�@
	auto delay = lpAnimMng.GetAnimationCache(type_, str)->getDelayPerUnit();
	auto val = (int)(animationFrame_ * 100.0f) / (int)(delay * 100.0f);
	return val;
}

void Actor::CheckMapObjHit(float delta)
{
	auto director = Director::getInstance();
	auto a = director->getRunningScene()->getChildByTag((int)zOlder::BG);
	if ((TMXLayer*)director->getRunningScene()->getChildByTag((int)zOlder::BG)->getChildByName("MapData") == nullptr)
	{
		return;
	}
	auto position = this->getPosition();

	auto CollisionData = (TMXLayer*)director->getRunningScene()->getChildByTag((int)zOlder::BG)->getChildByName("MapData")->getChildByName("col");
	// �R���W�������C���[�̏c��
	auto ColSize = CollisionData->getLayerSize();
	const int chipSize = CollisionData->getMapTileSize().width;
	auto colSize = Vec2::ZERO;

	for (auto col : currentCol_)
	{
		if (col->GetData().type_ == 1)
		{
			colSize = Vec2{ (float)col->GetData().size_.x,(float)col->GetData().size_.y };
		}
	}
	this->setAnchorPoint({ 0.5f,0.0f });
	// �����ׂ̗��ǂ������łȂ���
	auto checkID = [&](Vec2 point) {
		auto gridPos = Vec2{ point } / chipSize;
		auto bottom = Vec2(gridPos.x, ColSize.height - gridPos.y);
		auto posGid = CollisionData->getTileGIDAt(bottom);
		if (posGid != 0)
		{
			// �␳���K�v�ȂƂ���true
			return true;
		}
		else
		{
			return false;
		}
		return false;
	};
	// �����̃|�W�V����
	auto bottomPos = Vec2{ position.x,position.y - (0.3f * (delta * 50)) };
	// �����̉��ׂ̃|�W�V����
	// speed_��dir�ɂ����-�ɂȂ�����+�ɂȂ����肷��̂ŕϐ������p
	auto nextPos = Vec2{ position.x + speed_.x,position.y };
	// �����̃|�W�V��������ɏ��ڒP�ʂɒ���
	auto nextYGrid = Vec2{ bottomPos.x,bottomPos.y } / chipSize;
	// ���ׂ̃|�W�V��������ɏ��ڒP�ʂɒ���
	auto nextXGrid = Vec2{ nextPos.x,nextPos.y } / chipSize;

	auto gridYPos = Vec2(nextYGrid.x, ColSize.height - nextYGrid.y);
	auto gridYGid = CollisionData->getTileGIDAt(gridYPos);

	auto gridXPos = Vec2(nextXGrid.x, ColSize.height - nextXGrid.y);
	auto gridXGid = CollisionData->getTileGIDAt(gridXPos);
	// �͈͊Ocheck
	if (gridXPos.x > ColSize.width || gridXPos.x < 0 ||
		gridXPos.y > ColSize.height || gridXPos.y < 0)
	{
		return;
	}
	// �͈͊Ocheck
	if (gridYPos.x > ColSize.width || gridYPos.x < 0 ||
		gridYPos.y > ColSize.height || gridYPos.y < 0)
	{
		return;
	}
	if (gridXGid != 0)
	{
		isHitWall_ = true;
	}
	else
	{
		isHitWall_ = false;
	}
	if (gridYGid != 0)
	{
		onFloor_ = true;
	}
	else
	{
		onFloor_ = false;
	}
}

void Actor::SetIsAttacking(bool flg)
{
	isAttacking_ = flg;
}

void Actor::SetAttackOffset(cocos2d::Vec2 offset)
{
	attackRect_.offset_ = offset;
}

void Actor::SetCollider(void)
{
	// �U����`���̾�۸�
	attackCol_ = cocos2d::Rect(0, 0, 0, 0);
	// ��Ұ�ދ�`���̾�۸�
	damageCol_ = cocos2d::Rect(0, 0, 0, 0);

	for (auto col : currentCol_)
	{
		// ���݂̺ؼޮ݂��ް����i�[���Ă���
		auto colData = col->GetData();
		// �U����`��������
		if (colData.type_ == 0)
		{
			// �U����`�̻���
			attackCol_.size = Size(colData.size_.x * getScale(), colData.size_.y * getScale());
			// ��ڲ԰��������
			if (type_ == ActorType::Player)
			{
				// �����Ă���������߼޼�݂̕ύX��������
				if (direction_ == Direction::Right)
				{
					// �U����`���߼޼��
					// direction����+ or - ���Ă��Ȃ��Ƃ����Ȃ�
					attackCol_.origin = getPosition() + (attackCol_.size/2);
				}
				else if (direction_ == Direction::Left)
				{
					// �U����`���߼޼��
					// direction����+ or - ���Ă��Ȃ��Ƃ����Ȃ�
					attackCol_.origin = Vec2(getPosition().x - (attackCol_.size.width/2/* + 15.0f*/), getPosition().y + attackCol_.size.height);
				}
			}
			else
			{
				if (direction_ == Direction::Right)
				{
					// �U����`���߼޼��
					// direction����+ or - ���Ă��Ȃ��Ƃ����Ȃ�
					attackCol_.origin = getPosition() + attackCol_.size;
				}
				else if (direction_ == Direction::Left)
				{
					// �U����`���߼޼��
					// direction����+ or - ���Ă��Ȃ��Ƃ����Ȃ�
					attackCol_.origin = Vec2(getPosition().x - attackCol_.size.width,getPosition().y + attackCol_.size.height);
				}
			}
			/*auto damagedraw = DrawNode::create();
			damagedraw->drawRect(attackCol_.origin, attackCol_.origin - attackCol_.size, Color4F::GRAY);
			addChild(damagedraw);*/
		}
		else
		{
			// ��Ұ�ދ�`�̻���
			damageCol_.size = Size(colData.size_.x * getScale(), colData.size_.y * getScale());
			if (type_ == ActorType::Player)
			{
				// ��Ұ�ދ�`���߼޼��
				damageCol_.origin = Vec2(getPosition().x, getPosition().y);
				/*auto damagedraw = DrawNode::create();
				damagedraw->drawDot(damageCol_.origin,3.0f, Color4F::GRAY);
				addChild(damagedraw);*/
			}
			else
			{
				if (direction_ == Direction::Left)
				{
					// ��Ұ�ދ�`���߼޼��
					damageCol_.origin = Vec2(getPosition().x + (damageCol_.size.width / 4), getPosition().y);
				}
				else if (direction_ == Direction::Right)
				{
					// ��Ұ�ދ�`���߼޼��
					damageCol_.origin = Vec2(getPosition().x - (damageCol_.size.width / 4), getPosition().y);
				}
			}
		}
	}
}

bool Actor::OnHit(const cocos2d::Rect& collision)
{
	auto flg = false;

	if (collision.size.width > 0.0f && collision.size.height > 0.0f)
	{
		flg = true;
	}
	// ��`�Ԃ̋����𑪂�
	auto distance = Vec2(collision.origin.x - damageCol_.origin.x, collision.origin.y - damageCol_.origin.y);
	auto sizediff = Vec2((damageCol_.size.width / 2.0f) + (collision.size.width / 2.0f),
					     (damageCol_.size.height / 2.0f) + (collision.size.height / 2.0f));
	if (flg)
	{
		if (!isHitAttack_)
		{
			if (abs(distance.x) <= sizediff.x && abs(distance.y) <= sizediff.y)
			{
				//onDamaged_ = true;
				isHitAttack_ = true;
				return true;
			}
		}
	}
	return false;
}

void Actor::SetIsHitAttack(bool flg)
{
	isHitAttack_ = flg;
}

