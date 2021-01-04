#include <DxLib.h>
#include "Mouse.h"

Mouse::Mouse()
{

}

Mouse::~Mouse()
{
}

void Mouse::UpDate(void)
{
	// OldÇ…NowÇÃ√ﬁ∞¿Çäiî[
	for (auto c : ClickDir())
	{
		mouseData_[static_cast<int>(InputFlag::Old)][c]
			= mouseData_[static_cast<int>(InputFlag::Now)][c];
	}
	
	for (auto c : ClickDir())
	{
		if (GetMouseInput() & clickTable_[c])
		{
			mouseData_[static_cast<int>(InputFlag::Now)][c] = true;
		}
		else
		{
			mouseData_[static_cast<int>(InputFlag::Now)][c] = false;
		}
	}

	// œ≥ΩŒﬂºﬁºÆ›ÇÃçXêV
	GetMousePoint(&pos_.x, &pos_.y);
	posF_.x = pos_.x;
	posF_.y = pos_.y;
}

const Vector2I& Mouse::GetPos(void)const
{
	return pos_;
}

const Vector2F& Mouse::GetPosF(void) const
{
	return posF_;
}

const std::map<ClickDir, bool>& Mouse::GetMouseFlag(InputFlag flg)const
{
	return (flg == InputFlag::Now ? mouseData_[static_cast<int>(InputFlag::Now)] : mouseData_[static_cast<int>(InputFlag::Old)]);
}

void Mouse::SetUp(void)
{

	clickTable_ = {
		{ClickDir::Right,MOUSE_INPUT_RIGHT },
		{ClickDir::Left,MOUSE_INPUT_LEFT}
	};
}
