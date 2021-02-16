#include "ResidualShadow.h"
#include "Shader/ResShadowShader.h"

USING_NS_CC;

void ResidualShadow::CreateResidualShadow(cocos2d::Sprite& sprite, Layer& layer, int num)
{
	shader_ = std::make_shared<ResShadowShader>();
	isVisible_ = false;
	// ï™êgê∂ê¨ÇµÇƒÇ®Ç≠
	for (int i = 0; i < num; ++i)
	{
		cocos2d::Rect rect = cocos2d::Rect({ 0, 0 }, { 100, 100 });
		cocos2d::Sprite* tmp = Sprite::create();
		tmp->setTextureRect(rect);
		tmp->setVisible(isVisible_); 
		tmp->setPosition(100, 100);
		shader_->SetShader(*tmp);
		layer.addChild(tmp);
		residualShadows_.push_back(tmp);
	}
	layer_ = &layer;
}

void ResidualShadow::Move(Vec2 pos, Vec2 move)
{
	for (int i = 0; i < residualShadows_.size(); ++i)
	{
		Vec2 offset = move *(i + 1)* 1.1f;
		residualShadows_[i]->setPosition(pos - offset);
	}
}

void ResidualShadow::Start(Sprite& sprite)
{
	isVisible_ = true;
	for (auto& res : residualShadows_)
	{
		res->setSpriteFrame(sprite.getSpriteFrame());
		res->setScale(sprite.getScale());
		res->setAnchorPoint(sprite.getAnchorPoint());
		res->setFlippedX(sprite.isFlippedX());
		shader_->SetShader(*res);
		res->setVisible(isVisible_);
	}
}

void ResidualShadow::End()
{
	isVisible_ = false;
	for (auto& res : residualShadows_)
	{
		res->setVisible(isVisible_);
	}
}
