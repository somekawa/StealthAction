#pragma once
#include <array>
#include <vector>
#include <map>
#include "OPRT_state.h"

#define SWIPE_LEFT   1
#define SWIPE_RIGHT  3
#define SWIPE_DOWN   2
#define SWIPE_UP     4
#define SWIPE_CENTER 0

#define MULTI_TOUCH_MAX 2		// �����ɍs���鑀��̐�

// �L�[���֌W
struct KeyData {
	std::array<bool, static_cast<int>(BUTTON::MAX)> data_;					// keyflg�ŏ�������
	std::array<bool, static_cast<int>(BUTTON::MAX)> oldData_;
	std::array<bool, static_cast<int>(BUTTON::MAX)> input_;
};

struct Touches
{
	cocos2d::Vec2 pos;
	bool isMoveTouch;
	bool isAttackTouch;
	Touches() : pos(cocos2d::Vec2::ZERO), isMoveTouch(false), isAttackTouch(false) {};
};

struct OPRT_touch : OPRT_state
{
	OPRT_touch(cocos2d::Sprite* sp);
	~OPRT_touch();

	OPRT_TYPE GetType(void);				// �^�b�`���͂ł��邱�Ƃ�Ԃ�
	void update();
	cocos2d::Point swipeStartPoint;			// �X���C�v����̍ŏ��̍��W
	cocos2d::Point swipeEndPoint;			// �X���C�v����̏I���̍��W
	int swipeRotate;						// �X���C�v�̕���

	const std::array<bool, static_cast<int>(BUTTON::MAX)> &GetNowData(void);	// ���̏���Ԃ�
	const std::array<bool, static_cast<int>(BUTTON::MAX)> &GetOldData(void);	// 1�t���[���O�̏���Ԃ�

	// �}���`�^�b�v(�o�[�`�����p�b�h�܂�)
	void StartSwipe(cocos2d::Touch* touch);	// �X���C�v���n�܂�����
	void MoveSwipe(cocos2d::Touch* touch);	// �X���C�v�̈ړ����s��ꂽ��
	void EndSwipe(cocos2d::Touch* touch);	// �X���C�v���I�������

	void SetupTouch(cocos2d::Sprite* delta);	// �^�b�`����̃Z�b�g�A�b�v
	std::vector<Touches> touchVectors;
	bool moveFlag;

	void ResetKey(void);						// �}�b�v�ؑ֎��̃L�[���Z�b�g
private:
	KeyData keyData_;							// �L�[�̃f�[�^���i�[�����ϐ�
	std::string trStr_;							// �g�����X�t�H�[���ؑւŎg�p����ϐ�
	std::map<std::string, BUTTON> map_;			// �{�^�������L�[�ɂ��ă{�^���̉���/�񉟉�����
	std::map<BUTTON,std::vector<std::string>> sceneBtn_;	// Scene�ɑ΂���{�^���̏���ۑ�����
	cocos2d::Node* SetBtnNameInfo(std::string str, BUTTON button);	// ���݂̕\��Scene����{�^��������v������return����
};