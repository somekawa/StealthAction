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
	/*‚±‚Ì’†‚ÉSkill‚ÌŒø‰Ê‚â“®ì‚ğ‹Lq‚µ‚Ä‚­‚¾‚³‚¢*/
}

void TestSkill::Init()
{
}

bool TestSkill::GetActive(void)
{
	return false;
}
