#include "Guide.h"
#include "Scene/GameScene.h"

USING_NS_CC;

cocos2d::Scene* Guide::CreateGuide()
{
	Guide* pRet = new(std::nothrow) Guide();
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

Guide::Guide()
{
	auto director = Director::getInstance();
	auto size = director->getVisibleSize();
	auto tex = RenderTexture::create(size.width, size.height);
	tex->setPosition(size.width / 2, size.height / 2);
	this->addChild(tex);

	auto gameScene = director->getRunningScene();
	tex->begin();

	// ゲームシーン表示
	gameScene->visit();
	spriteDraw(size);
	tex->end();

	// 入力系統
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	auto listener = cocos2d::EventListenerKeyboard::create();
	auto a = Director::getInstance()->getRunningScene();
	listener->onKeyPressed = [&](cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* keyEvent)
	{
		if (keyCode == EventKeyboard::KeyCode::KEY_G)
		{
			Director::getInstance()->popScene();
		}
	};
#else
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [this](cocos2d::Touch* touch, cocos2d::Event* event)
	{
		Director::getInstance()->popScene();
		return true;
	};
#endif // (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

Guide::~Guide()
{
}

void Guide::spriteDraw(Size size)
{
	auto sprite = [&](std::string path, Vec2 pos,float scl = 1.0f,bool flip = false)
	{
		auto sprite = Sprite::create(path);
		auto sprite_size = sprite->getContentSize();
		sprite->retain();
		sprite->setScale(scl);
		sprite->setPosition(pos);
		sprite->setFlippedX(flip);
		sprite->visit();
	};
	sprite("image/mapBg.png", Vec2(size.width / 2, size.height / 2));
	sprite("image/playerGuide.png", Vec2(size.width / 2, size.height - (size.height / 10) - 144 / 2), 0.5f);
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	auto scl = 3.0f;
	sprite("image/PlayerAsset/Light/idle.png", Vec2(size.width / 6 + 20, (size.height / 10) + 18 / 2 * scl), scl);
	sprite("image/PlayerAsset/Dark/idle.png", Vec2(size.width - (size.width / 6 + 20), (size.height / 10) + 18 / 2 * scl), scl, true);
	sprite("image/androidGuide.png", Vec2((size.width / 2), (size.height / 2)-25), 0.5f);
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

#endif //CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
}

void Guide::textDraw(cocos2d::Size size)
{
}
