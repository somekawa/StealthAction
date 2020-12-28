#include "obj/Player.h"
#include "input/OPRT_state.h"
#include "move/MOVE_LR.h"
#include "move/FallFalling.h"
#include "move/Jump.h"
#include "move/JumpJumping.h"
#include "move/Attack.h"
#include "move/WallSlide.h"
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
	// blackListに引っかからないときにはwhiteListに追加
	_mapFlame.emplace(actName,0.0f);

	// 左右移動
	if (actName == "左移動" || actName == "右移動")
	{
		_mapModule.emplace(actName,std::move(module));
		_mapModule[actName].act.emplace_back(CheckKeyList());
		_mapModule[actName].act.emplace_back(CheckObjHit());
		_mapModule[actName].act.emplace_back(CheckList());
		_mapModule[actName].runAction = MOVE_LR();
	}

	// 反転
	if (actName == "左向き" || actName == "右向き")
	{
		_mapModule.emplace(actName,std::move(module));
		_mapModule[actName].act.emplace_back(CheckKeyList());
		_mapModule[actName].act.emplace_back(CheckList());
		//_mapModule[actName].act.emplace_back(CheckObjHit());
		_mapModule[actName].runAction = Flip();
	}

	if(actName == "落下")
	{
		_mapModule.emplace(actName,std::move(module));
		//_mapModule[actName].act.emplace_back(CheckKeyList());
		_mapModule[actName].act.emplace_back(CheckObjHit());
		_mapModule[actName].act.emplace_back(CheckList());
		_mapModule[actName].runAction = FallFalling();
	}

	if (actName == "ジャンプ")
	{
		_mapModule.emplace(actName,std::move(module));
		_mapModule[actName].act.emplace_back(CheckKeyList());
		_mapModule[actName].act.emplace_back(CheckObjHit());
		_mapModule[actName].act.emplace_back(CheckList());
		_mapModule[actName].runAction = Jump();
	}

	if (actName == "ジャンピング")
	{
		_mapModule.emplace(actName, std::move(module));
		// 長押しにするならコメントアウトはずす
		//_mapModule[actName].act.emplace_back(CheckKeyList());
		_mapModule[actName].act.emplace_back(CheckObjHit());
		_mapModule[actName].act.emplace_back(CheckList());
		_mapModule[actName].runAction = JumpJumping();
	}

	if (actName == "攻撃")
	{
		_mapModule.emplace(actName, std::move(module));
		_mapModule[actName].act.emplace_back(CheckKeyList());
		//_mapModule[actName].act.emplace_back(CheckObjHit());
		//_mapModule[actName].act.emplace_back(CheckList());
		_mapModule[actName].runAction = Attack();
	}

	if (actName == "右壁スライド" || actName == "左壁スライド")
	{
		_mapModule.emplace(actName, std::move(module));
		_mapModule[actName].act.emplace_back(CheckKeyList());
		_mapModule[actName].act.emplace_back(CheckObjHit());
		_mapModule[actName].act.emplace_back(CheckList());
		_mapModule[actName].runAction = WallSlide();
	}
	// jumpingのときは自然と落ちるときとcollisionで当たった時に落下に切り替える
	// collisionは関数オブジェクトで呼び出す必要がある
	//  →当たっていたら切り替える
}

void ActionCtl::update(float sp,Sprite& sprite)
{
	// mapModuleに登録した内容を順番に呼び出してcheckする
	// checklistとか
	auto actCheck = [&](std::string actName){
		for (auto check : _mapModule[actName].act)
		{
			// mapFlameの内容をmapModuleのフレーム情報に代入
			_mapModule[actName].flame = _mapFlame[actName];

			if (!(check(sprite,_mapModule[actName])))
			{
				// フレーム値を0に戻す
				_mapFlame[actName] = 0.0f;
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
	for (auto data : _mapModule)
	{
		if (actCheck(data.first))
		{
			// フレーム値の更新
			data.second.flame = _mapFlame[data.first];

			// (左右移動とかの)処理が走るところ
			data.second.runAction(sprite, data.second);

			// data.second.action = for文で回してるアクション
			// GetActionは実際に今行われているアクション
			if (((Player&)sprite).GetAction() == "player_Fall" || data.second.actName != "player_NON"|| ((Player&)sprite).GetAction() == "player_Jump" || ((Player&)sprite).GetAction() == "player_Jumping")
			{
				// 現在のアクションがジャンプになっているときもJUMPINGを設定するようにしておく
				if (((Player&)sprite).GetAction() == "player_Jumping" || ((Player&)sprite).GetAction() == "player_Jump")
				{
					((Player&)sprite).SetAction("player_Jumping");
				}
				else 
				{
					((Player&)sprite).SetAction(data.second.actName);
				}
			}
			
			// フレームの加算(落下とジャンプで使用している)
			_mapFlame[data.first]+= 0.1f;
			if (_mapFlame[data.first] >= 3.0f)
			{
				_mapFlame[data.first] = 3.0f;
			}

			// 空中での攻撃ボタンで一時静止用
			if (_mapFlame["攻撃"] > 0.0f && _mapFlame["落下"] > 0.0f)
			{
				_mapModule["落下"].stopCnt = 1;
			}

			if (_mapModule["落下"].stopCnt == 1)
			{
				if (_mapFlame["落下"] >= 3.0f)
				{
					_mapModule["落下"].stopCnt = 0;
				}
			}

			//TRACE("%f\n", _mapFlame["ジャンプ"]);
			TRACE("%f\n", _mapFlame["ジャンピング"]);


			// ここはジャンプの最高点に到達したときとかに関係する処理
			if (((Player&)sprite).GetAction() == "player_Jumping" || ((Player&)sprite).GetAction() == "player_Jumping")
			{
				//actFlg = true;
				//((Player&)sprite).SetAction(ACTION::JUMPING);

				// _mapFlame[data.first]にしてしまうと、現在のアクションがジャンプでも
				// 左右移動とかの加算された値が3.0fを超えていたら落下に切り替わる原因になってた
				// else ifの部分を付け加えて、ジャンピングを0.1fからにすることで角でジャンプし続けるバグ解消
				if (_mapFlame["ジャンピング"] >= 0.1f && _mapFlame["ジャンピング"] < 3.0f)
				{
					((Player&)sprite).SetAction("player_Jumping");
					//continue;
				}
				else if (_mapFlame["ジャンプ"] >= 0.1f)
				{
					// ジャンプし始めに必要
					((Player&)sprite).SetAction("player_Jumping");
				}
				else 
				{
					// 最高点に到達したら落下に切り替える
					((Player&)sprite).SetAction("player_Fall");
				}
			}

			actFlg = true;
		}
		else
		{
			// プレイヤーが落下できなかった(着地している)ときは、IDLEにする
			// 今まで(書き換えたい情報)がFALLINGの状態だった && 現在の状態がFALLINGであるときIDLEにする
			if (data.second.actName == "player_Fall" && ((Player&)sprite).GetAction() == "player_Fall")
			{
				((Player&)sprite).SetAction("player_Look_Intro");
			}
		}

		// プレイヤー向き変更(アンカーポイントの変更に必要)
		if (actCheck("左向き"))
		{
			((Player&)sprite).SetDir(Direction::Left);
		}
		else if (actCheck("右向き"))
		{
			((Player&)sprite).SetDir(Direction::Right);
		}
	}

	// 何もアクションが行われていなければIDOLを設定する
	if (!actFlg)
	{
		((Player&)sprite).SetAction("player_Look_Intro");
	}
}