#pragma once
#include <cocos2d.h>
#include <string>
#include "Geometory.h"
#include "../ActorType.h"

class Actor;

class Obj :
	public cocos2d::Sprite
{
public:
	Obj(Actor& target);
	~Obj();

	void update(float delta) = 0;
	// ����
	virtual void Move(void) = 0;
	// �e��Ұ��݂̓o�^
	virtual void AnimRegistrator(void) = 0;
	// ��Ұ��݂̏�����
	virtual bool InitAnim(void);
	// ��΂����Ώۂɓ����������ǂ����������֐�
	// �������Ă����true���Ȃ����false
	virtual bool HitToTarget(void);

	// �A�j���[�V�����̍X�V
	virtual void UpdateAnimation(float delta);
	// �A�j���[�V�����̕ύX
	// param@ animName: �ύX��A�j���[�V������
	virtual void ChangeAnimation(std::string animName);
private:

protected:
	float animationFrame_;
	bool isAnimEnd_;
	ActorType type_;
	std::string currentAnimation_;
	// ���E���]�׸�
	cocos2d::FlipX* flipFlag_;
	// �����ݑ_���Ă���Ώ�
	Actor& target_;
	// ���ޯĂƓ������Ă��邩���׸�
	bool isHitTarget_;
	
};

