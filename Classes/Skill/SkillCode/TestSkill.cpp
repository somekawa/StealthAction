#include "TestSkill.h"

TestSkill::TestSkill()
{
	// add‚Å‚«‚È‚¢
	AddActiveSkill(this);
	// lpEffect.add‚İ‚½‚¢‚È‚±‚Æ‚ğ‚â‚é
}

TestSkill::~TestSkill()
{
}

void TestSkill::UpDate(void)
{
	/*‚±‚Ì’†‚ÉSkill‚ÌŒø‰Ê‚â“®ì‚ğ‹Lq‚µ‚Ä‚­‚¾‚³‚¢*/
	// ‚±‚±‚Å´Ìª¸Ä‚ÌÎß¼Ş¼®İ‚ÌˆÚ“®‚ğ‚â‚é
}

void TestSkill::Init()
{
}

bool TestSkill::GetActive(void)
{
	return false;
}
