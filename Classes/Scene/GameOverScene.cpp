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

	auto _Gbg = LayerGradient::create(Color4B::BLACK, Color4B::WHITE);
	this->addChild(_Gbg);

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	auto listener = cocos2d::EventListenerKeyboard::create();
	listener->onKeyPressed = [this](cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event * keyEvent)
	{
		if (keyCode == EventKeyboard::KeyCode::KEY_SPACE)
		{
			if (!isChanged_)
			{
				this->ChangeScene();
			}
		}
	};
	auto label = Label::createWithTTF("GameOver...", "fonts/PixelMplus12-Regular.ttf", 48);
	label->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height - (label->getContentSize().height * 2)));
	label->setColor(Color3B::RED);
	this->addChild(label, 0);

	auto label2 = Label::createWithTTF("Please Press SPACE Key", "fonts/PixelMplus12-Regular.ttf", 48);
	label2->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height / 3 - (label->getContentSize().height * 2)));
	label2->setColor(Color3B::BLACK);
	this->addChild(label2, 0);
#else
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [this](cocos2d::Touch * touch, cocos2d::Event * event)
	{
		this->ChangeScene();
		return true;
	};
	auto label = Label::createWithTTF("GameOver...", "fonts/PixelMplus12-Regular.ttf", 48);
	label->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height - (label->getContentSize().height * 2)));
	label->setColor(Color3B::RED);
	this->addChild(label, 0);

	auto label2 = Label::createWithTTF("Tap To Title", "fonts/PixelMplus12-Regular.ttf", 48);
	label2->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height / 3 - (label->getContentSize().height * 2)));
	label2->setColor(Color3B::BLACK);
	this->addChild(label2, 0);

#endif // (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
	this->setName("GameOverScene");

	// メッセージの点滅
	auto actionBlink = Blink::create(7, 5);				// 7秒で5回点滅
	auto repeat = Repeat::create(actionBlink, -1);
	label2->runAction(repeat);

	// 死亡時のキャラクター
	auto deathChar = Sprite::create("image/gameover_char.png");
	deathChar->setPosition(visibleSize / 2);
	deathChar->setScale(3.0f);
	this->addChild(deathChar);

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
