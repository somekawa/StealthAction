#include "obj/Player.h"
#include "input/OPRT_state.h"
#include "move/MOVE_LR.h"
#include "move/FallFalling.h"
#include "move/Jump.h"
#include "move/JumpJumping.h"
#include "move/Attack.h"
#include "Check/CheckKeyList.h"
#include "Check/CheckObjHit.h"
#include "Check/CheckList.h"
#include "Check/Flip.h"
#include "ActionCtl.h"
#include "_Debug/_DebugConOut.h"

USING_NS_CC;

ActionCtl::ActionCtl()
{
}

ActionCtl::~ActionCtl()
{
}

void ActionCtl::ActCtl(std::string actName,ActModule & module)
{
	// blackList�Ɉ���������Ȃ��Ƃ��ɂ�whiteList�ɒǉ�
	_mapFlame.emplace(actName,0.0f);

	// ���E�ړ�
	if (actName == "���ړ�" || actName == "�E�ړ�")
	{
		_mapModule.emplace(actName,std::move(module));
		_mapModule[actName].act.emplace_back(CheckKeyList());
		_mapModule[actName].act.emplace_back(CheckObjHit());
		_mapModule[actName].act.emplace_back(CheckList());
		_mapModule[actName].runAction = MOVE_LR();
	}

	// ���]
	if (actName == "������" || actName == "�E����")
	{
		_mapModule.emplace(actName,std::move(module));
		_mapModule[actName].act.emplace_back(CheckKeyList());
		_mapModule[actName].act.emplace_back(CheckList());
		//_mapModule[actName].act.emplace_back(CheckObjHit());
		_mapModule[actName].runAction = Flip();
	}

	if(actName == "����")
	{
		_mapModule.emplace(actName,std::move(module));
		//_mapModule[actName].act.emplace_back(CheckKeyList());
		_mapModule[actName].act.emplace_back(CheckObjHit());
		_mapModule[actName].act.emplace_back(CheckList());
		_mapModule[actName].runAction = FallFalling();
	}

	if (actName == "�W�����v")
	{
		_mapModule.emplace(actName,std::move(module));
		_mapModule[actName].act.emplace_back(CheckKeyList());
		_mapModule[actName].act.emplace_back(CheckObjHit());
		_mapModule[actName].act.emplace_back(CheckList());
		_mapModule[actName].runAction = Jump();
	}

	if (actName == "�W�����s���O")
	{
		_mapModule.emplace(actName, std::move(module));
		// �������ɂ���Ȃ�R�����g�A�E�g�͂���
		//_mapModule[actName].act.emplace_back(CheckKeyList());
		_mapModule[actName].act.emplace_back(CheckObjHit());
		_mapModule[actName].act.emplace_back(CheckList());
		_mapModule[actName].runAction = JumpJumping();
	}

	if (actName == "�U��")
	{
		_mapModule.emplace(actName, std::move(module));
		_mapModule[actName].act.emplace_back(CheckKeyList());
		_mapModule[actName].act.emplace_back(CheckObjHit());
		_mapModule[actName].act.emplace_back(CheckList());
		_mapModule[actName].runAction = Attack();
	}

	// jumping�̂Ƃ��͎��R�Ɨ�����Ƃ���collision�œ����������ɗ����ɐ؂�ւ���
	// collision�͊֐��I�u�W�F�N�g�ŌĂяo���K�v������
	//  ���������Ă�����؂�ւ���
}

void ActionCtl::update(float sp,Sprite& sprite)
{
	// mapModule�ɓo�^�������e�����ԂɌĂяo����check����
	// checklist�Ƃ�
	auto actCheck = [&](std::string actName){
		for (auto check : _mapModule[actName].act)
		{
			// mapFlame�̓��e��mapModule�̃t���[�����ɑ��
			_mapModule[actName].flame = _mapFlame[actName];

			if (!(check(sprite,_mapModule[actName])))
			{
				// �t���[���l��0�ɖ߂�
				_mapFlame[actName] = 0.0f;
				// 1�ł���������������false
				return false;
			}
		}
		// ���ׂĔ����Ă�����true
		return true;
	};

	/*<�Q�l��>*/
	//if (actCheck("���ړ�"))
	//{
	//	_mapModule["���ړ�"].runAction(sprite, _mapModule["���ړ�"]);
	//}

	// �����A�N�V�������s���Ă��邩�𔻒肷��t���O
	bool actFlg = false;

	// ��̃����_����true�������ꍇ�̂ݏ���������
	for (auto data : _mapModule)
	{
		if (actCheck(data.first))
		{
			// �t���[���l�̍X�V
			data.second.flame = _mapFlame[data.first];

			// (���E�ړ��Ƃ���)����������Ƃ���
			data.second.runAction(sprite, data.second);

			// data.second.action = for���ŉ񂵂Ă�A�N�V����
			// GetAction�͎��ۂɍ��s���Ă���A�N�V����
			if (((Player&)sprite).GetAction() == ACTION::FALLING || data.second.action != ACTION::NON || ((Player&)sprite).GetAction() == ACTION::JUMPING || ((Player&)sprite).GetAction() == ACTION::JUMP)
			{
				// ���݂̃A�N�V�������W�����v�ɂȂ��Ă���Ƃ���JUMPING��ݒ肷��悤�ɂ��Ă���
				if ((_mapFlame["�W�����s���O"] >= 0.1f && _mapFlame["�W�����s���O"] < 3.0f) || ((Player&)sprite).GetAction() == ACTION::JUMP)
				{
					((Player&)sprite).SetAction(ACTION::JUMPING);
				}
				else 
				{
					((Player&)sprite).SetAction(data.second.action);
				}
			}
			
			// �t���[���̉��Z(�����ƃW�����v�Ŏg�p���Ă���)
			_mapFlame[data.first]+= 0.1f;
			if (_mapFlame[data.first] >= 10.0f)
			{
				_mapFlame[data.first] = 10.0f;
			}

			// �����̓W�����v�̍ō��_�ɓ��B�����Ƃ��Ƃ��Ɋ֌W���鏈��
			if (((Player&)sprite).GetAction() == ACTION::JUMPING)
			{
				actFlg = true;
				//((Player&)sprite).SetAction(ACTION::JUMPING);

				// _mapFlame[data.first]�ɂ��Ă��܂��ƁA���݂̃A�N�V�������W�����v�ł�
				// ���E�ړ��Ƃ��̉��Z���ꂽ�l��3.0f�𒴂��Ă����痎���ɐ؂�ւ�錴���ɂȂ��Ă�
				if (_mapFlame["�W�����s���O"] < 3.0f)
				{
					((Player&)sprite).SetAction(ACTION::JUMPING);
					continue;
				}
				else
				{
					// �ō��_�ɓ��B�����痎���ɐ؂�ւ���
					((Player&)sprite).SetAction(ACTION::FALLING);
				}
			}

			actFlg = true;
		}
		else
		{
			// �v���C���[�������ł��Ȃ�����(���n���Ă���)�Ƃ��́AIDLE�ɂ���
			// ���܂�(���������������)��FALLING�̏�Ԃ����� && ���݂̏�Ԃ�FALLING�ł���Ƃ�IDLE�ɂ���
			if (data.second.action == ACTION::FALLING && ((Player&)sprite).GetAction() == ACTION::FALLING)
			{
				((Player&)sprite).SetAction(ACTION::IDLE);
			}
		}
	}

	// �����A�N�V�������s���Ă��Ȃ����IDOL��ݒ肷��
	if (!actFlg)
	{
		((Player&)sprite).SetAction(ACTION::IDLE);
	}
}