#include "EnemyManager.h"
#include "obj/Enemy/Enemy.h"
#include "obj/Enemy/Assassin.h"
#include "obj/Enemy/Cultist.h"
#include "obj/Enemy/TwistedCultist.h"
#include "obj/Enemy/Imp.h"
#include "anim/AnimMng.h"

// AIä÷åWÇÃinclude
#include "BehaviorBaseAI/AIActions/NormalAttack.h"
#include "BehaviorBaseAI/AIActions/SkillAttackAction.h"
#include "BehaviorBaseAI/AIActions/MoveAction.h"
#include "BehaviorBaseAI/JudgementTool/AttackJudgement.h"
#include "BehaviorBaseAI/JudgementTool/MoveJudgement.h"
#include "BehaviorBaseAI/JudgementTool/SkillAttackJudgement.h"

USING_NS_CC;

EnemyManager::EnemyManager(cocos2d::Layer& layer,Player& player):
	layer_(layer),player_(&player)
{
	spawnFlag_ = false;
}

EnemyManager::~EnemyManager()
{

}

void EnemyManager::Update(void)
{
	for (auto e : enemies_)
	{
		// éÄÇÒÇ≈Ç¢ÇÈÇ©ê∂Ç´ÇƒÇ¢ÇÈÇ©ÇÃÃ◊∏ﬁäiî[
		spawnFlag_ = (e->AliveCheck());
		if (!spawnFlag_)
		{
			auto randomType = static_cast<int>(ActorType::Assassin) + (rand() % static_cast<int>(ActorType::Assassin));
			CreateEnemy((ActorType)randomType);
		}
	}
}

void EnemyManager::Initialize(void)
{
	// ¿≤ÃﬂñàÇÃ±∆“∞ºÆ›ÅAbehaviorÇÃìoò^
	for (auto type : ActorType())
	{
		// Ãﬂ⁄≤‘∞Ç∆FireballÇÃèÍçáÇÕΩ∑ØÃﬂ
		if (type == ActorType::Player ||
			type == ActorType::Fireball)
		{
			continue;
		}
		AddAnim(type);
		AddBehavior(type);
	}


}

void EnemyManager::CreateInitialEnemyOnFloor(int initNum)
{
	// ÿΩƒÇÃíÜêgÇãÛÇ…Ç∑ÇÈ
	enemies_.clear();

	for (int init = 0; init < initNum; init++)
	{
		auto randomType = static_cast<int>(ActorType::Assassin) + (rand() % static_cast<int>(ActorType::Assassin));
		CreateEnemy((ActorType)randomType);
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
		// attackÇÃéqÇ…NormalAttackÇÇ‘ÇÁâ∫Ç∞ÇÈ
		behavior_[static_cast<int>(type)].AddNode("Attack", "NormalAttack", 1,
			BehaviorTree::SelectRule::Sequence, NULL, NormalAttack::Instance());
		// attackÇÃéqÇ…skillAttackÇÇ‘ÇÁâ∫Ç∞ÇÈ
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
		// attackÇÃéqÇ…NormalAttackÇÇ‘ÇÁâ∫Ç∞ÇÈ
		behavior_[static_cast<int>(type)].AddNode("Attack", "NormalAttack", 1,
			BehaviorTree::SelectRule::Sequence, NULL, NormalAttack::Instance());
		// attackÇÃéqÇ…skillAttackÇÇ‘ÇÁâ∫Ç∞ÇÈ
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
		// attackÇÃéqÇ…NormalAttackÇÇ‘ÇÁâ∫Ç∞ÇÈ
		behavior_[static_cast<int>(type)].AddNode("Attack", "NormalAttack", 1,
			BehaviorTree::SelectRule::Sequence, NULL, NormalAttack::Instance());
		// attackÇÃéqÇ…skillAttackÇÇ‘ÇÁâ∫Ç∞ÇÈ
		behavior_[static_cast<int>(type)].AddNode("Attack", "SkillAttack", 2,
			BehaviorTree::SelectRule::Sequence,
			SkillAttackJudgement::Instance(), SkillAttackAction::Instance());
		behavior_[static_cast<int>(type)].AddNode("Move", "moveAction", 1,
			BehaviorTree::SelectRule::Non, NULL, MoveAction::Instance());
		break;
	case ActorType::Fireball:
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
		lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/assassin/assassin", "idle", 8, 0.03f, ActorType::Assassin, true);
		// run
		lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/assassin/assassin", "run", 8, 0.08f, ActorType::Assassin, true);

		// attack
		lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/assassin/assassin", "attack", 13, 0.08f, ActorType::Assassin, false);

		// death
		lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/assassin/assassin", "death", 16, 0.08f, ActorType::Assassin, false);

		// hit 
		lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/assassin/assassin", "hit", 3, 0.09f, ActorType::Assassin, false);
		break;
	case ActorType::TwistedCultist:
		lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/twistedCultist/twistedCultist", "idle", 6, 0.03f, ActorType::TwistedCultist, true);
		// run
		lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/twistedCultist/twistedCultist", "walk", 8, 0.08f, ActorType::TwistedCultist, true);
		// attack
		lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/twistedCultist/twistedCultist", "attack", 7, 0.08f, ActorType::TwistedCultist, false);

		// death
		lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/twistedCultist/twistedCultist", "death", 12, 0.08f, ActorType::TwistedCultist, false);

		// hit 
		lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/twistedCultist/twistedCultist", "hit", 3, 0.09f, ActorType::TwistedCultist, false);
		break;
	case ActorType::Cultist:
		lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/cultist/cultist", "idle", 6, 0.03f, ActorType::Cultist, true);
		// run
		lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/cultist/cultist", "walk", 8, 0.08f, ActorType::Cultist, true);

		// attack
		lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/cultist/cultist", "attack", 10, 0.08f, ActorType::Cultist, false);

		// death
		lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/cultist/cultist", "death", 12, 0.08f, ActorType::Cultist, false);

		// hit 
		lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/cultist/cultist", "hit", 3, 0.09f, ActorType::Cultist, false);
		break;
		// ç°ÇÃÇ∆Ç±ÇÎÇ±Ç±Ç…fireballÇÇ®Ç¢ÇƒÇ¢ÇÈ
	case ActorType::Fireball:
		// fireball 
		lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/Fireball/fireball", "normal", 4, 0.08f, ActorType::Fireball, true);
		// fireball_impact
		lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/Fireball/fireball", "impact", 5, 0.08f, ActorType::Fireball, false);
		break;
	case ActorType::Max:
		break;
	default:
		break;
	}
}

void EnemyManager::CreateEnemy(ActorType type)
{
	Enemy* sprite = nullptr;
	auto spawnPos = Vec2{ 300.0f,200.0f };
	auto vRange = VisionRange(0.0f, 0.0f);

	switch (type)
	{
	case ActorType::Player:
		break;
	case ActorType::Imp:

		break;
	case ActorType::Assassin:
		vRange = VisionRange(20.0f, 50.0f);

		sprite = Assassin::CreateAssassin(spawnPos, *player_, &behavior_[static_cast<int>(type)], vRange, 50, layer_);

		break;
	case ActorType::TwistedCultist:
		vRange = VisionRange(50.0f, 200.0f);

		sprite = TwistedCultist::CreateTwistedCultist(spawnPos, *player_, &behavior_[static_cast<int>(type)], vRange, 50, layer_);

		break;
	case ActorType::Cultist:
		vRange = VisionRange(200.0f, 100.0f);

		sprite = Cultist::CreateCultist(spawnPos, *player_, &behavior_[static_cast<int>(type)], vRange, 50, layer_);

		break;
	case ActorType::Fireball:
		break;
	case ActorType::Max:
		break;
	default:
		break;
	}
	// ìGÇ…ñºëOÇïtÇØÇÈ
// éÄÇÒÇæÇÁ"death"Ç∆Ç»ÇÈ
	sprite->setName("enemy");

	// ìGÇActorópÉåÉCÉÑÅ[ÇÃéqãüÇ…Ç∑ÇÈ
	layer_.addChild(sprite);
	// ìGÇÃ±ØÃﬂ√ﬁ∞ƒ
	sprite->scheduleUpdate();
	// ΩŒﬂ∞›Ã◊∏ﬁÇê‹ÇÈ
	spawnFlag_ = true;

	enemies_.emplace(enemies_.end(),sprite);
}
