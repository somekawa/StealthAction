#pragma once
#include <array>

#include "InputFlag.h"

// ｷｰﾎﾞｰﾄﾞのﾎﾞﾀﾝ
using KeyArray = std::array<char, 256>;

class KeyInput
{
public:
	KeyInput();
	~KeyInput();

	// 更新
	void UpDate(void);
	// 入力情報の取得
	const KeyArray& GetInput(InputFlag flg)const;

private:
	KeyArray keyNow_;
	KeyArray keyOld_;
};

