#include "ClearScene.h"
#include "TitleScene.h"
#include "anim/AnimMng.h"

USING_NS_CC;

Scene* ClearScene::CreateClearScene()
{
	ClearScene* pRet = new(std::nothrow) ClearScene();
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

ClearScene::ClearScene()
{
}

bool ClearScene::init()
{
	if (!Scene::init())
	{
		return false;
	}
	const auto visibleSize = Director::getInstance()->getVisibleSize();
	const Vec2 origin = Director::getInstance()->getVisibleOrigin();

	isChanged_ = false;

	const auto _Gbg = LayerGradient::create(Color4B::BLACK, Color4B::WHITE);
	this->addChild(_Gbg);

	const auto sprite = Sprite::create("image/clear.jpg");
	sprite->setAnchorPoint({ 0.0f, 0.0f });
	sprite->setContentSize(visibleSize);
	this->addChild(sprite);


#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	auto listener = cocos2d::EventListenerKeyboard::create();
	listener->onKeyPressed = [this](cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* keyEvent)
	{
		if (keyCode == EventKeyboard::KeyCode::KEY_SPACE)
		{
			if (!isChanged_)
			{
				this->ChangeScene();
			}
		}
	};
	const auto label = Label::createWithTTF("Game Clear", "fonts/PixelMplus12-Regular.ttf", 48);
	label->setPosition(Vec2(origin.x + visibleSize.width / 2.0f,
		origin.y + visibleSize.height - (label->getContentSize().height * 2.0f)));
	label->setColor(Color3B::WHITE);
	this->addChild(label, 0);

	const auto label2 = Label::createWithTTF("Please Press SPACE Key", "fonts/PixelMplus12-Regular.ttf", 48);
	label2->setPosition(Vec2(origin.x + visibleSize.width / 2.0f,
		origin.y + visibleSize.height / 3.0f - (label->getContentSize().height * 2.0f)));
	label2->setColor(Color3B::WHITE);
	this->addChild(label2, 0);
#else
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [this](cocos2d::Touch* touch, cocos2d::Event* event)
	{
		this->ChangeScene();
		return true;
	};
	const auto label = Label::createWithTTF("GameOver...", "fonts/PixelMplus12-Regular.ttf", 48);
	label->setPosition(Vec2(origin.x + visibleSize.width / 2.0f,
		origin.y + visibleSize.height - (label->getContentSize().height * 2.0f)));
	label->setColor(Color3B::RED);
	this->addChild(label, 0);

	const auto label2 = Label::createWithTTF("Tap To Title", "fonts/PixelMplus12-Regular.ttf", 48);
	label2->setPosition(Vec2(origin.x + visibleSize.width / 2.0f,
		origin.y + visibleSize.height / 3.0f - (label->getContentSize().height * 2.0f)));
	label2->setColor(Color3B::BLACK);
	this->addChild(label2, 0);

#endif // (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
	this->setName("GameOverScene");

	// メッセージの点滅
	const auto actionBlink = Blink::create(7, 5);				// 7秒で5回点滅
	const auto repeat = Repeat::create(actionBlink, -1);
	label2->runAction(repeat);

	this->scheduleUpdate();
	return true;
}

void ClearScene::update(float delta)
{
	if (Director::getInstance()->getRunningScene()->getName() != "GameOverScene")
	{
		return;
	}

	// トランジション中のupdate更新を防ぐ
	if (isChanged_)
	{
		return;
	}
}

void ClearScene::ChangeScene()
{
	lpAnimMng.AnimDataClear();		
	isChanged_ = true;
	Scene* scene = TitleScene::CreateTitleScene();
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, scene, Color3B::WHITE));
}