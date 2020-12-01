#pragma once

enum class DIR {
	NON,
	UP,
	DOWN,
	RIGHT,
	LEFT,
	MAX
};

DIR begin(DIR);
DIR end(DIR);
DIR operator* (DIR key);
DIR operator  ++(DIR& key);			// ‘O‰‰ŽZ
