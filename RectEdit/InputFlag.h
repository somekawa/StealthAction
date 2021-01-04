#pragma once

enum class InputFlag
{
	Now,
	Old,
	Max
};

static InputFlag begin(InputFlag)
{
	return InputFlag::Now;
}

static InputFlag end(InputFlag)
{
	return InputFlag::Max;
}

static InputFlag operator++(InputFlag& input)
{
	return (input =
		InputFlag(std::underlying_type<InputFlag>::type(input) + 1));
}

static InputFlag& operator*(InputFlag& input)
{
	return input;
}