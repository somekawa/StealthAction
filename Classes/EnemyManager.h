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
	// 更新関数
	void Update(const std::shared_ptr<EffectManager>& effectMng);
	// 敵関係の初期化
	void Initialize(void);
	// ﾌﾛｱ変更した際に初めに敵をｽﾎﾟｰﾝしておく
	// param@ initNum: 初めの敵の数
	void CreateInitialEnemyOnFloor(int initNum, const std::shared_ptr<EffectManager>& effectMng);
	
	// ﾀｲﾌﾟ別behaviorTreeの登録
	void AddBehavior(ActorType type);
	// ﾀｲﾌﾟ別spriteAnimationの登録
	void AddAnim(ActorType type);
	// 敵のﾀｲﾌﾟ毎に生成
	// 生成ﾀｲﾐﾝｸﾞは敵が死んだら
	void CreateEnemy(ActorType type,const std::shared_ptr<EffectManager>& effectMng);
	// ﾎﾞｽの出現関数
	void CreateBoss(const std::shared_ptr<EffectManager>& effectMng);
private:
	cocos2d::Layer& layer_;
	cocos2d::Layer& hpLayer_;

	Player* player_;
	bool spawnFlag_;

	std::array<BehaviorTree, 6> behavior_;

	std::list<Enemy*> enemies_;
};

