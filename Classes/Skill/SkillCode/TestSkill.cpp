#include "TestSkill.h"

TestSkill::TestSkill()
{
	// addできない
	AddActiveSkill(this);
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
