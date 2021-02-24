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

	// ƒQ[ƒ€ƒV[ƒ“•\Ž¦
	gameScene->visit();
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	auto sprite = Sprite::create("image/guide.png");
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	auto sprite = Sprite::create("image/android_guide.png");
#endif //CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	auto sprite_size = sprite->getContentSize();
	sprite->retain();
	sprite->setPosition(size.width / 2, size.height / 2);
	sprite->visit();
	tex->end();

	// “ü—ÍŒn“
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
