#include "TestSkill.h"

TestSkill::TestSkill()
{
	AddActiveSkill(std::make_shared<TestSkill>());
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
