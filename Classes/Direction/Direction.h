#pragma once

// 方向変数
enum class Direction
{
	Right,
	Left,
	Up,
	Down,
	Max
};

// マップ方向変数
enum class MapDirection
{
	W_UP,	// 西の上	左側の上
	W_DOWN,	// 西の下	左側の下
	N_LEFT,	// 北の左	上側の左
	S_LEFT,	// 南の左	下側の左
	N_RIGHT,// 北の右	上側の右
	S_RIGHT,// 南の右	下側の右
	E_UP,	// 東の上	右側の上
	E_Down,	// 東の下	右側の下
	Max
};
