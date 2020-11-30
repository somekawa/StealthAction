#pragma once
#include <map>
#include <list>
#include <vector>
#include <array>
#include <string>
#include "../../Loader/TextLoder.h"

class Player;
using DataTable = std::map<std::string, std::vector<std::string>>;
class Trader
{
public:
	Trader(const std::vector<std::string>& f_data,std::shared_ptr<Player>player);
	~Trader();
	//抽選された商品リストを取得
	std::array<std::string, 3> GetSaleList(void);
	//textデータの取得
	std::map<std::string, Param> GetTxtData_(void);
	//対象が購入できる商品を取得
	std::string GetSkillData(cocos2d::Vec2& pos);
	cocos2d::Vec2 GetPos(void);
private:
	std::map<std::string, Param> txtData_;
	std::array<std::string, 3> saleList_;
	//商品リストの抽選
	std::array<std::string, 3> Display(const std::vector<std::string>& file);
	cocos2d::Vec2 pos_;
};

