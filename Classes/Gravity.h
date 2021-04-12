#pragma once
#include "Geometory.h"

class Actor;
// ���t���[���Ώ�actor�ɂ�����d��
#define Gravitation 0.3f

// �d�̓N���X
class Gravity
{
public:
	Gravity(Actor& target);
	~Gravity();
	// �d�͂�������
	void ApplyGravityToTarget(float delta);
private:
	// �d�͂�������Ώۂ�actor
	Actor& target_;
	// �����Ă����d�̗͂�
	float g_;
};