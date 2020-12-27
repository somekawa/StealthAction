#include "Skill/SkillMng.h"
#include "TestSkill.h"

TestSkill::TestSkill(std::shared_ptr<SkillMng> skillMng)
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
