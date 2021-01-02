#pragma once
#include <cocos2d.h>
#include "obj/ActorType.h"
#include "BehaviorBaseAI/BehaviorTree.h"

class Player;
class Enemy;

class EnemyManager
{
public:
	EnemyManager(cocos2d::Layer& layer,Player& player);
	~EnemyManager();
	// 更新関数
	void Update(void);
	// 敵関係の初期化
	void Initialize(void);
	// ﾌﾛｱ変更した際に初めに敵をｽﾎﾟｰﾝしておく
	// param@ initNum: 初めの敵の数
	void CreateInitialEnemyOnFloor(int initNum);
	
	// ﾀｲﾌﾟ別behaviorTreeの登録
	void AddBehavior(ActorType type);
	// ﾀｲﾌﾟ別spriteAnimationの登録
	void AddAnim(ActorType type);
	// 敵のﾀｲﾌﾟ毎に生成
	// 生成ﾀｲﾐﾝｸﾞは敵が死んだら
	void CreateEnemy(ActorType type);

private:
	cocos2d::Layer& layer_;
	Player* player_;
	bool spawnFlag_;

	BehaviorTree assassinBehavior_;
	BehaviorTree twistedCultistBehavior_;
	BehaviorTree cultistBehavior_;

	std::array<BehaviorTree, 5> behavior_;

	std::list<Enemy*> enemies_;
};

