#pragma once
#include <cocos2d.h>

class PlayerShader
{
public:
	PlayerShader();
	~PlayerShader();
	// ノードにシェーダを設定
	void SetShader(cocos2d::Node& node, cocos2d::Vec3 color);

private:
	cocos2d::backend::Program* program_;
};