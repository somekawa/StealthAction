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
	/*この中にSkillの効果や動作を記述してください*/
}

void TestSkill::Init()
{
}

bool TestSkill::GetActive(void)
{
	return false;
}
