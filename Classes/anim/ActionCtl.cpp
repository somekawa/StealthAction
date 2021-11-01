// �f�[�^�ǂݍ��ݏ����A�v���C���[�֘A�����S��
#include "_Debug/_DebugConOut.h"
#include "RapidXML/rapidxml.hpp"
#include "RapidXML/rapidxml_utils.hpp"
#include "obj/Player.h"
#include "obj/Enemy/Enemy.h"
#include "obj/Enemy/Assassin.h"
#include "obj/Enemy/Cultist.h"
#include "obj/Enemy/TwistedCultist.h"
#include "input/OPRT_state.h"
#include "move/Move_LR.h"
#include "move/FallFalling.h"
#include "move/Jump.h"
#include "move/JumpJumping.h"
#include "move/Attack.h"
#include "move/WallSlide.h"
#include "Check/CheckKeyList.h"
#include "Check/CheckObjHit.h"
#include "Check/CheckList.h"
#include "Check/Flip.h"
#include "Effect/EffectManager.h"
#include "ActionCtl.h"

#include "EnemyAIFunctions/Judgement/ChaseJudgement.h"
#include "EnemyAIFunctions/Judgement/PatrolJudgement.h"
#include "EnemyAIFunctions/Judgement/FlipJudgement.h"
#include "EnemyAIFunctions/Judgement/PhysicalAttackJudgement.h"

#include "EnemyAIFunctions/Actions/ChaseAction.h"
#include "EnemyAIFunctions/Actions/FlipAction.h"
#include "EnemyAIFunctions/Actions/PatrolAction.h"
#include "EnemyAIFunctions/Actions/FallAction.h"
#include "EnemyAIFunctions/Actions/PhysicalAttackAction.h"
#include "EnemyAIFunctions/Actions/MoveLRAction.h"

USING_NS_CC;

ActionCtl::ActionCtl()
{
	// �`�F�b�N�֌W(�v���C���[�ƃG�l�~�[����)
	checkStructFunc_.emplace("CheckKeyList", CheckKeyList());
	checkStructFunc_.emplace("CheckList"   , CheckList());
	checkStructFunc_.emplace("CheckObjHit" , CheckObjHit());
	// �`�F�b�N�֌W(�G�l�~�[)
	checkStructFunc_.emplace("FlipJudgement"		  , FlipJudgement());
	checkStructFunc_.emplace("PatrolJudgement"		  , PatrolJudgement());
	checkStructFunc_.emplace("ChaseJudgement"		  , ChaseJudgement());
	checkStructFunc_.emplace("PhysicalAttackJudgement", PhysicalAttackJudgement());

	// �A�N�V�����֌W(�v���C���[)
	plActStructFunc_.emplace("Flip"       , Flip());
	plActStructFunc_.emplace("Move_LR"    , Move_LR());
	plActStructFunc_.emplace("FallFalling", FallFalling());
	plActStructFunc_.emplace("Attack"     , Attack());
	plActStructFunc_.emplace("WallSlide"  , WallSlide());
	plActStructFunc_.emplace("Jump"       , Jump());
	plActStructFunc_.emplace("JumpJumping", JumpJumping());
	// �A�N�V�����֌W(�G�l�~�[)
	enActStructFunc_.emplace("FlipAction"		   , FlipAction());
	enActStructFunc_.emplace("MoveLRAction"		   , MoveLRAction());
	enActStructFunc_.emplace("FallAction"		   , FallAction());
	enActStructFunc_.emplace("PhysicalAttackAction", PhysicalAttackAction());

	// xml�f�[�^�̓ǂݍ���
	XmlPurser("player", plXmlData_);
	XmlPurser("enemy" , enXmlData_);
}

ActionCtl::~ActionCtl()
{
}

void ActionCtl::Update(ActorType type,Sprite& sprite)
{
	// �e�A�N�^�[�̍X�V
	updater_[type](sprite);
}

void ActionCtl::InitAct(ActorType type, AnimationType actName,ActModule& module)
{
	// blackList�Ɉ���������Ȃ��Ƃ��ɂ�whiteList�ɒǉ�
	mapFlame_.emplace(actName, 0.0f);

	// ���W���[���ݒ�
	auto setModule = [&](ActorType type) {
		if (type != ActorType::Player)
		{
			for (auto data : enXmlData_[actName].second)
			{
				mapModule_[actName].act.emplace_back(checkStructFunc_[data]);
			}
			mapModule_[actName].runAction = enActStructFunc_[enXmlData_[actName].first];
		}
		else
		{
			for (auto data : plXmlData_[actName].second)
			{
				mapModule_[actName].act.emplace_back(checkStructFunc_[data]);
			}
			mapModule_[actName].runAction = plActStructFunc_[plXmlData_[actName].first];
		}
	};

	if (actName == AnimationType::L_move || actName == AnimationType::R_move)
	{
		mapModule_.emplace(actName, std::move(module));
		mapModule_[actName].commonAction = actName;
		setModule(type);
	}
	else if (actName == AnimationType::L_dir || actName == AnimationType::R_dir   ||
			 actName == AnimationType::Fall  || actName == AnimationType::Attack1)
	{
		mapModule_.emplace(actName, std::move(module));
		setModule(type);
	}
	else
	{
		if (type == ActorType::Player)
		{
			if (actName == AnimationType::L_wallSlide || actName == AnimationType::R_wallSlide ||
				actName == AnimationType::Jump        || actName == AnimationType::Jumping)
			{
				mapModule_.emplace(actName, std::move(module));
				setModule(type);
			}
		}
	}
}

void ActionCtl::InitUpdater(ActorType& type)
{
	switch (type)
	{
	case ActorType::Player:
		updater_.emplace(type, [&](Sprite & sprite) {
			// mapModule�ɓo�^�������e�����ԂɌĂяo����check����
			// checklist�Ƃ�
			auto actCheck = [&](AnimationType actName) {
				for (auto check : mapModule_[actName].act)
				{
					// mapFlame�̓��e��mapModule�̃t���[�����ɑ��
					mapModule_[actName].flame = mapFlame_[actName];

					if (!(check(sprite, mapModule_[actName])))
					{
						// �t���[���l��0�ɖ߂�
						mapFlame_[actName] = 0.0f;
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
			for (auto data : mapModule_)
			{
				if (actCheck(data.first))
				{
					// �t���[���l�̍X�V
					data.second.flame = mapFlame_[data.first];

					// (���E�ړ��Ƃ���)����������Ƃ���
					data.second.runAction(sprite, data.second);

					// data.second.action = for���ŉ񂵂Ă�A�N�V����
					// GetAction�͎��ۂɍ��s���Ă���A�N�V����
					if (data.second.actName != "Non" ||
						dynamic_cast<Player&>(sprite).GetAction() == "Fall" || dynamic_cast<Player&>(sprite).GetAction() == "Jump" || dynamic_cast<Player&>(sprite).GetAction() == "Jumping")
					{
						// ���݂̃A�N�V�������W�����v�ɂȂ��Ă���Ƃ���JUMPING��ݒ肷��悤�ɂ��Ă���
						if (dynamic_cast<Player&>(sprite).GetAction() == "Jumping" || dynamic_cast<Player&>(sprite).GetAction() == "Jump")
						{
							dynamic_cast<Player&>(sprite).SetAction("Jumping");
						}
						else
						{
							dynamic_cast<Player&>(sprite).SetAction(data.second.actName);
						}
					}

					// �t���[���̉��Z(�����ƃW�����v�Ŏg�p���Ă���)
					mapFlame_[data.first] += 0.1f;
					if (mapFlame_[data.first] >= 3.0f)
					{
						mapFlame_[data.first] = 3.0f;
					}

					// �����̓W�����v�̍ō��_�ɓ��B�����Ƃ��Ƃ��Ɋ֌W���鏈��
					if (dynamic_cast<Player&>(sprite).GetAction() == "Jumping" || dynamic_cast<Player&>(sprite).GetAction() == "Jumping")
					{
						// _mapFlame[data.first]�ɂ��Ă��܂��ƁA���݂̃A�N�V�������W�����v�ł�
						// ���E�ړ��Ƃ��̉��Z���ꂽ�l��3.0f�𒴂��Ă����痎���ɐ؂�ւ�錴���ɂȂ��Ă�
						// else if�̕�����t�������āA�W�����s���O��0.1f����ɂ��邱�ƂŊp�ŃW�����v��������o�O����
						if (mapFlame_[AnimationType::Jumping] >= 0.1f && mapFlame_[AnimationType::Jumping] < 3.0f)
						{
							dynamic_cast<Player&>(sprite).SetAction("Jumping");
						}
						else if (mapFlame_[AnimationType::Jump] >= 0.1f)
						{
							// �W�����v���n�߂ɕK�v
							dynamic_cast<Player&>(sprite).SetAction("Jumping");
						}
						else
						{
							// �ō��_�ɓ��B�����痎���ɐ؂�ւ���
							dynamic_cast<Player&>(sprite).SetAction("Fall");
						}
					}
					actFlg = true;
				}
				else
				{
					// �v���C���[�������ł��Ȃ�����(���n���Ă���)�Ƃ��́AIDLE�ɂ���
					// ���܂�(���������������)��FALLING�̏�Ԃ����� && ���݂̏�Ԃ�FALLING�ł���Ƃ�IDLE�ɂ���
					if (data.second.actName == "Fall" && dynamic_cast<Player&>(sprite).GetAction() == "Fall")
					{
						dynamic_cast<Player&>(sprite).SetAction("Look_Intro");
					}
				}

				// �v���C���[�����ύX
				if (actCheck(AnimationType::L_dir))
				{
					dynamic_cast<Player&>(sprite).SetDir(Direction::Left);
				}
				else if (actCheck(AnimationType::R_dir))
				{
					dynamic_cast<Player&>(sprite).SetDir(Direction::Right);
				}
				else
				{
					// �����������s��Ȃ�
				}
			}

			// �����A�N�V�������s���Ă��Ȃ����IDOL��ݒ肷��
			if (!actFlg)
			{
				dynamic_cast<Player&>(sprite).SetAction("Look_Intro");
			}
		});
		break;
	case ActorType::Assassin:
		updater_.emplace(type, [&](Sprite & sprite) {
			auto actCheck = [&](AnimationType actName) {
				for (auto check : mapModule_[actName].act)
				{
					// mapFlame�̓��e��mapModule�̃t���[�����ɑ��
					mapModule_[actName].flame = mapFlame_[actName];

					if (!(check(sprite, mapModule_[actName])))
					{
						// �t���[���l��0�ɖ߂�
						mapFlame_[actName] = 0.0f;
						// 1�ł���������������false
						return false;
					}
				}
				// ���ׂĔ����Ă�����true
				return true;
			};
			// �����A�N�V�������s���Ă��邩�𔻒肷��t���O
			bool actFlg = false;

			// ��̃����_����true�������ꍇ�̂ݏ���������
			for (auto data : mapModule_)
			{
				if (actCheck(data.first))
				{
					// �t���[���l�̍X�V
					data.second.flame = mapFlame_[data.first];

					// (���E�ړ��Ƃ���)����������Ƃ���
					data.second.runAction(sprite, data.second);
					actFlg = true;
				}
				else
				{
					if (data.first == AnimationType::R_move)
					{
						dynamic_cast<Enemy&>(sprite).SetDirection(Direction::Left);
					}
					else if (data.first == AnimationType::L_move)
					{
						dynamic_cast<Enemy&>(sprite).SetDirection(Direction::Right);
					}
					else
					{
						// �����������s��Ȃ�
					}
				}
				// �t���[���̉��Z(�����ƃW�����v�Ŏg�p���Ă���)
				mapFlame_[data.first] += 0.1f;
				if (mapFlame_[data.first] >= 3.0f)
				{
					mapFlame_[data.first] = 3.0f;
				}
			}
			// �����A�N�V�������s���Ă��Ȃ����idle��ݒ肷��
			if (!actFlg)
			{
				dynamic_cast<Enemy&>(sprite).SetAction("assassin_idle");
			}
		});
		break;
	case ActorType::TwistedCultist:
		updater_.emplace(type, [&](Sprite & sprite) {
			auto actCheck = [&](AnimationType actName) {
				for (auto check : mapModule_[actName].act)
				{
					// mapFlame�̓��e��mapModule�̃t���[�����ɑ��
					mapModule_[actName].flame = mapFlame_[actName];

					if (!(check(sprite, mapModule_[actName])))
					{
						// �t���[���l��0�ɖ߂�
						mapFlame_[actName] = 0.0f;
						// 1�ł���������������false
						return false;
					}
				}
				// ���ׂĔ����Ă�����true
				return true;
			};
			// �����A�N�V�������s���Ă��邩�𔻒肷��t���O
			bool actFlg = false;

			// ��̃����_����true�������ꍇ�̂ݏ���������
			for (auto data : mapModule_)
			{
				if (actCheck(data.first))
				{
					// �t���[���l�̍X�V
					data.second.flame = mapFlame_[data.first];

					// (���E�ړ��Ƃ���)����������Ƃ���
					data.second.runAction(sprite, data.second);
					actFlg = true;

				}
				else
				{
					if (data.first == AnimationType::R_move)
					{
						dynamic_cast<Enemy&>(sprite).SetDirection(Direction::Left);
					}
					else if (data.first == AnimationType::L_move)
					{
						dynamic_cast<Enemy&>(sprite).SetDirection(Direction::Right);
					}
					else
					{
						// �����������s��Ȃ�
					}
				}

				// �t���[���̉��Z(�����ƃW�����v�Ŏg�p���Ă���)
				mapFlame_[data.first] += 0.1f;
				if (mapFlame_[data.first] >= 3.0f)
				{
					mapFlame_[data.first] = 3.0f;
				}
			}
			// �����A�N�V�������s���Ă��Ȃ����idle��ݒ肷��
			if (!actFlg)
			{
				TRACE("idle_enemy\n");
				dynamic_cast<Enemy&>(sprite).SetAction("twistedCultist_idle");
			}
		});
		break;
	case ActorType::Cultist:
		updater_.emplace(type, [&](Sprite & sprite) {
			auto actCheck = [&](AnimationType actName) {
				for (auto check : mapModule_[actName].act)
				{
					// mapFlame�̓��e��mapModule�̃t���[�����ɑ��
					mapModule_[actName].flame = mapFlame_[actName];

					if (!(check(sprite, mapModule_[actName])))
					{
						// �t���[���l��0�ɖ߂�
						mapFlame_[actName] = 0.0f;
						// 1�ł���������������false
						return false;
					}
				}
				// ���ׂĔ����Ă�����true
				return true;
			};
			// �����A�N�V�������s���Ă��邩�𔻒肷��t���O
			bool actFlg = false;

			// ��̃����_����true�������ꍇ�̂ݏ���������
			for (auto data : mapModule_)
			{
				if (actCheck(data.first))
				{

					// �t���[���l�̍X�V
					data.second.flame = mapFlame_[data.first];

					// (���E�ړ��Ƃ���)����������Ƃ���
					data.second.runAction(sprite, data.second);
					actFlg = true;

				}
				else
				{
					if (data.first == AnimationType::R_move)
					{
						dynamic_cast<Enemy&>(sprite).SetDirection(Direction::Left);
					}
					else if (data.first == AnimationType::L_move)
					{
						dynamic_cast<Enemy&>(sprite).SetDirection(Direction::Right);
					}
					else
					{
						// �����������s��Ȃ�
					}
				}

				// �t���[���̉��Z(�����ƃW�����v�Ŏg�p���Ă���)
				mapFlame_[data.first] += 0.1f;
				if (mapFlame_[data.first] >= 3.0f)
				{
					mapFlame_[data.first] = 3.0f;
				}
			}
			// �����A�N�V�������s���Ă��Ȃ����idle��ݒ肷��
			if (!actFlg)
			{
				dynamic_cast<Enemy&>(sprite).SetAction("cultist_idle");
			}
		});
		break;
	case ActorType::Max:
		break;
	default:
		break;
	}

}

void ActionCtl::XmlPurser(std::string fileName, xmlMap& xmlmap)
{
	// xml����ǂݍ���
	const std::string path = "../Resources/Data/ActCtlData/";   // ������../���Ȃ��ƃt�@�C�����T���Ȃ�
	const std::string xml  = ".xml";
	const std::string f_name = path + fileName + xml;
	rapidxml::xml_document<> doc;
	rapidxml::file<> file(f_name.c_str());
	doc.parse<0>(file.data(), file.size());
	rapidxml::xml_node<>* parentNode = doc.first_node("data");

	int num = 1;	// AnimationType��Non���΂��ĊJ�n����ׂɏ����l��1�ɂ���

	// ���ꂼ�ꂪact��1�����܂���
	// AnimationType���L�[�ɂ���pair�œo�^
	for (rapidxml::xml_node<char>* actItr = parentNode->first_node("act"); actItr != nullptr; actItr = actItr->next_sibling())
	{
		std::vector<std::string> tmpVec;		// runaction��1�ɑ΂���check�͕�������ׁAvector�ɕۑ�����
		for (rapidxml::xml_node<char>* itr = actItr->first_node("checks")->first_node("check"); itr != nullptr; itr = itr->next_sibling())
		{
			// check�֌W������
			tmpVec.emplace_back(itr->value());
		}

		// �o�^
		xmlmap[static_cast<AnimationType>(num)] = std::make_pair(actItr->first_node("runaction")->value(), tmpVec);	// runaction��1�ɑ΂��āAcheck�֘A������o�^������@
		num++;
	}
}