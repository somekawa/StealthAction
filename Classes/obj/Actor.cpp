// �f�[�^�ǂݍ��ݏ���(XmlActDataRead�֐��Asplit�֐�)��S��
#include "RapidXML/rapidxml.hpp"
#include "RapidXML/rapidxml_utils.hpp"
#include "_Debug/_DebugConOut.h"
#include "Scene/GameScene.h"
#include "Gravity.h"
#include "ActionRect.h"
#include "Skill/SkillMng.h"
#include "Actor.h"

USING_NS_CC;

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
	//onDamaged_ = false;
	animationFrame_ = 0.0f;
	// �������g���Q�Ƃœn���A����
	gravity_ = std::make_unique<Gravity>(*this);
	// �U���̍ۂɏo�������޼ު�Ă�ڲ԰
	attackLayer_ = Layer::create();
	// ��݂�ڲ԰��attackLayer_���Ԃ牺����
	myLayer.addChild(attackLayer_, 2, "attack");
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
	float duration = lpAnimMng.GetAnimationCache(type_, currentAnimation_)->getDuration();
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

void Actor::AnimRegistrator(void)
{
}

void Actor::SetPos(Vector2I pos)
{
	pos_ = pos;
}

void Actor::SetDirection(Direction dir)
{
	direction_ = dir;
}

const int Actor::GetAnimationFrameInt(void)
{
	// ���t���[�����Z�����l(animationFrame)��1�t���[���ɗv���鎞��(delayPerUnit)������
	// delayPerUnit�Ŋ���ƌ��݂̃t���[���l��int�Ŏ擾�\�@
	auto delay = lpAnimMng.GetAnimationCache(type_, currentAnimation_)->getDelayPerUnit();
	return static_cast<int>(animationFrame_ * 100.0f) / static_cast<int>(delay * 100.0f);
}

const int Actor::GetAnimationFrameInt(std::string str)
{
	auto delay = lpAnimMng.GetAnimationCache(type_, str)->getDelayPerUnit();
	return static_cast<int>(animationFrame_ * 100.0f) / static_cast<int>(delay * 100.0f);
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
	attackCol_ = { 0.0f, 0.0f, 0.0f, 0.0f };
	// ��Ұ�ދ�`���̾�۸�
	damageCol_ = { 0.0f, 0.0f, 0.0f, 0.0f };

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
					attackCol_.origin = getPosition() + (attackCol_.size / 2.0f);
				}
				else if (direction_ == Direction::Left)
				{
					// �U����`���߼޼��
					// direction����+ or - ���Ă��Ȃ��Ƃ����Ȃ�
					attackCol_.origin = Vec2(getPosition().x - (attackCol_.size.width / 2.0f), getPosition().y + attackCol_.size.height);
				}
				else
				{
					// �����������s��Ȃ�
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
				else
				{
					// �����������s��Ȃ�
				}
			}
		}
		else
		{
			// ��Ұ�ދ�`�̻���
			damageCol_.size = Size(colData.size_.x * getScale(), colData.size_.y * getScale());
			if (type_ == ActorType::Player)
			{
				// ��Ұ�ދ�`���߼޼��
				damageCol_.origin = Vec2(getPosition().x, getPosition().y);
			}
			else
			{
				if (direction_ == Direction::Left)
				{
					// ��Ұ�ދ�`���߼޼��
					damageCol_.origin = Vec2(getPosition().x + (damageCol_.size.width / 4.0f), getPosition().y);
				}
				else if (direction_ == Direction::Right)
				{
					// ��Ұ�ދ�`���߼޼��
					damageCol_.origin = Vec2(getPosition().x - (damageCol_.size.width / 4.0f), getPosition().y);
				}
				else
				{
					// �����������s��Ȃ�
				}
			}
		}
	}
}

bool Actor::OnHit(const cocos2d::Rect& collision)
{
	bool flg = false;

	if (collision.size.width > 0.0f && collision.size.height > 0.0f)
	{
		flg = true;
	}
	else
	{
		return false;
	}

	if (!isHitAttack_)
	{
		// ��`�Ԃ̋����𑪂�
		auto distance = Vec2(collision.origin.x - damageCol_.origin.x, collision.origin.y - damageCol_.origin.y);
		auto sizediff = Vec2((damageCol_.size.width / 2.0f) + (collision.size.width / 2.0f),
			(damageCol_.size.height / 2.0f) + (collision.size.height / 2.0f));

		if (abs(distance.x) <= sizediff.x && abs(distance.y) <= sizediff.y)
		{
			isHitAttack_ = true;
			return true;
		}
	}
	return false;
}

void Actor::SetIsHitAttack(bool flg)
{
	isHitAttack_ = flg;
}

void Actor::XmlActDataRead(std::string string, OPRT_state* oprt)
{
	std::string path = "../Resources/Data/moduleData/" + string + ".xml";

	// xml�ǂݍ���
	rapidxml::xml_document<> doc;
	rapidxml::file<> file(path.c_str());
	doc.parse<0>(file.data(), file.size());
	rapidxml::xml_node<>* parentNode = doc.first_node("data");

	// ���W���[������xml�f�[�^����擾
	auto sumData = parentNode->first_node("moduleSum")->first_attribute()->value();
	std::vector<ActModule> act;
	act.resize(std::atoi(sumData));

	std::map<std::string, std::function<void(std::string, int num)>> dataFunc;

	dataFunc["actName"] = [&](std::string str, int num) {
		act[num].actName = str;
	};

	dataFunc["state"] = [&](std::string str, int num) {
		act[num].state = oprt;
	};

	dataFunc["button"] = [&](std::string str, int num) {
		act[num].button = static_cast<BUTTON>(std::atoi(str.c_str()));
	};

	dataFunc["vel"] = [&](std::string str, int num) {
		auto string = split(str, ",");	// �J���}��؂菈��
		act[num].vel = Vec2(static_cast<float>(std::atof(string[0].c_str())), static_cast<float>(std::atof(string[1].c_str())));
	};

	dataFunc["checkPoint1"] = [&](std::string str, int num) {
		auto string = split(str, ",");
		act[num].checkPoint1 = Vec2(static_cast<float>(std::atof(string[0].c_str())), static_cast<float>(std::atof(string[1].c_str())));
	};

	dataFunc["checkPoint2"] = [&](std::string str, int num) {
		auto string = split(str, ",");
		act[num].checkPoint2 = Vec2(static_cast<float>(std::atof(string[0].c_str())), static_cast<float>(std::atof(string[1].c_str())));
	};

	dataFunc["checkPoint3"] = [&](std::string str, int num) {
		auto string = split(str, ",");
		act[num].checkPoint3 = Vec2(static_cast<float>(std::atof(string[0].c_str())), static_cast<float>(std::atof(string[1].c_str())));
	};

	dataFunc["checkPoint4"] = [&](std::string str, int num) {
		auto string = split(str, ",");
		act[num].checkPoint4 = Vec2(static_cast<float>(std::atof(string[0].c_str())), static_cast<float>(std::atof(string[1].c_str())));
	};

	dataFunc["touch"] = [&](std::string str, int num) {
		act[num].touch = static_cast<TOUCH_TIMMING>(std::atoi(str.c_str()));
	};

	dataFunc["gravity"] = [&](std::string str, int num) {
		auto string = split(str, ",");
		act[num].gravity = Vec2(static_cast<float>(std::atof(string[0].c_str())), static_cast<float>(std::atof(string[1].c_str())));
	};

	dataFunc["jumpVel"] = [&](std::string str, int num) {
		auto string = split(str, ",");
		act[num].jumpVel = Vec2(static_cast<float>(std::atof(string[0].c_str())), static_cast<float>(std::atof(string[1].c_str())));
	};

	dataFunc["flipFlg"] = [&](std::string str, int num) {
		act[num].flipFlg = static_cast<bool>(std::atoi(str.c_str()));
	};

	dataFunc["blackList"] = [&](std::string str, int num) {
		act[num].blackList.emplace_back(str);
	};

	dataFunc["whiteList"] = [&](std::string str, int num) {
		act[num].whiteList.emplace_back(str);
	};

	int num = 0;
	for (rapidxml::xml_node<char>* moduleItr = parentNode->first_node("module"); moduleItr != nullptr; moduleItr = moduleItr->next_sibling())
	{
		for (rapidxml::xml_attribute<char>* attr = moduleItr->first_node("act")->first_attribute(); attr; attr = attr->next_attribute())
		{
			// blackList�ȊO�̏���(name : =�̍���,value : =�̉E��("�Z�Z"�̒��g))
			dataFunc[attr->name()](attr->value(), num);
		}
		for (rapidxml::xml_node<char>* itr = moduleItr->first_node("blackList")->first_node("name"); itr != nullptr; itr = itr->next_sibling())
		{
			// blackList�̏���(blackList�ɓo�^������actName�������Ă���)
			dataFunc["blackList"](itr->value(), num);
		}
		for (rapidxml::xml_node<char>* itr = moduleItr->first_node("whiteList")->first_node("name"); itr != nullptr; itr = itr->next_sibling())
		{
			// whiteList�̏���(whiteList�ɓo�^������actName�������Ă���)
			dataFunc["whiteList"](itr->value(), num);
		}

		// �A�N�V�����̓o�^
		actCtl_.InitAct(type_, static_cast<AnimationType>(num + 1), act[num]);
		num++;
	}

	// �X�V�֐��̓o�^
	actCtl_.InitUpdater(type_);
}

std::vector<std::string> Actor::split(std::string str, std::string separator)
{
	if (separator == "") return { str };
	std::vector<std::string> result;
	std::string tstr = str + separator;
	auto l = tstr.length(), sl = separator.length();
	std::string::size_type pos = 0, prev = 0;

	for (; pos < l && (pos = tstr.find(separator, pos)) != std::string::npos; prev = (pos += sl))
	{
		result.emplace_back(tstr, prev, pos - prev);
	}
	return result;
}