#include "TestSkill.h"

TestSkill::TestSkill()
{
	// addできない
	AddActiveSkill(this);
	// lpEffect.addみたいなことをやる
}

TestSkill::~TestSkill()
{
}

void TestSkill::UpDate(void)
{
	/*この中にSkillの効果や動作を記述してください*/
	// ここでｴﾌｪｸﾄのﾎﾟｼﾞｼｮﾝの移動をやる
}

void TestSkill::Init()
{
}

bool TestSkill::GetActive(void)
{
	return false;
}
