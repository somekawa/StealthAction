#pragma once
#include <cocos2d.h>

class ResShadowShader
{
public:
	ResShadowShader();
	~ResShadowShader();
	// ノードにシェーダを設定
	void SetShader(cocos2d::Node& node);

private:

	cocos2d::backend::Program* program;
};

#pragma once
#include <cocos2d.h>
class ResShadowShader
{
public:
	ResShadowShader();
	~ResShadowShader();
	// ノードにシェーダを設定
	void SetShader(cocos2d::Node& node);

private:

	cocos2d::backend::Program* program;
};

