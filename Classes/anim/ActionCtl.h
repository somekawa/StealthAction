#pragma once
#include "cocos2d.h"
#include "input/OPRT_state.h"
#include "module/ActModule.h"
#include "obj/ActorType.h"
#include <map>
#include <array>
#include <functional>

class ActionCtl
{
public:
	ActionCtl();
	~ActionCtl();
	// ����͂����g�p���Ȃ�
	// RunInitializeActCtl�֐������ߕʂ�module��o�^�\�ɂ��Ă���
	void ActCtl(std::string actName, ActModule& module);
	// ActCtl�̍X�V(���ߕʂ�update���񂵂���)
	void update(ActorType type,float sp, cocos2d::Sprite& sprite);
	// Actor�����ߖ��̱���ݓo�^�p�֐��̎��s
	void RunInitializeActCtl(ActorType type,std::string actName,ActModule& module);
	// �X�̱�����module�̍X�V�֐��̓o�^
	void InitUpdater(ActorType& type);
private:
	// �X�̱������̍X�V�֐�
	std::map<ActorType, std::function<void(float, cocos2d::Sprite&)>> updater_;
	std::map<std::string, float> _mapFlame;		// �t���[����
	std::map<std::string, ActModule> _mapModule;// ���W���[��
};