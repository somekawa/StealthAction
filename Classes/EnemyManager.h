#pragma once
#include <cocos2d.h>
#include "obj/ActorType.h"
#include "BehaviorBaseAI/BehaviorTree.h"

class Player;
class Enemy;
class EffectManager;

class EnemyManager
{
public:
	EnemyManager(cocos2d::Layer& layer,cocos2d::Layer& hpLayer,Player& player);
	~EnemyManager();
	// XVŠÖ”
	void Update(const std::shared_ptr<EffectManager>& effectMng);
	// “GŠÖŒW‚Ì‰Šú‰»
	void Initialize(void);
	// ÌÛ±•ÏX‚µ‚½Û‚É‰‚ß‚É“G‚ğ½Îß°İ‚µ‚Ä‚¨‚­
	// param@ initNum: ‰‚ß‚Ì“G‚Ì”
	void CreateInitialEnemyOnFloor(int initNum, const std::shared_ptr<EffectManager>& effectMng);
	
	// À²Ìß•ÊbehaviorTree‚Ì“o˜^
	void AddBehavior(ActorType type);
	// À²Ìß•ÊspriteAnimation‚Ì“o˜^
	void AddAnim(ActorType type);
	// “G‚ÌÀ²Ìß–ˆ‚É¶¬
	// ¶¬À²Ğİ¸Ş‚Í“G‚ª€‚ñ‚¾‚ç
	void CreateEnemy(ActorType type,const std::shared_ptr<EffectManager>& effectMng);
	// ÎŞ½‚ÌoŒ»ŠÖ”
	void CreateBoss(const std::shared_ptr<EffectManager>& effectMng);
private:
	cocos2d::Layer& layer_;
	cocos2d::Layer& hpLayer_;

	Player* player_;
	bool spawnFlag_;

	std::array<BehaviorTree, 6> behavior_;

	std::list<Enemy*> enemies_;
};

