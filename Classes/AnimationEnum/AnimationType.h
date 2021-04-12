// 担当
#pragma once

// アニメーションタイプ
enum class AnimationType
{
	Non,			// アニメーションなし
	L_move,			// 左移動
	R_move,			// 右移動
	L_dir,			// 左向き
	R_dir,			// 右向き
	Attack1,		// 攻撃
	Attack2,		// 攻撃
	Attack3,		// 攻撃
	Fall,			// 落下
	Jump,			// ジャンプ
	Jumping,		// ジャンピング
	L_wallSlide,	// 左壁スライド
	R_wallSlide,	// 右壁スライド
	Max
};