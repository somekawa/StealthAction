#include "ResidualShadow.h"
#include "../Shader/ResShadowShader.h"

void ResidualShadow::CreateResidualShadow(cocos2d::Sprite& sprite, cocos2d::Layer& layer, int num)
{
	resShader_ = std::make_shared<ResShadowShader>();
	isVisible_ = false;
	// ï™êgê∂ê¨ÇµÇƒÇ®Ç≠
	for (int i = 0; i < num; ++i)
	{
		cocos2d::Rect rect = cocos2d::Rect({ 0, 0 }, { 100, 100 });
		cocos2d::Sprite* tmp = cocos2d::Sprite::create();
		tmp->setTextureRect(rect);
		tmp->setVisible(isVisible_); 
		tmp->setPosition(100, 100);
		resShader_->SetShader(*tmp);
		layer.addChild(tmp);
		residualShadows_.push_back(tmp);
	}
}

void ResidualShadow::Move(cocos2d::Vec2 pos, cocos2d::Vec2 move)
{
	for (unsigned int i = 0; i < residualShadows_.size(); ++i)
	{
		cocos2d::Vec2 offset = move *(static_cast<float>(i) + 1.0f)* 1.1f;
		residualShadows_[i]->setPosition(pos - offset);
	}
}

void ResidualShadow::Start(cocos2d::Sprite& sprite)
{
	isVisible_ = true;
	for (auto& res : residualShadows_)
	{
		res->setSpriteFrame(sprite.getSpriteFrame());
		res->setScale(sprite.getScale());
		res->setAnchorPoint(sprite.getAnchorPoint());
		res->setFlippedX(sprite.isFlippedX());
		resShader_->SetShader(*res);
		res->setVisible(isVisible_);
	}
}

void ResidualShadow::ResShadowEnd()
{
	isVisible_ = false;
	for (auto& res : residualShadows_)
	{
		res->setVisible(isVisible_);
	}
}