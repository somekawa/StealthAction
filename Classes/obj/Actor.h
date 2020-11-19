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
class Gravity;

class Actor :
	public cocos2d::Sprite
{
public:
	// �f�t�H���g�R���X�g���N�^�ǉ���
	//Actor();
	Actor();
	~Actor();
	// �eActor�̍s������
	virtual void Action(void) = 0;
	// �X�V����
	virtual void Update(void);
	virtual void update(float delta) = 0;
	// �A�j���[�V�����̍X�V
	virtual void UpdateAnimation(float delta);
	// �A�j���[�V�����̕ύX
	// param@ animName: �ύX��A�j���[�V������
	virtual void ChangeAnimation(std::string animName);

	// �����ύX
	virtual void ChangeDirection(void) = 0;
	// �A�j���[�V�����o�^
	virtual void AnimRegistrator(void) = 0;
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

	virtual const Vector2I& GetPos(void) const
	{
		return pos_;
	}

	virtual void SetPos(Vector2I pos);
	// ������K�v����̂�������Ȃ����A�F���g���Ǝv���A�쐬���܂���
	// �����l�Ō��݂̃t���[���l���擾
	virtual const int& GetAnimationFrameInt(void);

private:

protected:
	// �|�W�V����
	Vector2I pos_;
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
	// �R���C�_�[�{�b�N�X�̋�`�\����A�f�[�^��z�񂩂���o�����߁A
	// ��L�̃A�j���[�V�����t���[����int�^�����ɒ�����frame�l
	int animationFrame_int_;
	// �A�j���[�V�������I�����Ă��邩�ǂ����̃t���O
	bool isAnimEnd_;

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
	// �d��
	std::unique_ptr<Gravity> gravity_;
};