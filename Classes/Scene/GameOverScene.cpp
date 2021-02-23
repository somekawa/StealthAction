#include "GameOverScene.h"
#include "TitleScene.h"
#include "anim/AnimMng.h"

USING_NS_CC;

Scene* GameOverScene::CreateGameOverScene()
{
	GameOverScene* pRet = new(std::nothrow) GameOverScene();
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

GameOverScene::GameOverScene()
{
}

bool GameOverScene::init()
{
	if (!Scene::init())
	{
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	isChanged_ = false;

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	auto listener = cocos2d::EventListenerKeyboard::create();
	listener->onKeyPressed = [this](cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event * keyEvent)
	{
		if (!isChanged_)
		{
			this->ChangeScene();
		}
	};
	auto label = Label::createWithTTF("This is GameOverScene ", "fonts/PixelMplus12-Regular.ttf", 48);
	label->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height / 3 - label->getContentSize().height));

	// add the label as a child to this layer
	this->addChild(label, 0);
#else
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [this](cocos2d::Touch * touch, cocos2d::Event * event)
	{
		this->ChangeScene();
		return true;
	};
	auto label = Label::createWithTTF("Tap To Start ", "fonts/PixelMplus12-Regular.ttf", 48);
	label->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height / 3 - label->getContentSize().height));

	// add the label as a child to this layer
	this->addChild(label, 0);
#endif // (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
	this->setName("GameOverScene");
	this->scheduleUpdate();
	return true;
}

void GameOverScene::update(float delta)
{
	auto director = Director::getInstance();
	if (director->getRunningScene()->getName() != "GameOverScene")
	{
		return;
	}

	// トランジション中のupdate更新を防ぐ
	if (isChanged_)
	{
		return;
	}
}

void GameOverScene::ChangeScene()
{
	lpAnimMng.AnimDataClear();		// この削除方法はあとで変更かけるかも
	isChanged_ = true;
	Scene* scene = TitleScene::CreateTitleScene();
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, scene, Color3B::WHITE));
}
