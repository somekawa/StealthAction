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
	// �X�V�֐�
	void Update(const std::shared_ptr<EffectManager>& effectMng);
	// �G�֌W�̏�����
	void Initialize(void);
	// �۱�ύX�����ۂɏ��߂ɓG���݂߰��Ă���
	// param@ initNum: ���߂̓G�̐�
	void CreateInitialEnemyOnFloor(int initNum, const std::shared_ptr<EffectManager>& effectMng);
	
	// ���ߕ�behaviorTree�̓o�^
	void AddBehavior(ActorType type);
	// ���ߕ�spriteAnimation�̓o�^
	void AddAnim(ActorType type);
	// �G��������ؾ��
	// �۱���ύX������G�̐�������0�ɖ߂�
	// �����Ă�������܂��G�����������x�ɉ��Z����Ă���
	void ResetEnemyNum(void);
	// �G�����ߖ��ɐ���
	// �������ݸނ͓G�����񂾂�
	void CreateEnemy(ActorType type,const std::shared_ptr<EffectManager>& effectMng);
	// �޽�̏o���֐�
	void CreateBoss(const std::shared_ptr<EffectManager>& effectMng);
private:
	// ���������G�̔ԍ�(�����������ɔԍ��������Ă���)
	// Enemy����SetID()�ŌŗLID�ɾ�Ă���
	static int enemyNum_;
	// GameScene����̎Q�Ƃł��炤(CHAR_ENEMY��ڲ԰)
	cocos2d::Layer& layer_;
	// GameScene����̎Q�Ƃł��炤(FRONT��ڲ԰(UI�p))
	cocos2d::Layer& hpLayer_;
	// �G�ɎQ�Ƃœn��������ڲ԰���
	Player* player_;
	// �G��spawn�������׸�
	bool spawnFlag_;
	// �G�̍s�����L�q����behaviorTree
	std::array<BehaviorTree, 6> behavior_;
	// �G��ؽ�(Enemy class�Ǝ��̊֐����g�p�������̂�)
	// �G��getChildren�Ō��Ă�dynamic_cast��Enemy&��cast�s�\������
	std::list<Enemy*> enemies_;
};

