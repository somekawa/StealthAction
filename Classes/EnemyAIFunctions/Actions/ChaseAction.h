#pragma once
#include "module/ActModule.h"

// プレイヤーを追いかけるアクション
struct ChaseAction
{
	bool operator()(cocos2d::Sprite& sprite, ActModule& module);
};