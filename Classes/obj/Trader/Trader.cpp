#include <cocos2d.h>
#include <random>
#include "../Player/Player.h"
#include "Trader.h"

Trader::Trader(const std::vector<std::string>& f_data,std::shared_ptr<Player> player)
{
	pos_ = cocos2d::Vec2(0,0 );
	saleList_ = Display(f_data);
	for (auto display : saleList_)
	{
		// Player‚ªæ“¾‚Å‚«‚éƒXƒLƒ‹‚Ì’Šo
		txtData_[display] = lpTextLoder.TextReader("../Classes/Skill/skill_data/Player/" + display + ".txt");
	}
}
Trader::~Trader()
{
}

std::array<std::string, 3> Trader::GetSaleList(void)
{
	return saleList_;
}

std::map<std::string, Param> Trader::GetTxtData_(void)
{
	return txtData_;
}

std::string Trader::GetSkillData(cocos2d::Vec2& pos)
{
	auto sale = GetSaleList();
	std::string name;
	for (int n = 0; n < GetSaleList().size(); n++)
	{
		if (pos.x + 20.0f >= pos_.x + 500 / 2 + 50.0f * n && pos.x + 20.0f <= pos_.x + 500 / 2 + 50.0f * (n + 1))
		{
			name = sale[n];
		}
	}
	return name;
}

cocos2d::Vec2 Trader::GetPos(void)
{
	return pos_;
}

std::array<std::string, 3> Trader::Display(const std::vector<std::string>& file)
{
	std::random_device seed;
	std::default_random_engine engine;
	std::array<std::string, 3> display = {file[0],file[4],file[5] };
	std::array<int, 3> num;
	std::uniform_int<> dist(0, (file.size() - 1));
	engine.seed(seed());
	/*for (int n = 0; n < display.size(); n++)
	{
		num[n] = dist(engine);
		display[n] = file[num[n]];
	}*/
	return display;
}
