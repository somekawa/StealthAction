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
	auto pos = target_.getPosition();
	if (!target_.OnFloor())
	{
		// ����d�͂ɗ͂������Ă���(���̂Ƃ���1200�Ńe�X�g)
		g_ += (Gravitation + (delta * 50));
	}
	else
	{
		g_ = 0.0f;
	}

	// �^�[�Q�b�g�ɏd�͂�������
	pos.y -= g_;
	// �d�͂���������̕ύX��̃|�W�V�����Ń^�[�Q�b�g�̃|�W�V�����Z�b�g
	//target_.SetPos(pos);
	target_.setPosition(Vec2(pos.x, pos.y));
	//target_.runAction(MoveTo::create(0.0f, Vec2(pos.x, pos.y - g_)));
}
