#include "TitleScene.h"
#include "GameScene.h"

USING_NS_CC;

Scene* TitleScene::CreateTitleScene()
{
	TitleScene* pRet = new(std::nothrow) TitleScene();
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

TitleScene::TitleScene()
{
}

bool TitleScene::init()
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
	listener->onKeyPressed = [this](cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* keyEvent)
	{
		if (!isChanged_)
		{
			this->ChangeScene();
		}
	};
	auto label = Label::createWithTTF("To Start Press Any Key ", "fonts/PixelMplus12-Regular.ttf", 48);
	label->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height / 3 - label->getContentSize().height));

	// add the label as a child to this layer
	this->addChild(label, 0);
#else
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [this](cocos2d::Touch* touch, cocos2d::Event* event)
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
	this->setName("TitleScene");
	this->scheduleUpdate();
	return true;
}

void TitleScene::update(float delta)
{
	auto director = Director::getInstance();
	if (director->getRunningScene()->getName() != "TitleScene")
	{
		return;
	}
}

void TitleScene::ChangeScene()
{
	isChanged_ = true;
	Scene* scene = Game::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, scene, Color3B::WHITE));
}
