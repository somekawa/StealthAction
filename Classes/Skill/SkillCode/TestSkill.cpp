#include "TestSkill.h"

TestSkill::TestSkill()
{
	// add�ł��Ȃ�
	AddActiveSkill(this);
}

TestSkill::~TestSkill()
{
}

void TestSkill::UpDate(void)
{
	/*���̒���Skill�̌��ʂ⓮����L�q���Ă�������*/
}

void TestSkill::Init()
{
}

bool TestSkill::GetActive(void)
{
	return false;
}
