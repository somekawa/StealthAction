#pragma once

class Enemy;

// 実行状況
enum State
{
	// 実行中
	Run,
	// 失敗
	Failed,
	// 成功
	Complete
};

// 行動処理実行クラス
class ActionBase
{
public:

	virtual State Run(Enemy* enemy) = 0;
};

