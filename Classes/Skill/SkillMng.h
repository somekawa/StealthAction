#pragma once
#include <map>
struct SkillParam
{
	std::string name;	//スキル名の保存
	bool lock;	//true: lock | false: unlock
	bool activation;	//現在発動しているか true: 発動中 | false: 未発動
	float ct;	//クールタイム
};

class SkillMng
{
public:
	SkillMng();
	~SkillMng();

	//skillデータの取得
	SkillParam GetSkillData(std::string name);
	//全データの変更
	SkillParam ChangeAllData(std::string name, SkillParam param);
	//skillの開放
	bool SkillUnlock(std::string name);
	//skillの起動
	bool SkillActivate(std::string name);
	//skillの停止
	bool SkillInActivate(std::string name);

	void UpDate(std::string name);
private:
	//スキルデータの保存
	std::map<std::string, SkillParam> skillData_;
};

