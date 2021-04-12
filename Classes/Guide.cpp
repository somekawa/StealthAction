#include "Scene/GameScene.h"
#include "input/OPRT_touch.h"
#include "Guide.h"

USING_NS_CC;

Guide::Guide()
{
	oprtState_ = new OPRT_touch((Sprite*)this);

	const auto director = Director::getInstance();
	const auto size = director->getVisibleSize();
	const auto tex = RenderTexture::create(static_cast<int>(size.width), static_cast<int>(size.height));
	tex->setPosition(size.width / 2, size.height / 2);
	this->addChild(tex);

	const auto gameScene = director->getRunningScene();
	tex->begin();

	// ゲームシーン表示
	gameScene->visit();
	spriteDraw(size);
	tex->end();

	// 入力系統
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	auto listener = cocos2d::EventListenerKeyboard::create();
	auto a = Director::getInstance()->getRunningScene();
	listener->onKeyPressed = [&](cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* keyEvent)
	{
		if (keyCode == EventKeyboard::KeyCode::KEY_G)
		{
			Director::getInstance()->popScene();
		}
	};
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
#endif // (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

	this->setName("Guide");
	this->scheduleUpdate();
}

Guide::~Guide()
{
	delete oprtState_;
}

cocos2d::Scene* Guide::CreateGuide()
{
	Guide* pRet = new(std::nothrow) Guide();
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

void Guide::spriteDraw(Size size)
{
	const auto sprite = [&](std::string path, Vec2 pos,float scl = 1.0f,bool flip = false)
	{
		const auto sprite = Sprite::create(path);
		const auto sprite_size = sprite->getContentSize();
		sprite->retain();
		sprite->setScale(scl);
		sprite->setPosition(pos);
		sprite->setFlippedX(flip);
		sprite->visit();
	};
	sprite("image/mapBg.png", Vec2(size.width / 2, size.height / 2));
	sprite("image/playerGuide.png", Vec2(size.width / 2, size.height - (size.height / 10) - 144 / 2), 0.5f);

	const auto cancelBtn = MenuItemImage::create(
		"image/cancelBtn.png",
		"image/cancelBtn_select.png");
	cancelBtn->setPosition(Vec2(size.width - (cancelBtn->getContentSize().width * 0.25f) / 2.0f - 10.0f, size.height - (cancelBtn->getContentSize().height * 0.25f) / 2.0f - 10.0f));
	cancelBtn->setScale(0.25f);
	cancelBtn->setName("cancelBtn");			// MapMenuと同じ名前を設定する
	this->addChild(cancelBtn);

	auto scl = 3.0f;
	sprite("image/PlayerAsset/Light/idle.png", Vec2(size.width / 6 + 20, (size.height / 10) + 18 / 2 * scl), scl);
	sprite("image/PlayerAsset/Dark/idle.png", Vec2(size.width - (size.width / 6 + 20), (size.height / 10) + 18 / 2 * scl), scl, true);
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	scl = 0.2f;
	sprite("image/SkillGuide.png", Vec2(size.width / 2, (size.height / 2)+144*scl), scl);
	sprite("image/DashGuide.png", Vec2(size.width / 2, (size.height / 2)), scl);
	sprite("image/moveGuide1.png", Vec2(size.width / 2, (size.height / 2)-144 * scl), scl);
	sprite("image/AttackGuide.png", Vec2(size.width / 2, (size.height / 2)-(144 * scl)*2), scl);
	sprite("image/JumpGuide.png", Vec2(size.width / 2, (size.height / 2)-(144 * scl)*3), scl);
	sprite("image/TransformGuide.png", Vec2(size.width / 2, (size.height / 2)-(144 * scl)*4), scl);

#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	sprite("image/androidGuide.png", Vec2((size.width / 2), (size.height / 2) - 40), 0.5f);

#endif //CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
}

void Guide::textDraw(cocos2d::Size size)
{
}

void Guide::update(float delta)
{
	if (Director::getInstance()->getRunningScene()->getName() != "Guide")
	{
		return;
	}

	const auto label1 = this->getChildByName("cancelBtn");
	if (label1 != nullptr && (((MenuItemImage*)label1)->isSelected()))
	{
		((MenuItemImage*)label1)->unselected();
		Director::getInstance()->popScene();
	}
}