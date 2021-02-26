#include "EnemyManager.h"
#include "Scene/GameScene.h"
#include "obj/Enemy/Enemy.h"
#include "obj/Enemy/Assassin.h"
#include "obj/Enemy/Cultist.h"
#include "obj/Enemy/TwistedCultist.h"
#include "obj/Enemy/Imp.h"
#include "obj/Enemy/BigCultist.h"
#include "anim/AnimMng.h"
#include "Effect/EffectManager.h"

// AI関係のinclude
#include "BehaviorBaseAI/AIActions/NormalAttack.h"
#include "BehaviorBaseAI/AIActions/SkillAttackAction.h"
#include "BehaviorBaseAI/AIActions/MoveAction.h"
#include "BehaviorBaseAI/JudgementTool/AttackJudgement.h"
#include "BehaviorBaseAI/JudgementTool/MoveJudgement.h"
#include "BehaviorBaseAI/JudgementTool/SkillAttackJudgement.h"

#include "HPGauge.h"

USING_NS_CC;

int EnemyManager::enemyNum_ = 0;

EnemyManager::EnemyManager(Layer& layer,Layer& hpLayer,Player& player):
	layer_(layer),hpLayer_(hpLayer),player_(&player)
{
	// srand関数で、乱数パターンを初期化する
	unsigned int now = (unsigned int)time(0);
	srand(now);
	spawnFlag_ = false;
}

EnemyManager::~EnemyManager()
{

}

void EnemyManager::Update(void)
{
	//for (auto e : enemies_)
	//{
	//	// 死んでいるか生きているかのﾌﾗｸﾞ格納
	//	//spawnFlag_ = (e->AliveCheck());
	//	/*if (!spawnFlag_)
	//	{
	//		auto randomType = static_cast<int>(ActorType::Assassin) + (rand() % static_cast<int>(ActorType::Assassin));
	//		CreateEnemy((ActorType)randomType);
	//	}*/
	//}
	std::remove_if(enemies_.begin(), enemies_.end(), [&](Enemy* enemy)
		{
			return !(enemy->AliveCheck());
		});
}

void EnemyManager::Initialize(void)
{
	// ﾀｲﾌﾟ毎のｱﾆﾒｰｼｮﾝ、behaviorの登録
	for (auto type : ActorType())
	{
		AddAnim(type);
		// ﾌﾟﾚｲﾔｰとFireballの場合はｽｷｯﾌﾟ
		if (type == ActorType::Player)
		{
			continue;
		}
		AddBehavior(type);
	}
}

void EnemyManager::CreateInitialEnemyOnFloor(int initNum)
{
	// ﾘｽﾄの中身を空にする
	enemies_.clear();
	CreateEnemy(ActorType::TwistedCultist);
	CreateEnemy(ActorType::Cultist);
	CreateEnemy(ActorType::Assassin);

	for (int init = 0; init < initNum; init++)
	{
		//auto randomType = static_cast<int>(ActorType::Assassin) + (rand() % static_cast<int>(ActorType::Assassin));
		//CreateEnemy((ActorType)randomType);
	}
}

void EnemyManager::AddBehavior(ActorType type)
{
	switch (type)
	{
	case ActorType::Player:
		break;
	case ActorType::Imp:
		break;
	case ActorType::Assassin:
		// root node
		behavior_[static_cast<int>(type)].AddNode("", "Root", 0,
			BehaviorTree::SelectRule::Priority, NULL, NULL);
		// attack node
		behavior_[static_cast<int>(type)].AddNode("Root", "Attack", 1,
			BehaviorTree::SelectRule::Sequence, AttackJudgement::Instance(), NULL);
		// move node
		behavior_[static_cast<int>(type)].AddNode("Root", "Move", 2,
			BehaviorTree::SelectRule::Sequence, MoveJudgement::Instance(), NULL);
		// attackの子にNormalAttackをぶら下げる
		behavior_[static_cast<int>(type)].AddNode("Attack", "NormalAttack", 1,
			BehaviorTree::SelectRule::Sequence, NULL, NormalAttack::Instance());
		// attackの子にskillAttackをぶら下げる
		behavior_[static_cast<int>(type)].AddNode("Attack", "SkillAttack", 2,
			BehaviorTree::SelectRule::Sequence,
			SkillAttackJudgement::Instance(), SkillAttackAction::Instance());
		behavior_[static_cast<int>(type)].AddNode("Move", "moveAction", 1,
			BehaviorTree::SelectRule::Non, NULL, MoveAction::Instance());
		break;
	case ActorType::TwistedCultist:
		// root node
		behavior_[static_cast<int>(type)].AddNode("", "Root", 0,
			BehaviorTree::SelectRule::Priority, NULL, NULL);
		// attack node
		behavior_[static_cast<int>(type)].AddNode("Root", "Attack", 1,
			BehaviorTree::SelectRule::Sequence, AttackJudgement::Instance(), NULL);
		// move node
		behavior_[static_cast<int>(type)].AddNode("Root", "Move", 2,
			BehaviorTree::SelectRule::Sequence, MoveJudgement::Instance(), NULL);
		// attackの子にNormalAttackをぶら下げる
		behavior_[static_cast<int>(type)].AddNode("Attack", "NormalAttack", 1,
			BehaviorTree::SelectRule::Sequence, NULL, NormalAttack::Instance());
		// attackの子にskillAttackをぶら下げる
		behavior_[static_cast<int>(type)].AddNode("Attack", "SkillAttack", 2,
			BehaviorTree::SelectRule::Sequence,
			SkillAttackJudgement::Instance(), SkillAttackAction::Instance());
		behavior_[static_cast<int>(type)].AddNode("Move", "moveAction", 1,
			BehaviorTree::SelectRule::Non, NULL, MoveAction::Instance());
		break;
	case ActorType::Cultist:
		// root node
		behavior_[static_cast<int>(type)].AddNode("", "Root", 0,
			BehaviorTree::SelectRule::Priority, NULL, NULL);
		// attack node
		behavior_[static_cast<int>(type)].AddNode("Root", "Attack", 1,
			BehaviorTree::SelectRule::Sequence, AttackJudgement::Instance(), NULL);
		// move node
		behavior_[static_cast<int>(type)].AddNode("Root", "Move", 2,
			BehaviorTree::SelectRule::Sequence, MoveJudgement::Instance(), NULL);
		// attackの子にNormalAttackをぶら下げる
		behavior_[static_cast<int>(type)].AddNode("Attack", "NormalAttack", 1,
			BehaviorTree::SelectRule::Sequence, NULL, NormalAttack::Instance());
		// attackの子にskillAttackをぶら下げる
		behavior_[static_cast<int>(type)].AddNode("Attack", "SkillAttack", 2,
			BehaviorTree::SelectRule::Sequence,
			SkillAttackJudgement::Instance(), SkillAttackAction::Instance());
		behavior_[static_cast<int>(type)].AddNode("Move", "moveAction", 1,
			BehaviorTree::SelectRule::Non, NULL, MoveAction::Instance());
		break;
	case ActorType::BigCultist:
		// root node
		behavior_[static_cast<int>(type)].AddNode("", "Root", 0,
			BehaviorTree::SelectRule::Priority, NULL, NULL);
		// attack node
		behavior_[static_cast<int>(type)].AddNode("Root", "Attack", 1,
			BehaviorTree::SelectRule::Sequence, AttackJudgement::Instance(), NULL);
		// move node
		behavior_[static_cast<int>(type)].AddNode("Root", "Move", 2,
			BehaviorTree::SelectRule::Sequence, MoveJudgement::Instance(), NULL);
		// attackの子にNormalAttackをぶら下げる
		behavior_[static_cast<int>(type)].AddNode("Attack", "NormalAttack", 1,
			BehaviorTree::SelectRule::Sequence, NULL, NormalAttack::Instance());
		// attackの子にskillAttackをぶら下げる
		behavior_[static_cast<int>(type)].AddNode("Attack", "SkillAttack", 2,
			BehaviorTree::SelectRule::Sequence,
			SkillAttackJudgement::Instance(), SkillAttackAction::Instance());
		behavior_[static_cast<int>(type)].AddNode("Move", "moveAction", 1,
			BehaviorTree::SelectRule::Non, NULL, MoveAction::Instance());
		break;
	case ActorType::Max:
		break;
	default:
		break;
	}
}

void EnemyManager::AddAnim(ActorType type)
{
	switch (type)
	{
	case ActorType::Player:
		break;
	case ActorType::Imp:
		break;
	case ActorType::Assassin:
		lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/assassin/assassin", "idle", 8, 0.08f, ActorType::Assassin, true);
		// run
		lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/assassin/assassin", "run", 8, 0.08f, ActorType::Assassin, true);

		// attack
		lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/assassin/assassin", "attack", 13, 0.08f, ActorType::Assassin, false);

		// death
		lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/assassin/assassin", "death", 16, 0.08f, ActorType::Assassin, false);
		// fall 
		lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/assassin/assassin", "fall", 3, 0.09f, ActorType::Assassin, false);
		// hit 
		lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/assassin/assassin", "hit", 3, 0.09f, ActorType::Assassin, false);
		break;
	case ActorType::TwistedCultist:
		lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/twistedCultist/twistedCultist", "idle", 6, 0.08f, ActorType::TwistedCultist, true);
		// run
		lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/twistedCultist/twistedCultist", "walk", 8, 0.08f, ActorType::TwistedCultist, true);
		// attack
		lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/twistedCultist/twistedCultist", "attack", 7, 0.08f, ActorType::TwistedCultist, false);

		// death
		lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/twistedCultist/twistedCultist", "death", 12, 0.08f, ActorType::TwistedCultist, false);
		// fall 
		lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/twistedCultist/twistedCultist", "fall", 3, 0.09f, ActorType::TwistedCultist, false);
		// hit 
		lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/twistedCultist/twistedCultist", "hit", 3, 0.09f, ActorType::TwistedCultist, false);
		break;
	case ActorType::Cultist:
		lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/cultist/cultist", "idle", 6, 0.03f, ActorType::Cultist, true);
		// run
		lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/cultist/cultist", "walk", 8, 0.08f, ActorType::Cultist, true);

		// attack
		lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/cultist/cultist", "attack", 10, 0.08f, ActorType::Cultist, true);

		// death
		lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/cultist/cultist", "death", 12, 0.08f, ActorType::Cultist, false);
		// fall 
		lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/cultist/cultist", "fall", 3, 0.09f, ActorType::Cultist, false);
		// hit 
		lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/cultist/cultist", "hit", 3, 0.09f, ActorType::Cultist, false);
		break;
	case ActorType::BigCultist:
		lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/bigCultist/bigCultist", "idle", 8, 0.03f, ActorType::BigCultist, true);
		// run
		lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/bigCultist/bigCultist", "run", 8, 0.08f, ActorType::BigCultist, true);

		// attack
		lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/bigCultist/bigCultist", "attack", 20, 0.08f, ActorType::BigCultist, true);

		// death
		lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/bigCultist/bigCultist", "death", 12, 0.08f, ActorType::BigCultist, false);
		// fall 
		lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/bigCultist/bigCultist", "fall", 3, 0.09f, ActorType::BigCultist, false);
		// hit 
		lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/bigCultist/bigCultist", "hit", 3, 0.09f, ActorType::BigCultist, false);
		break;

	case ActorType::Max:
		break;
	default:
		break;
	}
}

void EnemyManager::ResetEnemyNum(void)
{
	enemyNum_ = 0;
}

void EnemyManager::CreateEnemy(ActorType type)
{
	// TMXMapからスポーン位置を取得して保存する
	if (spawnPos_.size() <= 0)
	{
		auto map = Director::getInstance()->getRunningScene()->getChildByTag(static_cast<int>(zOlder::BG))->getChildByName("MapData");
		auto objGroups = ((TMXTiledMap*)map)->getObjectGroup("enemySpawn");
		// スポーン位置を保存する
		if (objGroups == nullptr)
		{
			return;
		}
		// TMXMapのオブジェクトレイヤーから情報取得
		auto& objs = objGroups->getObjects();
		for (auto& obj : objs)
		{
			ValueMap prop = obj.asValueMap();
			auto name = prop["name"].asString();
			auto x = prop["x"].asFloat();
			auto y = prop["y"].asFloat();
			// enemyだった時
			if (name == "enemy")
			{
				spawnPos_.emplace(spawnPos_.begin(), Vec2(x, y));
			}
		}
	}

	int randNum = rand() % 5;
	auto spawnPos = spawnPos_[randNum];
	//auto spawnPos = Vec2{ 150 + float(rand() % 300), 100 + float(rand() % 200) };

	Enemy* sprite = nullptr;
	HPGauge* hpSprite = nullptr;
	auto vRange = VisionRange(0.0f, 0.0f);
	switch (type)
	{
	case ActorType::Player:
		break;
	case ActorType::Imp:

		break;
	case ActorType::Assassin:
		vRange = VisionRange(100.0f, 50.0f);

		sprite = Assassin::CreateAssassin(Vec2(100.0f,100.0f), *player_, &behavior_[static_cast<int>(type)], vRange, 50, layer_);
		// 敵に名前を付ける
		// 死んだら"death"となる
		sprite->setName("assassin");
		break;
	case ActorType::TwistedCultist:
		vRange = VisionRange(50.0f, 200.0f);

		sprite = TwistedCultist::CreateTwistedCultist(Vec2(spawnPos.x, spawnPos.y), *player_, &behavior_[static_cast<int>(type)], vRange, 50, layer_);
		// 敵に名前を付ける
		// 死んだら"death"となる
		sprite->setName("twistedCultist");
		break;
	case ActorType::Cultist:
		vRange = VisionRange(200.0f, 100.0f);

		sprite = Cultist::CreateCultist(Vec2(spawnPos.x,spawnPos.y), *player_, &behavior_[static_cast<int>(type)], vRange, 50, layer_);
		// 敵に名前を付ける
		// 死んだら"death"となる
		sprite->setName("cultist");
		break;
	case ActorType::Max:
		break;
	default:
		break;
	}
	// 固有IDのｾｯﾄ
	sprite->SetID(enemyNum_);
	hpSprite = HPGauge::createHPGauge(dynamic_cast<Enemy&>(*sprite),1);
	hpSprite->setTag(101);
	hpSprite->setName(sprite->getName() + "_HPgauge" + "_" + std::to_string(sprite->GetID()));
	// 敵をActor用レイヤーの子供にする
	layer_.addChild(sprite);
	hpLayer_.addChild(hpSprite);
	// 敵のｱｯﾌﾟﾃﾞｰﾄ
	sprite->scheduleUpdate();
	hpSprite->scheduleUpdate();
	// ｽﾎﾟｰﾝﾌﾗｸﾞを折る
	spawnFlag_ = true;
	enemyNum_++;
	enemies_.emplace(enemies_.end(),sprite);
}

void EnemyManager::CreateBoss(const std::shared_ptr<EffectManager>& effectMng)
{
	Enemy* sprite = nullptr;
	Sprite* hpSprite = nullptr;
	auto spawnPos = Vec2{ 400, 50 };
	auto vRange = VisionRange(50.0f, 100.0f);

	sprite = BigCultist::CreateBigCultist(Vec2(spawnPos.x, spawnPos.y), *player_, &behavior_[static_cast<int>(ActorType::BigCultist)], vRange, 100, layer_);
	hpSprite = HPGauge::createHPGauge(dynamic_cast<Enemy&>(*sprite), 1);

	// 敵に名前を付ける
	// 死んだら"death"となる
	sprite->setName("enemy");
	// 敵をActor用レイヤーの子供にする
	layer_.addChild(sprite);
	hpLayer_.addChild(hpSprite);
	// 敵のｱｯﾌﾟﾃﾞｰﾄ
	sprite->scheduleUpdate();
	hpSprite->scheduleUpdate();
}
