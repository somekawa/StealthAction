#pragma once
#include <cocos2d.h>
#include <utility>
#include <vector>
#include <map>
#include <array>
#include <functional>
#include <string>
#include "input/OPRT_state.h"
#include "module/ActModule.h"
#include "obj/ActorType.h"
#include "../AnimationEnum/AnimationType.h"

// first:runaction,second:check�֘A
using xmlMap = std::map<AnimationType, std::pair<std::string, std::vector<std::string>>>;
// �L�[:runaction�܂���check�֘A�̍\���̖�
using actFunc = std::map<std::string, std::function<bool(cocos2d::Sprite& sprite, ActModule& module)>>;

class ActionCtl
{
public:
	ActionCtl();
	~ActionCtl();
	// ActCtl�̍X�V(�^�C�v�ʂ�Update����)
	void Update(ActorType type,cocos2d::Sprite& sprite);

	// Actor�̃^�C�v���̃A�N�V�����o�^�p�֐��̎��s
	void InitAct(ActorType type, AnimationType actName,ActModule& module);
	// �X�̃A�N�^�[��module�̍X�V�֐��̓o�^
	void InitUpdater(ActorType& type);
private:
	// �X�̃A�N�^�[���̍X�V�֐�(�Ԃ�l��actFlg��bool�ɂ�����)
	std::map<ActorType, std::function<void(cocos2d::Sprite&)>> updater_;
	std::map<AnimationType, float> mapFlame_;	  // �t���[����
	std::map<AnimationType, ActModule> mapModule_;// ���W���[��

	actFunc checkStructFunc_;	// �`�F�b�N�֌W�̍\���̂�o�^
	actFunc plActStructFunc_;	// �v���C���[�̃��W���[���֌W�̍\���̂�o�^
	actFunc enActStructFunc_;	// �G�l�~�[�̃��W���[���֌W�̍\���̂�o�^

	void XmlPurser(std::string fileName,xmlMap& xmlmap);	// xml�f�[�^��ǂݍ���
	xmlMap plXmlData_;										// �v���C���[�Ɋւ���f�[�^��ۑ�������
	xmlMap enXmlData_;										// �G�l�~�[�Ɋւ���f�[�^��ۑ�������
};