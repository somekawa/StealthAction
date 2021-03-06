#pragma once
#include <cocos2d.h>
#include <vector>
#include <memory>

class ResShadowShader;
class ResidualShadow
{
public:
	ResidualShadow() = default;
	~ResidualShadow() = default;
	void CreateResidualShadow(cocos2d::Sprite& sprite,cocos2d::Layer& layer, int num);
	void Move(cocos2d::Vec2 pos, cocos2d::Vec2 move);
	void Start(cocos2d::Sprite& sprite);
	void ResShadowEnd();
private:
	std::vector<cocos2d::Sprite*>residualShadows_;
	std::shared_ptr<ResShadowShader>resShader_;
	bool isVisible_;
};

