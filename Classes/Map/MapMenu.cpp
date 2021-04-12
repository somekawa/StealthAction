// ボタン処理担当
#include "MapMenu.h"
#include "Generator/MapGenerator.h"
#include "Scene/GameScene.h"
#include "Generator/MST.h"
#include "input/OPRT_touch.h"
#include "GameMap.h"

USING_NS_CC;

MapMenu::MapMenu(GameMap& gameMap)
{
	oprtState_ = new OPRT_touch((Sprite*)this);

	auto& mapGen = gameMap.GetMapGenerator();
	const auto& nowID   = gameMap.GetNowID();
	const auto director = Director::getInstance();
	const auto size = director->getVisibleSize();
	tex = RenderTexture::create(static_cast<int>(size.width), static_cast<int>(size.height));
	tex->setPosition(size / 2.0f);
	this->addChild(tex);

	const auto gameScene = director->getRunningScene();

	// ×ボタン画像
	const auto canselBtn = MenuItemImage::create(
		"image/cancelBtn.png",
		"image/cancelBtn_select.png");
	canselBtn->setPosition((size - ((canselBtn->getContentSize() * 0.25f) / 2.0f)) - Size(10.0f,10.0f));
	canselBtn->setScale(0.25f);
	canselBtn->setName("cancelBtn");		
	this->addChild(canselBtn);
	tex->begin();

	// ゲームシーン表示
	gameScene->visit();

	const auto menuGuide = Sprite::create("image/mapBg.png");
	const auto sprite_size = menuGuide->getContentSize();
	menuGuide->setPosition(size / 2.0f);
	menuGuide->retain();
	menuGuide->visit();

	// 部屋表示
	int id = 0; 
	const auto& roomData = mapGen.GetMSTNode();
	const auto& roomSize = mapGen.GetRoomData()[0].size;
	const auto& mapParentList = gameMap.GetMapParentList();

	if (roomData.size() <= static_cast<unsigned>(nowID))
	{
		const auto label = Label::createWithTTF("Map Not Found ", "fonts/PixelMplus12-Regular.ttf", 48);
		label->setTextColor(Color4B::BLACK);
		label->setPosition((size / 2.0f) + Size(label->getContentSize().width / 10.0f, label->getContentSize().height));

		this->addChild(label, 0);
	}
	else
	{
		const auto offset = Vec2(size.width / 2.0f - roomData[nowID].key.x,
								 size.height / 2.0f - roomData[nowID].key.y);
		const auto renderer = _director->getRenderer();
		const auto& parentTransform = _director->getMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
		for (auto room : roomData)
		{
			// 到達部屋のみ表示
			if (!mapParentList.mapParents[id].isArrival)
			{
				id++;
				continue;
			}
			const cocos2d::Rect rect = cocos2d::Rect(0.0f, 0.0f, roomSize.width, roomSize.height);
			auto sprite = Sprite::create();
			sprite->setTextureRect(rect);
			sprite->setPosition(room.key + offset);
			sprite->autorelease();
			if (id == nowID)
			{
				sprite->setColor(Color3B(0, 0, 255));
			}
			sprite->setOpacity(128);
			sprite->visit();
			auto drawnode = DrawNode::create();
			drawnode->drawRect(Vec2(0.0f, 0.0f), roomSize, Color4F::GREEN);
			drawnode->setPosition(room.key + (offset - roomSize / 2.0f));
			drawnode->autorelease();
			drawnode->visit(renderer, parentTransform, FLAGS_TRANSFORM_DIRTY);

			id++;
		}
	}

	tex->end();

		// 入力系統
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	auto listener = cocos2d::EventListenerKeyboard::create();
	listener->onKeyPressed = [this](cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* keyEvent)
	{
		if (keyCode == EventKeyboard::KeyCode::KEY_M)
		{
			Director::getInstance()->popScene();
			return true;
		}
	};
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
#endif // (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

	this->setName("MapMenu");
	this->scheduleUpdate();
}

MapMenu::~MapMenu()
{
	delete oprtState_;
	tex->release();
}

cocos2d::Scene* MapMenu::CreateMapMenu(GameMap& gameMap)
{
	MapMenu* pRet = new(std::nothrow) MapMenu(gameMap);
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

void MapMenu::update(float delta)
{
	if (Director::getInstance()->getRunningScene()->getName() != "MapMenu")
	{
		return;
	}

	const auto button = this->getChildByName("cancelBtn");
	if (button != nullptr && static_cast<MenuItemImage*>(button)->isSelected())
	{
		static_cast<MenuItemImage*>(button)->unselected();
		Director::getInstance()->popScene();
	}
}