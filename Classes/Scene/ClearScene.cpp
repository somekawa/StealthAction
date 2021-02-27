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
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
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
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	isChanged_ = false;

	auto _Gbg = LayerGradient::create(Color4B::BLACK, Color4B::WHITE);
	this->addChild(_Gbg);

	auto sprite = Sprite::create("image/clear.jpg");
	sprite->setAnchorPoint({ 0, 0 });
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
	auto label = Label::createWithTTF("Game Clear", "fonts/PixelMplus12-Regular.ttf", 48);
	label->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height - (label->getContentSize().height * 2)));
	label->setColor(Color3B::WHITE);
	this->addChild(label, 0);

	auto label2 = Label::createWithTTF("Please Press SPACE Key", "fonts/PixelMplus12-Regular.ttf", 48);
	label2->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height / 3 - (label->getContentSize().height * 2)));
	label2->setColor(Color3B::WHITE);
	this->addChild(label2, 0);
#else
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [this](cocos2d::Touch* touch, cocos2d::Event* event)
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

	// ���b�Z�[�W�̓_��
	auto actionBlink = Blink::create(7, 5);				// 7�b��5��_��
	auto repeat = Repeat::create(actionBlink, -1);
	label2->runAction(repeat);

	
	this->scheduleUpdate();
	return true;
}

void ClearScene::update(float delta)
{
	auto director = Director::getInstance();
	if (director->getRunningScene()->getName() != "GameOverScene")
	{
		return;
	}

	// �g�����W�V��������update�X�V��h��
	if (isChanged_)
	{
		return;
	}
}

void ClearScene::ChangeScene()
{
	lpAnimMng.AnimDataClear();		// ���̍폜���@�͂��ƂŕύX�����邩��
	isChanged_ = true;
	Scene* scene = TitleScene::CreateTitleScene();
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, scene, Color3B::WHITE));
}