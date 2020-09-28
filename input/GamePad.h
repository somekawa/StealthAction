#pragma once
#include <map>
#include <string>
#include <vector>
#include "Controller.h"

struct GamePad :
	public Controller
{
	CONTROLLER_TYPE GetType(void) override;		// 自分の入力装置を返す
	bool Setup(void) override;					// 初期化(初期化成功でtrue)
	void Update(void)  override;				// 情報を取得して_dataに格納できるようにする
	void ChangeStage(int no,std::string str) override;	// 操作ステージの切替
	void Rotate(int no) override;
private:
	std::map<InputID, int> _inputTbl;	// 対応表
	//int PadInput;
	//int padNum;
	std::vector<int> padInpVec;
	std::vector<int> padVec;
};