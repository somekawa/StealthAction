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
	ON_TOUCH,		// �������u��
	TOUCHING,		// �������ςȂ�
	ON_RELEASE,		// �������u��
	RELEASED		// (������)�����Ă���
};

struct OPRT_state
{
	OPRT_state();
	~OPRT_state();

	virtual OPRT_TYPE GetType(void) = 0;				// ���̓^�C�v���󂯎��
	virtual void update() = 0;							// �������z�֐��ɂ��Ă��Ƃ���update
	virtual const std::array<bool, static_cast<int>(BUTTON::MAX)> &GetNowData(void) = 0;
	virtual const std::array<bool, static_cast<int>(BUTTON::MAX)> &GetOldData(void) = 0;
};