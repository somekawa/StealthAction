#pragma once

// プレイヤーのアニメーションタイプ
enum class AnimationType
{
	// アニメーションなし
	Non,
	// 待機
	Idle,
	// 歩く
	Walk,
	// 走る
	Run,
	// 攻撃
	Attack,
	// 死ぬ
	Death,
	// 落下
	Fall,
	// ジャンプ
	Jump,
	Max
};