#pragma once
#include <cocos2d.h>
#include <vector>
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
	// 敵生成数のﾘｾｯﾄ
	// ﾌﾛｱが変更したら敵の生成数は0に戻す
	// そしてそこからまた敵が生成される度に加算されていく
	void ResetEnemyNum(void);
	// 敵のﾀｲﾌﾟ毎に生成
	// 生成ﾀｲﾐﾝｸﾞは敵が死んだら
	void CreateEnemy(ActorType type);
	// ﾎﾞｽの出現関数
	void CreateBoss(const std::shared_ptr<EffectManager>& effectMng);
	// 敵ﾘｽﾄの取得
	std::list<Enemy*>& GetEnemies(void)
	{
		return enemies_;
	}
	
private:
	// 生成した敵の番号(生成した順に番号が増えていく)
	// Enemy側でSetID()で固有IDにｾｯﾄする
	static int enemyNum_;
	// GameSceneからの参照でもらう(CHAR_ENEMYのﾚｲﾔｰ)
	cocos2d::Layer& layer_;
	// GameSceneからの参照でもらう(FRONTのﾚｲﾔｰ(UI用))
	cocos2d::Layer& hpLayer_;
	// 敵に参照で渡す分のﾌﾟﾚｲﾔｰ情報
	Player* player_;
	// 敵をspawnさせるﾌﾗｸﾞ
	bool spawnFlag_;
	// 敵の行動を記述したbehaviorTree
	std::array<BehaviorTree, 6> behavior_;
	// 敵のﾘｽﾄ(Enemy class独自の関数を使用したいので)
	// 敵をgetChildrenで見てもdynamic_castでEnemy&にcast不可能だった
	std::list<Enemy*> enemies_;
	// スポーン位置をマップから取得
	std::vector<cocos2d::Vec2> spawnPos_;
};

