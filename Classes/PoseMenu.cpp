#include "PoseMenu.h"
#include "Map/GameMap.h"
#include "Scene/GameScene.h"

USING_NS_CC;

cocos2d::Scene* PoseMenu::CreatePoseMenu(GameMap& gameMap)
{
	PoseMenu* pRet = new(std::nothrow) PoseMenu(gameMap);
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

PoseMenu::PoseMenu(GameMap& gameMap)
{
	auto& mapGen = gameMap.GetMapGenerator();
	const auto& nowID = gameMap.GetNowID();
	auto director = Director::getInstance();
	auto size = director->getVisibleSize();
	auto tex = RenderTexture::create(size.width, size.height);
	tex->setPosition(size.width / 2, size.height / 2);
	this->addChild(tex);

	auto gameScene = director->getRunningScene();
	if (gameScene->getName() != "GameScene")
	{
		Director::getInstance()->popScene();
	}
	tex->begin();

	// ゲームシーン表示
	gameScene->visit();
	// ゲームシーンの表示を半分暗く
	cocos2d::Rect rect = cocos2d::Rect(0, 0, size.width, size.height);
	auto bg = Sprite::create();
	bg->setTextureRect(rect);
	bg->setColor({ 0, 0, 0 });
	bg->setOpacity(200);
	bg->setPosition(size.width / 2, size.height / 2);
	bg->retain();
	bg->visit();
	tex->end();
	// 入力系統
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	auto listener = cocos2d::EventListenerKeyboard::create();
	listener->onKeyPressed = [this](cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* keyEvent)
	{
		if (keyCode == EventKeyboard::KeyCode::KEY_F2)
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

PoseMenu::~PoseMenu()
{
}
