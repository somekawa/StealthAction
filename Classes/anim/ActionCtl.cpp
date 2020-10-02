#include "obj/Player.h"
#include "input/OPRT_state.h"
#include "move/MOVE_LR.h"
#include "move/FallFalling.h"
#include "move/JumpJumping.h"
#include "Check/CheckKeyList.h"
#include "Check/CheckObjHit.h"
#include "Check/CheckList.h"
#include "Check/Flip.h"
#include "ActionCtl.h"

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
		_mapModule[actName].runAction = JumpJumping();
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

			// 現在のステータスが落下(FALLING)以外のときは、ステータスを書き換えないようにする
			// また、NON(向きだけ変える)ときは、IDOLにならないようにする
			if (((Player&)sprite).GetAction() != ACTION::FALLING && data.second.action != ACTION::NON /*&& ((Player&)sprite).GetAction() != ACTION::JUMPING*/)
			{
				((Player&)sprite).SetAction(data.second.action);
			}
			
			// フレームの加算(落下とジャンプで使用している)
			_mapFlame[data.first]+= 0.1f;
			if (_mapFlame[data.first] >= 10.0f)
			{
				_mapFlame[data.first] = 10.0f;
			}

			if (((Player&)sprite).GetAction() == ACTION::JUMPING)
			{
				if (_mapFlame[data.first] < 3.0f)
				{
					//actFlg = true;
					return;
				}
				((Player&)sprite).SetAction(ACTION::FALLING);
			}

			actFlg = true;
		}
		else
		{
			// プレイヤーが落下できなかった(着地している)ときは、IDLEにする
			// 今まで(書き換えたい情報)がFALLINGの状態だった && 現在の状態がFALLINGであるときIDLEにする
			if (data.second.action == ACTION::FALLING && ((Player&)sprite).GetAction() == ACTION::FALLING)
			{
				((Player&)sprite).SetAction(ACTION::IDLE);
			}
		}
	}

	// 何もアクションが行われていなければIDOLを設定する
	if (!actFlg)
	{
		((Player&)sprite).SetAction(ACTION::IDLE);
	}
}


