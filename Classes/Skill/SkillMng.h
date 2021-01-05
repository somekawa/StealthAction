#pragma once
#include <map>
#include <string>
struct SkillParamMng
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
	SkillParamMng GetSkillData(std::string name);
	//全データの変更
	SkillParamMng ChangeAllData(std::string name, SkillParamMng param);
	//skillの開放
	bool SkillUnlock(std::string name);
	//skillの起動
	bool SkillActivate(std::string name);
	//skillの停止
	bool SkillInActivate(std::string name);

	void UpDate(void);
private:
	//スキルデータの保存
	std::map<std::string, SkillParamMng> skillData_;
};

