#pragma once
#include <array>
#include <map>
#include "InputID.h"

// 入力装置の種類
enum class CONTROLLER_TYPE {
	KEYBOARD,
	MOUSE,
	GAMEPAD,
	GAMEPAD_2DAI,	// 2つ以上の接続確認時
	MAX
};

// トリガー
enum class TRG
{
	NOW,
	OLD,
	MAX
};

// 現在と1フレーム前の情報を保存していく
using TrgBool = std::array<bool, static_cast<size_t>(TRG::MAX)>;
// TrgBoolがInputID分存在するようにする
using CtlData = std::map<InputID,TrgBool>;

struct Controller
{
	void operator()()
	{
		Update();
	}
	const CtlData& GetCtlData(void)				// 読み取り専用
	{
		return _data;	
	}

	virtual CONTROLLER_TYPE GetType(void) = 0;	// 自分の入力装置を返す
	virtual bool Setup(void) = 0;				// 初期化(初期化成功でtrue):引数はmyID?
	virtual void Rotate(int no) = 0;
	virtual void ChangeStage(int no, std::string str) = 0;
private:
	virtual void Update(void) = 0;				// 情報を取得して_dataに格納できるようにする
protected:
	CtlData _data;								// 外部で使われるのはこれ
};

