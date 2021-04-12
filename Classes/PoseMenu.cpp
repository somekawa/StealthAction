#include "anim/AnimMng.h"
#include "Map/GameMap.h"
#include "Map/MapMenu.h"
#include "Guide.h"
#include "Scene/GameScene.h"
#include "Scene/TitleScene.h"
#include "SoundMng.h"
#include "input/OPRT_touch.h"
#include "PoseMenu.h"

USING_NS_CC;

PoseMenu::PoseMenu(GameMap& gameMap):gameMap_(gameMap)
{
	oprtState_ = new OPRT_touch((Sprite*)this);
	ButtonDraw();

	// 入力系統
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	auto listener = cocos2d::EventListenerKeyboard::create();
	listener->onKeyPressed = [&](cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* keyEvent)
	{
		//マップ表示
		if (keyCode == EventKeyboard::KeyCode::KEY_M)
		{
			Director::getInstance()->pushScene(MapMenu::CreateMapMenu(gameMap_));
		}
		//操作説明表示
		if (keyCode == EventKeyboard::KeyCode::KEY_G)
		{
			Director::getInstance()->pushScene(Guide::CreateGuide());
		}

		//ゲームに戻る
		if (keyCode == EventKeyboard::KeyCode::KEY_TAB)
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
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
#endif // (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	this->setName("PoseMenu");
	this->scheduleUpdate();
}

PoseMenu::~PoseMenu()
{
	delete oprtState_;
}

cocos2d::Scene* PoseMenu::CreatePoseMenu(GameMap& gameMap)
{
	PoseMenu* pRet = new(std::nothrow) PoseMenu(gameMap);
	if (pRet && pRet->init())
	{
		pRet->autorelease();
	}
	else
	{
		delete pRet;
		pRet = nullptr;
	}
	return pRet;
}

void PoseMenu::ButtonDraw(void)
{
	const auto visibleSize = Director::getInstance()->getVisibleSize();
	const auto director = Director::getInstance();
	const auto size = director->getVisibleSize();
	const auto tex = RenderTexture::create(static_cast<int>(size.width), static_cast<int>(size.height));
	tex->setPosition(size.width / 2, size.height / 2);
	this->addChild(tex);

	const auto gameScene = director->getRunningScene();
	if (gameScene->getName() != "GameScene")
	{
		Director::getInstance()->popScene();
	}
	tex->begin();

	// ゲームシーン表示
	gameScene->visit();
	// ゲームシーンの表示を半分暗く
	cocos2d::Rect rect = cocos2d::Rect(0, 0, size.width, size.height);
	const auto bg = Sprite::create();
	bg->setTextureRect(rect);
	bg->setColor({ 0, 0, 0 });
	bg->setOpacity(200);
	bg->setPosition(size.width / 2, size.height / 2);
	bg->retain();
	bg->visit();

	auto button = [=](Vec2 pos,std::string path,std::string name)
	{
		const auto sprite = MenuItemImage::create(
			"image/button.png",
			"image/button.png");
		const auto sprite2 = MenuItemImage::create(path,path);
		sprite->retain();
		sprite->setName(name);
		sprite->setScaleX(0.2f);
		sprite->setScaleY(0.1f);
		sprite->setPosition(pos);
		sprite->visit();
		sprite2->retain();
		sprite2->setName(name);
		sprite2->setScale(0.5f);
		sprite2->setPosition(pos);
		sprite2->visit();
		this->addChild(sprite);
		this->addChild(sprite2, 1);
	};
	button(Vec2(size.width / 2.0f, (size.height / 5.0f) * 4), "image/continue.png"	  , "continueBtn");
	button(Vec2(size.width / 2.0f, (size.height / 5.0f) * 3), "image/view Map.png"	  , "mapBtn");
	button(Vec2(size.width / 2.0f, (size.height / 5.0f) * 2), "image/view Guide.png"  , "guideBtn");
	button(Vec2(size.width / 2.0f, (size.height / 5.0f)	 )  , "image/Game Exit.png"   , "exitBtn");

	tex->end();
}

void PoseMenu::update(float delta)
{
	if (Director::getInstance()->getRunningScene()->getName() != "PoseMenu")
	{
		return;
	}

	const auto label1 = this->getChildByName("continueBtn");
	if (label1 != nullptr && ((MenuItemImage*)label1)->isSelected())
	{
		((MenuItemImage*)label1)->unselected();
		Director::getInstance()->popScene();
	}

	const auto label2 = this->getChildByName("mapBtn");
	if (label2 != nullptr && ((MenuItemImage*)label2)->isSelected())
	{
		((MenuItemImage*)label2)->unselected();
		Director::getInstance()->pushScene(MapMenu::CreateMapMenu(gameMap_));
	}

	const auto label3 = this->getChildByName("guideBtn");
	if (label3 != nullptr && ((MenuItemImage*)label3)->isSelected())
	{
		((MenuItemImage*)label3)->unselected();
		Director::getInstance()->pushScene(Guide::CreateGuide());
	}

	const auto label4 = this->getChildByName("exitBtn");
	if (label4 != nullptr && ((MenuItemImage*)label4)->isSelected())
	{
		((MenuItemImage*)label4)->unselected();
		lpAnimMng.AnimDataClear();
		lpSoundMng.SetPauseAll(true);
		Director::getInstance()->popToSceneStackLevel(1);
		Director::getInstance()->replaceScene(TitleScene::CreateTitleScene());
	}
}