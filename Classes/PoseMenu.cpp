#include "PoseMenu.h"
#include "anim/AnimMng.h"
#include "Map/GameMap.h"
#include "Scene/GameScene.h"
#include "Scene/TitleScene.h"

USING_NS_CC;

cocos2d::Scene* PoseMenu::CreatePoseMenu(Scene* scene)
{
	PoseMenu* pRet = new(std::nothrow) PoseMenu(scene);
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

PoseMenu::PoseMenu(Scene* scene)
{
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

	// �Q�[���V�[���\��
	gameScene->visit();
	// �Q�[���V�[���̕\���𔼕��Â�
	cocos2d::Rect rect = cocos2d::Rect(0, 0, size.width, size.height);
	auto bg = Sprite::create();
	bg->setTextureRect(rect);
	bg->setColor({ 0, 0, 0 });
	bg->setOpacity(200);
	bg->setPosition(size.width / 2, size.height / 2);
	bg->retain();
	bg->visit();
	tex->end();
	// ���͌n��
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	auto listener = cocos2d::EventListenerKeyboard::create();
	listener->onKeyPressed = [&](cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* keyEvent)
	{
		if (keyCode == EventKeyboard::KeyCode::KEY_F2)
		{
			Director::getInstance()->popScene();
		}
		if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_SHIFT)
		{
			lpAnimMng.AnimDataClear();
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