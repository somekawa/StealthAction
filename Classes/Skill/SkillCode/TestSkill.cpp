#include "TestSkill.h"

TestSkill::TestSkill()
{
	// add�ł��Ȃ�
	AddActiveSkill(this);
	// lpEffect.add�݂����Ȃ��Ƃ����
}

TestSkill::~TestSkill()
{
}

void TestSkill::UpDate(void)
{
	/*���̒���Skill�̌��ʂ⓮����L�q���Ă�������*/
	// �����Ŵ̪�Ă��߼޼�݂̈ړ������
}

void TestSkill::Init()
{
}

bool TestSkill::GetActive(void)
{
	return false;
}
