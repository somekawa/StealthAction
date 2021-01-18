#pragma once

// 方向変数
enum class Direction
{
	Right,
	Left,
	Max
};

// マップ方向変数
enum class MapDirection
{								  //angle - 1
	E_UP,	// 東の上	右側の上	  0～ 44
	N_RIGHT,// 北の右	上側の右	 45～ 89
	N_LEFT,	// 北の左	上側の左	 90～134
	W_UP,	// 西の上	左側の上	135～179
	W_DOWN,	// 西の下	左側の下	180～224
	S_LEFT,	// 南の左	下側の左	225～269
	S_RIGHT,// 南の右	下側の右	270～314
	E_Down,	// 東の下	右側の下	315～359
	Max
};
//0,3 : 1, 6 :  2,5 : 4,7

static Direction operator+(Direction& dir, int i)
{
	return (Direction)((int)dir + i);
}