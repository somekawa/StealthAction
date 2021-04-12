// 一部担当場所
#pragma once
#include <cocos2d.h>
#include <mutex>
#include <vector>
#include <string>
#include "Effect/EffectManager.h"
#include "Direction/Direction.h"

struct SkillParam
{
	std::string name;	// スキル名の保存
	bool lock;			// true: lock | false: unlock
	bool activation;	// 現在発動しているか true: 発動中 | false: 未発動
	bool removeFlg;		// 削除してもよいスキルか true: 削除OK | false: 削除NG
	float ct;			// クールタイム
};

class SkillBase :public cocos2d::Sprite
{
public:
	static cocos2d::Sprite* createSkillBase();
	~SkillBase();
	void AddActiveSkill(SkillBase* ptr);
	void RemoveActiveSkill(void);
	virtual void UpDate(float delta);
	virtual void Render(void);
	std::vector<SkillBase*> GetSkillList(void);
	virtual cocos2d::Vec2 GetPlayerPos(void);				// プレイヤー座標
	virtual cocos2d::Vec2 GetTargetPos(void);				// 敵座標(ホーミング系の時にプレイヤーとの距離が一番近い敵の座標を取得したい)
	virtual cocos2d::Rect GetEffectData(void) { return effectData_; };			// effectDataの取得
	virtual const Direction& GetPlayerDirection(void);		// プレイヤーの向き
	void SetPlayerPos(cocos2d::Vec2 playerPos);
	void SetTargetPos(cocos2d::Vec2 targetPos);
	void SetPlayerDirection(const Direction& direction);

	bool GetAllSkillActivate(void);				// スキルのうちどれかがactiveになっていたらtrueを返す
	bool GetSkillCT(std::string name);			// falseならCTが0じゃないため、まだそのスキルは使用不可
	int GetSkillCTTime(std::string name);		// スキルCT取得

	// スキルエフェクト情報取得
	FXStruct& GetFX(void)
	{
		return fx_;
	}
private:
	CREATE_FUNC(SkillBase);
	cocos2d::Vec2 playerPos_;
	cocos2d::Vec2 targetPos_;
	Direction playerDirection_;
protected:
	cocos2d::Vec2 pos_;
	cocos2d::Vec2 tpos_;
	static std::vector<SkillBase*> skillList_;
	SkillParam param;

	FXStruct fx_;
	cocos2d::Rect effectData_;
};