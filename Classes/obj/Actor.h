#pragma once

#include <cocos2d.h> 
#include <memory>
#include <list>
#include <unordered_map>
#include "Geometory.h"
#include "Direction/Direction.h"
#include "ActorType.h"

class Input;
class Collider;
class ActionRect;

class Actor :
	public cocos2d::Sprite
{
public:
	// �f�t�H���g�R���X�g���N�^�ǉ���
	Actor();
	Actor(std::unordered_map<std::string,
		std::vector<std::vector<std::shared_ptr<ActionRect>>>>&collider);
	~Actor();
	// �eActor�̍s������
	virtual void Action(void) = 0;
	// �X�V����
	virtual void Update(void);
	virtual void update(float delta) = 0;

	// �����ύX
	virtual void ChangeDirection(void) = 0;

	// �I�u�W�F�N�g�����񂾂�deleteFlag��true�ɂ���
	virtual void Delete(void)
	{
		deleteFlag_ = true;
	}
	// deleteFlag��true�ɂȂ��Ă���I�u�W�F�N�g�������Alist����������邽�߂Ɏg�p
	virtual const bool GetDeleteFlag(void)
	{
		return deleteFlag_;
	}

	virtual const Vector2F& GetPos(void) const
	{
		return pos_;
	}

private:

protected:
	// �|�W�V����
	Vector2F pos_;
	// ���g�̃T�C�Y
	Vector2I size_;
	// �X�s�[�h
	Vector2I speed_;
	// �L�����N�^�[�̃^�C�v
	ActorType type_;
	// ���݂̱�Ұ���
	std::string currentAnimation_;
	// ��Ұ��݂��Ă��鎞�͂����ڰѕϐ������Z
	float animationFrame_;
	// �摜���]�̃t���O
	cocos2d::FlipX* flipFlag_;

	// �����Ă������
	Direction direction_;

	// �I�u�W�F�N�g�������t���O
	bool deleteFlag_;
	//std::unordered_map<std::string,
	//	std::vector<std::vector<std::shared_ptr<ActionRect>>>>&collider_;

	std::unordered_map<std::string,
		std::vector<std::vector<std::shared_ptr<ActionRect>>>>collider_;

};