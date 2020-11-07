#pragma once
#include <array>
#include <list>
#include <map>
#include <string>
#include "input/OPRT_state.h"

struct ActModule;

// �֐��I�u�W�F�N�g��list�ɓo�^���邽�߂̏�����
using funcActModule = std::function<bool(cocos2d::Sprite&, ActModule&)>;

struct ActModule {
	OPRT_state* state;											// �L�[���
	cocos2d::Vec2 vel;											// ���E�ړ��̑��x
	bool flipFlg;												// �A�j���[�V�����̔��]����
	cocos2d::Vec2 map;											// ��ʊO�ɏo�Ȃ��悤�ɂ���
	cocos2d::Vec2 gravity;										// �d��
	cocos2d::Vec2 addGravity;									// �d�͂ɉ����Ă���
	cocos2d::Vec2 jumpVel;										// �W�����v���x
	cocos2d::Vec2 subtraction;									// �W�����v�l�������Ă���
	std::string actName;										// �v���C���[�̃A�N�V�������(�������݂������)
	BUTTON button;												// �L�[�������

	cocos2d::Vec2 checkPoint1;									// �v���C���[�̎l���̂ǂ���(1��)
	cocos2d::Vec2 checkPoint2;									// �v���C���[�̎l���̂ǂ���(2��)
	cocos2d::Vec2 checkPoint3;									// �����Ŏg���e�X�g
	cocos2d::Vec2 checkPoint4;									// �����Ŏg���e�X�g

	TOUCH_TIMMING touch;

	std::list<std::string>blackList;									// �u���b�N���X�g(�����Ă͂����Ȃ����)
	std::list<std::string>whiteList;									// �z���C�g���X�g(�����Ă������)

	funcActModule runAction;
	std::list<funcActModule>act;

	float flame;												// �����ƃW�����v�ɂ����t���[����

	bool jumpFlg;

	int num;
};