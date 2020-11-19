#include "Gravity.h"
#include "obj/Actor.h"

USING_NS_CC;

Gravity::Gravity(Actor& target):
	target_(target)
{
	g_ = 0.0f;
}

Gravity::~Gravity()
{
}

void Gravity::ApplyGravityToTarget(float delta)
{
	// �܂��^�[�Q�b�g�̃|�W�V�����擾
	auto pos = target_.GetPos();
	// ����d�͂ɗ͂������Ă���(���̂Ƃ���1200�Ńe�X�g)
	g_ += (Gravitation+(delta * 1200));
	// �^�[�Q�b�g�ɏd�͂�������
	pos.y -= g_;
	// �d�͂���������̕ύX��̃|�W�V�����Ń^�[�Q�b�g�̃|�W�V�����Z�b�g
	//target_.SetPos(pos);
	target_.setPosition(Vec2(pos.x, pos.y));
}
