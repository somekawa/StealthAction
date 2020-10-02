#pragma once

#include "cocos2d.h"
#include "input/DIR_ID.h"

enum class OPRT_TYPE {
	KEY,
	TOUCH,
	MAX
};

enum class TRG_KEY {
	NON,
	NOW,
	OLD,
	INPUT,
};

enum class BUTTON {
	UP,
	DOWN,
	RIGHT,
	LEFT,
	ATTACK,
	MAX
};

enum class TOUCH_TIMMING {
	ON_TOUCH,		// 押した瞬間
	TOUCHING,		// 押しっぱなし
	ON_RELEASE,		// 離した瞬間
	RELEASED		// (ずっと)離している
};

struct OPRT_state
{
	OPRT_state();
	~OPRT_state();

	virtual OPRT_TYPE GetType(void) = 0;				// 入力タイプを受け取る
	virtual void update() = 0;							// 純粋仮想関数にしてたときのupdate
	virtual const std::array<bool, static_cast<int>(BUTTON::MAX)> &GetNowData(void) = 0;
	virtual const std::array<bool, static_cast<int>(BUTTON::MAX)> &GetOldData(void) = 0;
};