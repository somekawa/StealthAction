#pragma once
#include <array>

#include "InputFlag.h"

// ·°ÎŞ°ÄŞ‚ÌÎŞÀİ
using KeyArray = std::array<char, 256>;

class KeyInput
{
public:
	KeyInput();
	~KeyInput();

	// XV
	void UpDate(void);
	// “ü—Íî•ñ‚Ìæ“¾
	const KeyArray& GetInput(InputFlag flg)const;

private:
	KeyArray keyNow_;
	KeyArray keyOld_;
};

