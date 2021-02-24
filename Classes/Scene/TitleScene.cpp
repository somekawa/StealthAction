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

	auto _Gbg = LayerGradient::create(Color4B::BLACK, Color4B::WHITE);
	this->addChild(_Gbg);

	auto map = TMXTiledMap::create("image/Environment/title.tmx");
	auto b = map->getOpacity();
	map->getLayer("layer2")->setGlobalZOrder(3);
	this->addChild(map);

	auto plSp = Sprite::create();
	std::string path_light = "image/PlayerAnimationAsset/player/Light/player_Light";
	// run
	lpAnimMng.addAnimationCache(path_light, "Run", 9, (float)0.08, ActorType::Player, false);
	lpAnimMng.InitAnimation(*plSp, ActorType::Player, "player_Light_Run");
	plSp->setName("plSp");
	plSp->setScale(3.0f);
	plSp->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 3));
	this->addChild(plSp);

	flipFlg_ = false;


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
		origin.y + visibleSize.height / 3 - (label->getContentSize().height * 2)));

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
		origin.y + visibleSize.height / 3 - (label->getContentSize().height * 2)));

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

	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto pl = this->getChildByName("plSp");
	if (!flipFlg_)
	{
		if (pl->getPositionX() < visibleSize.width - (pl->getContentSize().width * 2))
		{
			pl->setPositionX(pl->getPositionX() + 3);
		}
		else
		{
			flipFlg_ = true;
		}
	}
	else
	{
		if (pl->getPositionX() > 0 + (pl->getContentSize().width * 2))
		{
			pl->setPositionX(pl->getPositionX() - 3);
		}
		else
		{
			flipFlg_ = false;
		}
	}
	pl->runAction(FlipX::create(flipFlg_));
}

void TitleScene::ChangeScene()
{
	isChanged_ = true;
	Scene* scene = Game::createScene();
	Director::getInstance()->popToRootScene();
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, scene, Color3B::WHITE));
}
