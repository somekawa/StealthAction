// �S���ꏊ
#pragma once
#include <cocos2d.h>
#include <vector>
#include <array>
#include <memory>
#include <utility>
#include "Direction/Direction.h"
#include "obj/Actor.h"

#define ATTAKFLAG 1    //00000001

// �p�����[�^
struct PlParam
{
	std::string modeName;		// ���[�h�̖��O
	float runSpeedUp;			// �ړ����x�ւ̉��Z�l
	float attackDef;			// �ʏ�̍U����
	int attackChain;			// �ő�A����
	std::vector<std::pair<std::string,float>> chainAnimName;	// first�F�U���A�j���[�V������,second�F�_���[�W�ǉ��{��
};

// �r�b�g�t�B�[���h
struct Bits
{
	bool transformFlg    : 1;		// �ϐg�p�t���O
	bool dashFlg		 : 1;		// �_�b�V���A�N�V�����t���O
	bool gameOverFlg     : 1;		// GameOverAction��1�x�����Đ����邽�߂ɕK�v�ȕϐ�(true:�Đ�����or�Đ��I��,false:�Đ��O)
	bool gameOverSceneFlg: 1;		// GameScene�ɃQ�[���I�[�o�[�ւ̑J�ڂ�m�点��ׂ̃t���O
	bool deathFlg		 : 1;		// HP��0�ɂȂ����Ƃ��Ƀt���O�𗧂Ă�
	bool oldPosKeepFlg_  : 1;		// �U�����[�V�������s�O�̍��W��ۑ�����t���O
	bool reserve		 : 2;		// ���U�[�u�̈�
};

#define DashMove 30.0f										// �_�b�V���A�N�V�����̈ړ���
#define Calculation(X) ((X * 100.0f) - 16.0f) / 4.0f		// �_�b�V���A�N�V�����̎w���֐��v�Z���ɗ��p
#define AttackSizeCorrection	15.0f						// �U���摜�̃T�C�Y���ɂ��␳
#define TransformSizeCorrection 10.0f						// ���[�h�ύX�摜�̃T�C�Y���ɂ��␳
#define ReceiveDamage			10.0f						// �G����󂯂�_���[�W��
#define SlipDamage				0.05f						// �X���b�v�_���[�W��

class SkillBase;
class ResidualShadow;

class Player : public Actor
{
public:
	Player(int hp,cocos2d::Layer& myLayer, cocos2d::Layer& enemyLayer, SkillBase* skillBasePtr);
	~Player();

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
	float GetRunSpeedUp(void);									// ���݂̃��[�h����ړ����Z�l���擾����

private:
	void AttackMotion(float delta);								// �U�����[�V�����̐ݒ�
	void TransformMotion(float delta);							// �g�����X�t�H�[���̐ݒ�
	void DashMotion(float delta);								// �_�b�V��(���)�̐ݒ�
	bool GameOverAction(void);									// �Q�[���I�[�o�[���̃A�j���[�V��������(true:�Đ��I��,false:�Đ���)
	void ActModuleRegistration(void);							// �A�N�V�������W���[���̓o�^
	void SkillAction(void);										// �X�L������

	OPRT_state* oprtState_;										// OPRT_state�̂��̂��Ăяo��
	std::string actionOld_;										// 1�t���[���O�̃A�N�V����
	std::string playerMode_;									// Light/Dark���[�h�̐ؑ֗p�ϐ�
	static int no_;												// �v���C���[�l���̔c������ϐ�
	const int myNo_;											// �����̔ԍ���ێ�����ϐ�
	Bits bitFlg_;												// �r�b�g�t�B�[���h��p�����ϐ�

	cocos2d::Action* gameOverAction_;							// �Q�[���I�[�o�[���ɍĐ�������̂̕ϐ�
	cocos2d::Vec2 deathPos_;									// ���S���̍��W��ێ����邽�߂̕ϐ�

	SkillBase* skillBase_;										// SkillBase�N���X�̃|�C���^���(�R���X�g���N�^�ōŏ��Ɏ擾)
	SkillBase* skillSprite_;									// skillSprite_�̏��(Skill�������ɍX�V)
	cocos2d::Layer& enemyList_;									// �X�L������������擾����ۂɗ��p

	std::shared_ptr<ResidualShadow> resShadow_;					// ���g

	std::vector<std::string> SkillTbl_;							// �X�L�����̃e�[�u��
	std::vector<std::string> SkillSETbl_;						// �X�L��SE���̃e�[�u��

	char attackMotion_;													// �U�����[�V�����t���O(1bit���g�p)
	std::map<std::string, std::vector<std::string>> attackMotionName_;	// �L�[:�v���C���[���[�h,�f�[�^:�U�����[�V������
	const cocos2d::Vec2 charSize_;								// �L�����摜�T�C�Y

	std::vector<PlParam> plParam_;								// ���[�h���̃p�����[�^�ۑ��ϐ�
};