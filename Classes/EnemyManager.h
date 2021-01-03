#pragma once
#include <cocos2d.h>
#include "obj/ActorType.h"
#include "BehaviorBaseAI/BehaviorTree.h"

class Player;
class Enemy;

class EnemyManager
{
public:
	EnemyManager(cocos2d::Layer& layer,cocos2d::Layer& hpLayer,Player& player);
	~EnemyManager();
	// XVŠÖ”
	void Update(void);
	// “GŠÖŒW‚Ì‰Šú‰»
	void Initialize(void);
	// ÌÛ±•ÏX‚µ‚½Û‚É‰‚ß‚É“G‚ğ½Îß°İ‚µ‚Ä‚¨‚­
	// param@ initNum: ‰‚ß‚Ì“G‚Ì”
	void CreateInitialEnemyOnFloor(int initNum);
	
	// À²Ìß•ÊbehaviorTree‚Ì“o˜^
	void AddBehavior(ActorType type);
	// À²Ìß•ÊspriteAnimation‚Ì“o˜^
	void AddAnim(ActorType type);
	// “G‚ÌÀ²Ìß–ˆ‚É¶¬
	// ¶¬À²Ğİ¸Ş‚Í“G‚ª€‚ñ‚¾‚ç
	void CreateEnemy(ActorType type);

private:
	cocos2d::Layer& layer_;
	cocos2d::Layer& hpLayer_;
	Player* player_;
	bool spawnFlag_;

	BehaviorTree assassinBehavior_;
	BehaviorTree twistedCultistBehavior_;
	BehaviorTree cultistBehavior_;

	std::array<BehaviorTree, 5> behavior_;

	std::list<Enemy*> enemies_;
};

