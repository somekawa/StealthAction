#pragma once

#include <cocos2d.h> 
#include <memory>
#include <list>

#include "Direction/Direction.h"

enum class ActorType
{
	Player,
	Imp,
	Max
};

class Input;
class Collider;

class Actor :
	public cocos2d::Sprite
{
public:
	Actor();
	~Actor();
	// �eActor�̍s������
	virtual void Action(void) = 0;
	// �X�V����
	virtual void Update(void);

	// ���g��sprite��Ԃ�
	virtual const cocos2d::Sprite* GetSprite(void) const
	{
		return sprite_;
	}
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

	const cocos2d::Vec2& GetPos(void) const
	{
		return pos_;
	}

private:
	// �I�u�W�F�N�g�������t���O
	bool deleteFlag_;
protected:
	// �|�W�V����
	cocos2d::Vec2 pos_;
	// ���g�̃T�C�Y
	cocos2d::Vec2 size_;
	// �X�s�[�h
	cocos2d::Vec2 speed_;
	// �X�v���C�g
	cocos2d::Sprite* sprite_;
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

	//std::list<std::unique_ptr<Collider>> collider_;
	Collider* collider_;
};