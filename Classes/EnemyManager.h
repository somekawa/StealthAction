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
	// “G¶¬”‚ÌØ¾¯Ä
	// ÌÛ±‚ª•ÏX‚µ‚½‚ç“G‚Ì¶¬”‚Í0‚É–ß‚·
	// ‚»‚µ‚Ä‚»‚±‚©‚ç‚Ü‚½“G‚ª¶¬‚³‚ê‚é“x‚É‰ÁZ‚³‚ê‚Ä‚¢‚­
	void ResetEnemyNum(void);
	// “G‚ÌÀ²Ìß–ˆ‚É¶¬
	// ¶¬À²Ğİ¸Ş‚Í“G‚ª€‚ñ‚¾‚ç
	void CreateEnemy(ActorType type,const std::shared_ptr<EffectManager>& effectMng);
	// ÎŞ½‚ÌoŒ»ŠÖ”
	void CreateBoss(const std::shared_ptr<EffectManager>& effectMng);
private:
	// ¶¬‚µ‚½“G‚Ì”Ô†(¶¬‚µ‚½‡‚É”Ô†‚ª‘‚¦‚Ä‚¢‚­)
	// Enemy‘¤‚ÅSetID()‚ÅŒÅ—LID‚É¾¯Ä‚·‚é
	static int enemyNum_;
	// GameScene‚©‚ç‚ÌQÆ‚Å‚à‚ç‚¤(CHAR_ENEMY‚ÌÚ²Ô°)
	cocos2d::Layer& layer_;
	// GameScene‚©‚ç‚ÌQÆ‚Å‚à‚ç‚¤(FRONT‚ÌÚ²Ô°(UI—p))
	cocos2d::Layer& hpLayer_;
	// “G‚ÉQÆ‚Å“n‚·•ª‚ÌÌßÚ²Ô°î•ñ
	Player* player_;
	// “G‚ğspawn‚³‚¹‚éÌ×¸Ş
	bool spawnFlag_;
	// “G‚Ìs“®‚ğ‹Lq‚µ‚½behaviorTree
	std::array<BehaviorTree, 6> behavior_;
	// “G‚ÌØ½Ä(Enemy class“Æ©‚ÌŠÖ”‚ğg—p‚µ‚½‚¢‚Ì‚Å)
	// “G‚ğgetChildren‚ÅŒ©‚Ä‚àdynamic_cast‚ÅEnemy&‚Écast•s‰Â”\‚¾‚Á‚½
	std::list<Enemy*> enemies_;
};

