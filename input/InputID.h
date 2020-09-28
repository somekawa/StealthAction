#pragma once

// ボタンの種類
enum class InputID {
	//UP,
	DOWN,
	LEFT,
	RIGHT,
	//BTN1,
	//BTN2,
	MAX
};

static InputID begin(InputID) { return InputID::DOWN; };
static InputID end(InputID) { return InputID::MAX; };
//InputID operator++(InputID& state) { return state = static_cast<InputID>(static_cast<int>(state) + 1); };
static InputID operator++(InputID& state) { return (state = InputID(std::underlying_type<InputID>::type(state) + 1)); };
//underlying_type = 基底の型を調べてくれる命令
//enum classは何も書かなければint型になるから今回はintが中に入っていることになる

// ポインタ型のものが必要になるから
static InputID operator* (InputID state) { return (state); };

// 実体を1つだけにしないとエラーがでる
// それぞれの場所で作られることがダメ→staticでアドレスを共通にしよう