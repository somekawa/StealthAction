#pragma once
#include <array>
#include <map>
#include "Controller.h"

using KEY_ARRAY = std::array<char, 256>;

struct KeyBoard :
	public Controller
{
	CONTROLLER_TYPE GetType(void) override;		// 自分の入力装置を返す
	bool Setup(void) override;					// 初期化(初期化成功でtrue)
	void Update(void)  override;				// 情報を取得して_dataに格納できるようにする
	void ChangeStage(int no, std::string str) override;	// 操作ステージの切替
	void Rotate(int no) override;
private:
	KEY_ARRAY _keyData;							// コンテナに生でアクセスしたいときはdataをよく使う
	std::map<InputID, int> _inputTbl;			// 対応表
};