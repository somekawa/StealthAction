// Enemy�̐��������S��(CreateEnemy�֐�189~253)
#include "EnemyManager.h"
#include "Scene/GameScene.h"
#include "obj/Enemy/Enemy.h"
#include "obj/Enemy/Assassin.h"
#include "obj/Enemy/Cultist.h"
#include "obj/Enemy/TwistedCultist.h"
#include "anim/AnimMng.h"
#include "Effect/EffectManager.h"

// AI�֌W��include
#include "BehaviorBaseAI/AIActions/NormalAttack.h"
#include "BehaviorBaseAI/JudgementTool/AttackJudgement.h"
#include "BehaviorBaseAI/JudgementTool/MoveJudgement.h"
#include "BehaviorBaseAI/JudgementTool/SkillAttackJudgement.h"

#include "HPGauge.h"

USING_NS_CC;

int EnemyManager::enemyNum_ = 0;

EnemyManager::EnemyManager(Layer& layer,Layer& hpLayer,Player& player):
	layer_(layer),hpLayer_(hpLayer),player_(&player)
{
	// srand�֐��ŁA�����p�^�[��������������
	const unsigned int now = static_cast<unsigned int>(time(0));
	srand(now);
	spawnFlag_ = false;
}

EnemyManager::~EnemyManager()
{

}

void EnemyManager::Update(void)
{
	std::remove_if(enemies_.begin(), enemies_.end(), [&](Enemy* enemy)
		{
			return !(enemy->AliveCheck());
		});
}

void EnemyManager::Initialize(void)
{
	// ���ߖ��̱�Ұ��݁Abehavior�̓o�^
	for (auto type : ActorType())
	{
		AddAnim(type);
		// ��ڲ԰��Fireball�̏ꍇ�ͽ����
		if (type == ActorType::Player)
		{
			continue;
		}
		AddBehavior(type);
	}
}

void EnemyManager::CreateInitialEnemyOnFloor(int initNum)
{
	// ؽĂ̒��g����ɂ���
	enemies_.clear();
	CreateEnemy(ActorType::TwistedCultist);
	CreateEnemy(ActorType::Cultist);
	CreateEnemy(ActorType::Assassin);
}

void EnemyManager::AddBehavior(ActorType type)
{
	switch (type)
	{
	case ActorType::Player:
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
		// attack�̎q��NormalAttack���Ԃ牺����
		behavior_[static_cast<int>(type)].AddNode("Attack", "NormalAttack", 1,
			BehaviorTree::SelectRule::Sequence, NULL, NormalAttack::Instance());
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
		// attack�̎q��NormalAttack���Ԃ牺����
		behavior_[static_cast<int>(type)].AddNode("Attack", "NormalAttack", 1,
			BehaviorTree::SelectRule::Sequence, NULL, NormalAttack::Instance());
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
		// attack�̎q��NormalAttack���Ԃ牺����
		behavior_[static_cast<int>(type)].AddNode("Attack", "NormalAttack", 1,
			BehaviorTree::SelectRule::Sequence, NULL, NormalAttack::Instance());
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
	case ActorType::Assassin:
		lpAnimMng.XmlAnimDataRead("assassinAnim", type);
		break;
	case ActorType::TwistedCultist:
		lpAnimMng.XmlAnimDataRead("twistedCultistAnim", type);
		break;
	case ActorType::Cultist:
		lpAnimMng.XmlAnimDataRead("cultistAnim", type);
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
	// TMXMap����X�|�[���ʒu���擾���ĕۑ�����
	if (spawnPos_.size() <= 0)
	{
		const auto map = Director::getInstance()->getRunningScene()->getChildByTag(static_cast<int>(zOrder::BG))->getChildByName("MapData");
		const auto objGroups = ((TMXTiledMap*)map)->getObjectGroup("enemySpawn");
		// �X�|�[���ʒu��ۑ�����
		if (objGroups == nullptr)
		{
			return;
		}
		// TMXMap�̃I�u�W�F�N�g���C���[������擾
		const auto& objs = objGroups->getObjects();
		for (auto& obj : objs)
		{
			ValueMap prop = obj.asValueMap();
			const auto name = prop["name"].asString();
			const auto x = prop["x"].asFloat();
			const auto y = prop["y"].asFloat();
			// enemy��������
			if (name == "enemy")
			{
				spawnPos_.emplace(spawnPos_.begin(), Vec2(x, y));
			}
		}
	}

	const int randNum = rand() % 5;
	const auto spawnPos = spawnPos_[randNum];

	Enemy* sprite = nullptr;
	auto vRange = VisionRange(0.0f, 0.0f);
	switch (type)
	{
	case ActorType::Player:
		break;
	case ActorType::Assassin:
		vRange = VisionRange(100.0f, 50.0f);

		sprite = Assassin::CreateAssassin(Vec2(spawnPos_[3].x, spawnPos_[3].y), *player_, &behavior_[static_cast<int>(type)], vRange, 50, layer_);
		// �G�ɖ��O��t����
		// ���񂾂�"death"�ƂȂ�
		sprite->setName("assassin");
		break;
	case ActorType::TwistedCultist:
		vRange = VisionRange(50.0f, 200.0f);

		sprite = TwistedCultist::CreateTwistedCultist(Vec2(spawnPos.x, spawnPos.y), *player_, &behavior_[static_cast<int>(type)], vRange, 50, layer_);
		// �G�ɖ��O��t����
		// ���񂾂�"death"�ƂȂ�
		sprite->setName("twistedCultist");
		break;
	case ActorType::Cultist:
		vRange = VisionRange(200.0f, 100.0f);

		sprite = Cultist::CreateCultist(Vec2(spawnPos.x,spawnPos.y), *player_, &behavior_[static_cast<int>(type)], vRange, 50, layer_);
		// �G�ɖ��O��t����
		// ���񂾂�"death"�ƂȂ�
		sprite->setName("cultist");
		break;
	case ActorType::Max:
		break;
	default:
		break;
	}
	
	if (sprite != nullptr)
	{
		// �ŗLID�̾��
		sprite->SetID(enemyNum_);
		HPGauge* hpSprite = HPGauge::createHPGauge(dynamic_cast<Enemy&>(*sprite), 1);
		hpSprite->setTag(101);
		hpSprite->setName(sprite->getName() + "_HPgauge" + "_" + std::to_string(sprite->GetID()));
		// �G��Actor�p���C���[�̎q���ɂ���
		layer_.addChild(sprite);
		hpLayer_.addChild(hpSprite);
		// �G�̱����ް�
		sprite->scheduleUpdate();
		hpSprite->scheduleUpdate();
		// ��߰��׸ނ�܂�
		spawnFlag_ = true;
		enemyNum_++;
		enemies_.emplace(enemies_.end(), sprite);
	}
}