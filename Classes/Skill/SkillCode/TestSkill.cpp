#include "Skill/SkillMng.h"
#include "TestSkill.h"

TestSkill::TestSkill(std::shared_ptr<SkillMng> skillMng)
{
	AddActiveSkill(std::make_shared<TestSkill>(skillMng));
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
