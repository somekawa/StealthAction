// 担当場所
#include "SoundMng.h"
#include "TitleScene.h"
#include "GameScene.h"

USING_NS_CC;

TitleScene::TitleScene()
{
	lpSoundMng.AddSound("TitleBGM", "BGM/mizunishizumuorgan.mp3", SoundType::BGM);
}

TitleScene::~TitleScene()
{
}

Scene* TitleScene::CreateTitleScene()
{
	TitleScene* pRet = new(std::nothrow) TitleScene();
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

bool TitleScene::init()
{
	if (!Scene::init())
	{
		return false;
	}

	lpSoundMng.PlayBySoundName("TitleBGM");
	const auto visibleSize = Director::getInstance()->getVisibleSize();
	const Vec2 origin = Director::getInstance()->getVisibleOrigin();

	isChanged_ = false;

	const auto _Gbg = LayerGradient::create(Color4B::BLACK, Color4B::WHITE);
	this->addChild(_Gbg);

	const auto map = TMXTiledMap::create("image/Environment/title.tmx");
	map->getLayer("layer2")->setGlobalZOrder(3);
	this->addChild(map);

	const auto plSp = Sprite::create();
	const std::string path_light = "image/PlayerAnimationAsset/player/Light/player_Light";
	// run
	lpAnimMng.addAnimationCache(path_light, "Run", 9, 0.08f, ActorType::Player, false);
	lpAnimMng.InitAnimation(*plSp, ActorType::Player, "player_Light_Run");
	plSp->setName("plSp");
	plSp->setScale(3.0f);
	plSp->setPosition(Vec2(visibleSize.width / 2.0f, visibleSize.height / 3.0f));
	this->addChild(plSp);

	flipFlg_ = false;

	// タイトルロゴ
	const auto titleLogo = Sprite::create("image/title_logo.png");
	titleLogo->setPosition(Vec2(visibleSize.width / 3.0f - 50.0f, visibleSize.height / 2.0f + 140.0f));
	this->addChild(titleLogo);


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
	const auto label = Label::createWithTTF("Please Press SPACE Key ", "fonts/PixelMplus12-Regular.ttf", 48);
	label->setPosition(Vec2(origin.x + visibleSize.width / 2.0f,
		origin.y + visibleSize.height / 3.0f - (label->getContentSize().height * 2.0f)));

	this->addChild(label, 0);
#else
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [this](cocos2d::Touch* touch, cocos2d::Event* event)
	{
		this->ChangeScene();
		return true;
	};
	auto label = Label::createWithTTF("Tap To Start ", "fonts/PixelMplus12-Regular.ttf", 48);
	label->setPosition(Vec2(origin.x + visibleSize.width / 2.0f,
		origin.y + visibleSize.height / 3.0f - (label->getContentSize().height * 2.0f)));

	this->addChild(label, 0);
#endif // (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	// メッセージの点滅
	const auto actionBlink = Blink::create(7, 5);				// 7秒で5回点滅
	const auto repeat = Repeat::create(actionBlink, -1);
	label->runAction(repeat);

	runSpeed_ = 3.0f;

	this->setName("TitleScene");
	this->scheduleUpdate();
	return true;
}

void TitleScene::update(float delta)
{
	const auto director = Director::getInstance();
	if (director->getRunningScene()->getName() != "TitleScene")
	{
		return;
	}

	const auto visibleSize = Director::getInstance()->getVisibleSize();
	const auto pl = this->getChildByName("plSp");
	if (!flipFlg_)
	{
		if (pl->getPositionX() < visibleSize.width - (pl->getContentSize().width * 2.0f))
		{
			pl->setPositionX(pl->getPositionX() + runSpeed_);
		}
		else
		{
			flipFlg_ = true;
		}
	}
	else
	{
		if (pl->getPositionX() > 0 + (pl->getContentSize().width * 2.0f))
		{
			pl->setPositionX(pl->getPositionX() - runSpeed_);
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
	lpSoundMng.SetPauseAll(true);
	Scene* scene = Game::createScene();
	Director::getInstance()->popToRootScene();
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, scene, Color3B::WHITE));
}