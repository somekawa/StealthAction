#pragma once

enum class DIR {
	NON,
	UP,
	DOWN,
	RIGHT,
	LEFT,
	MAX
};

enum class ACTION {
	NON,
	IDLE,
	JUMP,
	JUMPING,
	RUN_L,
	RUN_R,
	FALLING,
	ATTACK,
	MAX
};

DIR begin(DIR);
DIR end(DIR);
DIR operator* (DIR key);
DIR operator  ++(DIR& key);			// �O���Z

ACTION begin(ACTION);
ACTION end(ACTION);
ACTION operator* (ACTION key);
ACTION operator  ++(ACTION& key);   // �O���Z
