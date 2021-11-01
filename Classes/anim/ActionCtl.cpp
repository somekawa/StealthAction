// データ読み込み処理、プレイヤー関連処理担当
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
	// チェック関係(プレイヤーとエネミー共通)
	checkStructFunc_.emplace("CheckKeyList", CheckKeyList());
	checkStructFunc_.emplace("CheckList"   , CheckList());
	checkStructFunc_.emplace("CheckObjHit" , CheckObjHit());
	// チェック関係(エネミー)
	checkStructFunc_.emplace("FlipJudgement"		  , FlipJudgement());
	checkStructFunc_.emplace("PatrolJudgement"		  , PatrolJudgement());
	checkStructFunc_.emplace("ChaseJudgement"		  , ChaseJudgement());
	checkStructFunc_.emplace("PhysicalAttackJudgement", PhysicalAttackJudgement());

	// アクション関係(プレイヤー)
	plActStructFunc_.emplace("Flip"       , Flip());
	plActStructFunc_.emplace("Move_LR"    , Move_LR());
	plActStructFunc_.emplace("FallFalling", FallFalling());
	plActStructFunc_.emplace("Attack"     , Attack());
	plActStructFunc_.emplace("WallSlide"  , WallSlide());
	plActStructFunc_.emplace("Jump"       , Jump());
	plActStructFunc_.emplace("JumpJumping", JumpJumping());
	// アクション関係(エネミー)
	enActStructFunc_.emplace("FlipAction"		   , FlipAction());
	enActStructFunc_.emplace("MoveLRAction"		   , MoveLRAction());
	enActStructFunc_.emplace("FallAction"		   , FallAction());
	enActStructFunc_.emplace("PhysicalAttackAction", PhysicalAttackAction());

	// xmlデータの読み込み
	XmlPurser("player", plXmlData_);
	XmlPurser("enemy" , enXmlData_);
}

ActionCtl::~ActionCtl()
{
}

void ActionCtl::Update(ActorType type,Sprite& sprite)
{
	// 各アクターの更新
	updater_[type](sprite);
}

void ActionCtl::InitAct(ActorType type, AnimationType actName,ActModule& module)
{
	// blackListに引っかからないときにはwhiteListに追加
	mapFlame_.emplace(actName, 0.0f);

	// モジュール設定
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
			// mapModuleに登録した内容を順番に呼び出してcheckする
			// checklistとか
			auto actCheck = [&](AnimationType actName) {
				for (auto check : mapModule_[actName].act)
				{
					// mapFlameの内容をmapModuleのフレーム情報に代入
					mapModule_[actName].flame = mapFlame_[actName];

					if (!(check(sprite, mapModule_[actName])))
					{
						// フレーム値を0に戻す
						mapFlame_[actName] = 0.0f;
						// 1つでも引っかかったらfalse
						return false;
					}
				}
				// すべて抜けてきたらtrue
				return true;
			};

			/*<参考例>*/
			//if (actCheck("左移動"))
			//{
			//	_mapModule["左移動"].runAction(sprite, _mapModule["左移動"]);
			//}

			// 何かアクションが行われているかを判定するフラグ
			bool actFlg = false;

			// 上のラムダ式でtrueだった場合のみ処理が走る
			for (auto data : mapModule_)
			{
				if (actCheck(data.first))
				{
					// フレーム値の更新
					data.second.flame = mapFlame_[data.first];

					// (左右移動とかの)処理が走るところ
					data.second.runAction(sprite, data.second);

					// data.second.action = for文で回してるアクション
					// GetActionは実際に今行われているアクション
					if (data.second.actName != "Non" ||
						dynamic_cast<Player&>(sprite).GetAction() == "Fall" || dynamic_cast<Player&>(sprite).GetAction() == "Jump" || dynamic_cast<Player&>(sprite).GetAction() == "Jumping")
					{
						// 現在のアクションがジャンプになっているときもJUMPINGを設定するようにしておく
						if (dynamic_cast<Player&>(sprite).GetAction() == "Jumping" || dynamic_cast<Player&>(sprite).GetAction() == "Jump")
						{
							dynamic_cast<Player&>(sprite).SetAction("Jumping");
						}
						else
						{
							dynamic_cast<Player&>(sprite).SetAction(data.second.actName);
						}
					}

					// フレームの加算(落下とジャンプで使用している)
					mapFlame_[data.first] += 0.1f;
					if (mapFlame_[data.first] >= 3.0f)
					{
						mapFlame_[data.first] = 3.0f;
					}

					// ここはジャンプの最高点に到達したときとかに関係する処理
					if (dynamic_cast<Player&>(sprite).GetAction() == "Jumping" || dynamic_cast<Player&>(sprite).GetAction() == "Jumping")
					{
						// _mapFlame[data.first]にしてしまうと、現在のアクションがジャンプでも
						// 左右移動とかの加算された値が3.0fを超えていたら落下に切り替わる原因になってた
						// else ifの部分を付け加えて、ジャンピングを0.1fからにすることで角でジャンプし続けるバグ解消
						if (mapFlame_[AnimationType::Jumping] >= 0.1f && mapFlame_[AnimationType::Jumping] < 3.0f)
						{
							dynamic_cast<Player&>(sprite).SetAction("Jumping");
						}
						else if (mapFlame_[AnimationType::Jump] >= 0.1f)
						{
							// ジャンプし始めに必要
							dynamic_cast<Player&>(sprite).SetAction("Jumping");
						}
						else
						{
							// 最高点に到達したら落下に切り替える
							dynamic_cast<Player&>(sprite).SetAction("Fall");
						}
					}
					actFlg = true;
				}
				else
				{
					// プレイヤーが落下できなかった(着地している)ときは、IDLEにする
					// 今まで(書き換えたい情報)がFALLINGの状態だった && 現在の状態がFALLINGであるときIDLEにする
					if (data.second.actName == "Fall" && dynamic_cast<Player&>(sprite).GetAction() == "Fall")
					{
						dynamic_cast<Player&>(sprite).SetAction("Look_Intro");
					}
				}

				// プレイヤー向き変更
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
					// 何も処理を行わない
				}
			}

			// 何もアクションが行われていなければIDOLを設定する
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
					// mapFlameの内容をmapModuleのフレーム情報に代入
					mapModule_[actName].flame = mapFlame_[actName];

					if (!(check(sprite, mapModule_[actName])))
					{
						// フレーム値を0に戻す
						mapFlame_[actName] = 0.0f;
						// 1つでも引っかかったらfalse
						return false;
					}
				}
				// すべて抜けてきたらtrue
				return true;
			};
			// 何かアクションが行われているかを判定するフラグ
			bool actFlg = false;

			// 上のラムダ式でtrueだった場合のみ処理が走る
			for (auto data : mapModule_)
			{
				if (actCheck(data.first))
				{
					// フレーム値の更新
					data.second.flame = mapFlame_[data.first];

					// (左右移動とかの)処理が走るところ
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
						// 何も処理を行わない
					}
				}
				// フレームの加算(落下とジャンプで使用している)
				mapFlame_[data.first] += 0.1f;
				if (mapFlame_[data.first] >= 3.0f)
				{
					mapFlame_[data.first] = 3.0f;
				}
			}
			// 何もアクションが行われていなければidleを設定する
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
					// mapFlameの内容をmapModuleのフレーム情報に代入
					mapModule_[actName].flame = mapFlame_[actName];

					if (!(check(sprite, mapModule_[actName])))
					{
						// フレーム値を0に戻す
						mapFlame_[actName] = 0.0f;
						// 1つでも引っかかったらfalse
						return false;
					}
				}
				// すべて抜けてきたらtrue
				return true;
			};
			// 何かアクションが行われているかを判定するフラグ
			bool actFlg = false;

			// 上のラムダ式でtrueだった場合のみ処理が走る
			for (auto data : mapModule_)
			{
				if (actCheck(data.first))
				{
					// フレーム値の更新
					data.second.flame = mapFlame_[data.first];

					// (左右移動とかの)処理が走るところ
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
						// 何も処理を行わない
					}
				}

				// フレームの加算(落下とジャンプで使用している)
				mapFlame_[data.first] += 0.1f;
				if (mapFlame_[data.first] >= 3.0f)
				{
					mapFlame_[data.first] = 3.0f;
				}
			}
			// 何もアクションが行われていなければidleを設定する
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
					// mapFlameの内容をmapModuleのフレーム情報に代入
					mapModule_[actName].flame = mapFlame_[actName];

					if (!(check(sprite, mapModule_[actName])))
					{
						// フレーム値を0に戻す
						mapFlame_[actName] = 0.0f;
						// 1つでも引っかかったらfalse
						return false;
					}
				}
				// すべて抜けてきたらtrue
				return true;
			};
			// 何かアクションが行われているかを判定するフラグ
			bool actFlg = false;

			// 上のラムダ式でtrueだった場合のみ処理が走る
			for (auto data : mapModule_)
			{
				if (actCheck(data.first))
				{

					// フレーム値の更新
					data.second.flame = mapFlame_[data.first];

					// (左右移動とかの)処理が走るところ
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
						// 何も処理を行わない
					}
				}

				// フレームの加算(落下とジャンプで使用している)
				mapFlame_[data.first] += 0.1f;
				if (mapFlame_[data.first] >= 3.0f)
				{
					mapFlame_[data.first] = 3.0f;
				}
			}
			// 何もアクションが行われていなければidleを設定する
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
	// xmlから読み込む
	const std::string path = "../Resources/Data/ActCtlData/";   // ここで../がないとファイルが探せない
	const std::string xml  = ".xml";
	const std::string f_name = path + fileName + xml;
	rapidxml::xml_document<> doc;
	rapidxml::file<> file(f_name.c_str());
	doc.parse<0>(file.data(), file.size());
	rapidxml::xml_node<>* parentNode = doc.first_node("data");

	int num = 1;	// AnimationTypeのNonを飛ばして開始する為に初期値を1にする

	// それぞれがactで1かたまり状態
	// AnimationTypeをキーにしてpairで登録
	for (rapidxml::xml_node<char>* actItr = parentNode->first_node("act"); actItr != nullptr; actItr = actItr->next_sibling())
	{
		std::vector<std::string> tmpVec;		// runactionが1つに対してcheckは複数ある為、vectorに保存する
		for (rapidxml::xml_node<char>* itr = actItr->first_node("checks")->first_node("check"); itr != nullptr; itr = itr->next_sibling())
		{
			// check関係が入る
			tmpVec.emplace_back(itr->value());
		}

		// 登録
		xmlmap[static_cast<AnimationType>(num)] = std::make_pair(actItr->first_node("runaction")->value(), tmpVec);	// runactionの1つに対して、check関連複数個を登録する方法
		num++;
	}
}