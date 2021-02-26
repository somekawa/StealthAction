#include "anim/AnimMng.h"
#include "Map/GameMap.h"
#include "Map/MapMenu.h"
#include "Guide.h"
#include "Scene/GameScene.h"
#include "Scene/TitleScene.h"
#include "SoundMng.h"
#include "PoseMenu.h"

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

	ButtonDraw();

	// 入力系統
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	auto listener = cocos2d::EventListenerKeyboard::create();
	auto a = Director::getInstance()->getRunningScene();
	listener->onKeyPressed = [&](cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* keyEvent)
	{
		//マップ表示
		if (keyCode == EventKeyboard::KeyCode::KEY_M)
		{
			Director::getInstance()->pushScene(MapMenu::CreateMapMenu(gameMap));
		}
		//操作説明表示
		if (keyCode == EventKeyboard::KeyCode::KEY_G)
		{
			Director::getInstance()->pushScene(Guide::CreateGuide());
		}

		//ゲームに戻る
		if (keyCode == EventKeyboard::KeyCode::KEY_F2)
		{
			Director::getInstance()->popScene();
		}
		//ゲームを終了しタイトルに戻る
		if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_SHIFT)
		{
			lpAnimMng.AnimDataClear();
			lpSoundMng.SetPauseAll(true);
			Director::getInstance()->popToSceneStackLevel(1);
			Director::getInstance()->replaceScene(TitleScene::CreateTitleScene());
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

void PoseMenu::ButtonDraw(void)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
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

	auto button = [=](Vec2 pos,std::string txt)
	{
		auto sprite = Sprite::create("image/button.png");
		auto sprite_size = sprite->getContentSize();
		auto label = Label::createWithTTF(txt, "fonts/PixelMplus12-Regular.ttf", 40);
		label->setTextColor(Color4B::BLACK);
		label->setPosition(pos);

		// add the label as a child to this layer
		this->addChild(label, 0);
		sprite->retain();
		sprite->setScaleX(0.2);
		sprite->setScaleY(0.1);
		sprite->setPosition(pos);
		sprite->visit();
	};
	button(Vec2(size.width / 2, (size.height / 5) * 4), "continue"	);
	button(Vec2(size.width / 2, (size.height / 5) * 3), "view Map"	);
	button(Vec2(size.width / 2, (size.height / 5) * 2), "view Guide");
	button(Vec2(size.width / 2, (size.height / 5)	 ), "Game Exit");

	tex->end();
}
