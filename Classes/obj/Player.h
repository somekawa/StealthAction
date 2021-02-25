#pragma once
#include <array>
#include <memory>
#include "cocos2d.h"
#include "Direction/Direction.h"
#include "anim/AnimMng.h"

#include "obj/Actor.h"

// �r�b�g�t�B�[���h
struct Bits
{
	bool FirstAttackFlg  : 1;		// 1���ڍU�����[�V�����t���O
	bool SecondAttackFlg : 1;		// 2���ڍU�����[�V�����t���O
	bool ThirdAttackFlg  : 1;		// 3���ڍU�����[�V�����t���O
	bool TransfromFlg    : 1;		// �ϐg�p�t���O
	bool DashFlg		 : 1;
};

#define AttackMove 30
#define AttackNumDef 10				// �ʏ��Ԃ̍U����

class SkillBase;
class ResidualShadow;
class Player : public Actor
{
public:
	Player(int hp,cocos2d::Layer& myLayer, cocos2d::Layer& enemyLayer, SkillBase* skillBasePtr);
	~Player();

	void Action(void) override;
	void ChangeDirection(float delta) override;
	void update(float delta) override;
	void AnimRegistrator(void) override;						// �A�j���[�V�����̓o�^
	const AttackRect& GetAttackRect(void);

	std::string GetAction(void);								// ���݂̃A�N�V���������擾����
	void SetAction(std::string action);							// ���݂̃A�N�V������Ԃ��Z�b�g����
	void SetDir(Direction dir);									// ���݂̕������Z�b�g����
	void KeyInputClear(void);									// �}�b�v�̐ؑ֎��ɃL�[���͂����Z�b�g������
	static Player* CreatePlayer(int hp,cocos2d::Layer& myLayer, cocos2d::Layer& enemyLayer, SkillBase* skillBasePtr);
	bool GetGameOverFlg(void);									// �Q�[���I�[�o�[�V�[���֑J�ڂ��Ă��悢�����f����
	int GetGiveDamageNum(void);									// �G�ɗ^����_���[�W��(light��dark�ł͒l���ς��)
	std::string GetPlayerColor(void);							// ���݂̃v���C���[�J���[
private:
	void attackMotion(float delta);								// �U�����[�V�����̐ݒ�
	void transformMotion(float delta);							// �g�����X�t�H�[���̐ݒ�
	void dashMotion(float delta);								// �_�b�V��(���)�̐ݒ�
	void colliderVisible(void);									// ��`�̕`��֘A
	void attackCollider(std::string str, cocos2d::Node* col, float& pos);	// �U����`�̃I�t�Z�b�g
	bool gameOverAction(void);									// �Q�[���I�[�o�[���̃A�j���[�V��������(true:�Đ��I��,false:�Đ���)
	void actModuleRegistration(void);
	OPRT_state* oprtState_;										// OPRT_state�̂��̂��Ăяo��

	std::string actionOld_;										// 1�t���[���O�̃A�N�V����

	static int no_;												// �v���C���[�l���̔c������ϐ�
	int myNo_;													// �����̔ԍ���ێ�����ϐ�

	//float oldPos_;											// �U�����[�V�������s�O�̍��W��ۊǂ���
	bool oldPosKeepFlg_;										// �U�����[�V�������s�O�̍��W��ۊǂ�1�x�����̎��s�ɂ��邽�߂̃t���O

	Bits bitFlg_;												// �r�b�g�t�B�[���h��p�����ϐ�
	float attackColOffset_;										// �U�����̃I�t�Z�b�g

	cocos2d::Action* gameOverAction_;							// �Q�[���I�[�o�[���ɍĐ�������̂̕ϐ�
	bool gameOverFlg_;											// gameOverAction��1�x�����Đ����邽�߂ɕK�v�ȕϐ�(true:�Đ�����or�Đ��I��,false:�Đ��O)
	bool deathFlg_;												// HP��0�ɂȂ����Ƃ��Ƀt���O�𗧂Ă�
	cocos2d::Vec2 deathPos_;									// ���S���̍��W��ێ����邽�߂̕ϐ�

	void skillAction(void);
	SkillBase* skillBase_;										// SkillBase�N���X�̃|�C���^���(�R���X�g���N�^�ōŏ��Ɏ擾)
	SkillBase* skillSprite = nullptr;							// skillSprite�̏��(Skill�������ɍX�V)
	std::vector<std::string> plfile_;							// Player�̃X�L�����(.txt)������vector
	cocos2d::Layer& enemyList_;
	std::string playerColor = "player_Light_";

	// ���g
	std::shared_ptr<ResidualShadow> resShadow_;
	// �_�b�V���e�X�g�Ŏg���Ă��ϐ�
	//float testnum = -4.0f;
	//float testnum = 0.00f;

	bool gameOverSceneFlg_ = false;
};