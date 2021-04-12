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
	// �X�V�֐�
	void Update(void);
	// �G�֌W�̏�����
	void Initialize(void);
	// �t���A�ύX�����ۂɏ��߂ɓG���X�|�[�����Ă���
	// param@ initNum: ���߂̓G�̐�
	void CreateInitialEnemyOnFloor(int initNum);
	
	// �^�C�v��behaviorTree�̓o�^
	void AddBehavior(ActorType type);
	// �^�C�v��spriteAnimation�̓o�^
	void AddAnim(ActorType type);
	// �G�������̃��Z�b�g
	// �t���A���ύX������G�̐�������0�ɖ߂�
	// �����Ă�������܂��G�����������x�ɉ��Z����Ă���
	void ResetEnemyNum(void);
	// �G�̃^�C�v���ɐ���
	// �����^�C�~���O�͓G�����񂾂�
	void CreateEnemy(ActorType type);
	// �G���X�g�̎擾
	std::list<Enemy*>& GetEnemies(void)
	{
		return enemies_;
	}
	
private:
	// ���������G�̔ԍ�(�����������ɔԍ��������Ă���)
	// Enemy����SetID()�ŌŗLID�ɃZ�b�g����
	static int enemyNum_;
	// GameScene����̎Q�Ƃł��炤(CHAR_ENEMY�̃��C���[)
	cocos2d::Layer& layer_;
	// GameScene����̎Q�Ƃł��炤(FRONT�̃��C���[(UI�p))
	cocos2d::Layer& hpLayer_;
	// �G�ɎQ�Ƃœn�����̃v���C���[���
	Player* player_;
	// �G��spawn������t���O
	bool spawnFlag_;
	// �G�̍s�����L�q����behaviorTree
	std::array<BehaviorTree, 6> behavior_;
	// �G�̃��X�g(Enemy class�Ǝ��̊֐����g�p�������̂�)
	// �G��getChildren�Ō��Ă�dynamic_cast��Enemy&��cast�s�\������
	std::list<Enemy*> enemies_;
	// �X�|�[���ʒu���}�b�v����擾
	std::vector<cocos2d::Vec2> spawnPos_;
};