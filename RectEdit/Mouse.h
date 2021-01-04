#pragma once
#include <type_traits>
#include <array>
#include <map>

#include "InputFlag.h"
#include "Geometory.h"

enum class ClickDir
{
	Right,
	Left,
	Max
};

static ClickDir begin(ClickDir)
{
	return ClickDir::Right;
}
static ClickDir end(ClickDir)
{
	return ClickDir::Max;
}
static ClickDir operator++(ClickDir& cDir)
{
	return (cDir =
		ClickDir(std::underlying_type<InputFlag>::type(cDir) + 1));
}
static ClickDir& operator*(ClickDir& cDir)
{
	return cDir;
}


using MouseFlag = std::array<std::map<ClickDir, bool>, static_cast<int>(InputFlag::Max)>;

class Mouse
{
public:
	Mouse();
	~Mouse();

	void UpDate(void);
	const Vector2I& GetPos(void)const;
	const Vector2F& GetPosF(void)const;
	const std::map<ClickDir, bool>& GetMouseFlag(InputFlag flg)const;

	void SetUp(void);

private:
	Vector2I pos_;
	// マウスポジションをfloatに変換する用
	Vector2F posF_;

	MouseFlag mouseData_;

	std::map<ClickDir,int> clickTable_;
};

