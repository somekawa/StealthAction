#pragma once
#include <cocos2d.h>
#include <mutex>
#include <vector>
#include <string>

#include "Effect/EffectManager.h"
#include "Direction/Direction.h"

struct SkillParam
{
	std::string name;	//スキル名の保存
	bool lock;	//true: lock | false: unlock
	bool activation;	//現在発動しているか true: 発動中 | false: 未発動
	float ct;	//クールタイム
};

class TestSkill;
// 結局testSkillが呼ばれる際にSkillBaseが呼ばれるので、ここのｺﾝｽﾄﾗｸﾀで名前ﾃﾞｰﾀを格納しそれをskillDataとして使用する。

class SkillBase :public cocos2d::Sprite
{
public:
	static cocos2d::Sprite* createSkillBase();
	//SkillBase();
	~SkillBase();
	void AddActiveSkill(SkillBase* ptr);
	void RemoveActiveSkill(void);
	virtual void UpDate(float delta);
	virtual void Render(void);
	virtual cocos2d::Vec2 GetPlayerPos(void);	// プレイヤー座標
	virtual const Direction& GetPlayerDirection(void); // ﾌﾟﾚｲﾔｰの向き
	virtual cocos2d::Vec2 GetTargetPos(void);	// 敵座標(ホーミング系の時にプレイヤーとの距離が一番近い敵の座標を取得したい)
	void SetPlayerPos(cocos2d::Vec2 playerPos);
	void SetTargetPos(cocos2d::Vec2 targetPos);
	void SetPlayerDirection(const Direction& direction);
	// ｽｷﾙｴﾌｪｸﾄ情報取得
	FXStruct& GetFX(void)
	{
		return fx_;
	}
private:
	CREATE_FUNC(SkillBase);
	cocos2d::Vec2 playerPos_;
	cocos2d::Vec2 targetPos_;
	Direction playerDirection_;
	//std::shared_ptr<SkillMng> skillMng_;
protected:
	//virtual bool GetActive(void) = 0;
	cocos2d::Vec2 pos_;
	cocos2d::Vec2 tpos_;
	std::vector<SkillBase*> skillList_;
	SkillParam param;
	// 自身のｴﾌｪｸﾄのｽﾌﾟﾗｲﾄ
	cocos2d::Sprite* effectSprite_;

	FXStruct fx_;
};

