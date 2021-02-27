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
	const auto& nowID = gameMap.GetNowID();
	auto director = Director::getInstance();
	auto size = director->getVisibleSize();
	tex = RenderTexture::create(size.width, size.height);
	tex->setPosition(size.width / 2, size.height / 2);
	this->addChild(tex);

	auto gameScene = director->getRunningScene();
	// ×ボタン画像に変える
	auto canselBtn = MenuItemImage::create(
		"image/keep_button1.png",
		"image/keep_button1_select.png");
	canselBtn->setPosition(Vec2(size.width - (canselBtn->getContentSize().width * 0.25) / 2 - 10, size.height - (canselBtn->getContentSize().height * 0.25) / 2 - 10));
	canselBtn->setScale(0.25f);
	canselBtn->setName("cancelBtn");		// Guideと同じ名前を設定する
	this->addChild(canselBtn);
	tex->begin();

	// ゲームシーン表示
	gameScene->visit();

	// 部屋表示
	int id = 0; 
	auto& roomData = mapGen.GetMSTNode();
	auto& roomSize = mapGen.GetRoomData()[0].size;
	const auto& mapParentList = gameMap.GetMapParentList();
	if (roomData.size() > nowID)
	{
		auto offset = Vec2(size.width / 2 - roomData[nowID].key.x,
			size.height / 2 - roomData[nowID].key.y);
		auto renderer = _director->getRenderer();
		auto& parentTransform = _director->getMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
		for (auto room : roomData)
		{
			// 到達部屋のみ表示 現在制作のため
			if (!mapParentList.mapParents[id].isArrival)
			{
				id++;
				continue;
			}
			cocos2d::Rect rect = cocos2d::Rect(0, 0, roomSize.width, roomSize.height);
			auto sprite = Sprite::create();
			sprite->setTextureRect(rect);
			sprite->setPosition(room.key + offset);
			sprite->autorelease();
			if (id == nowID)
			{
				sprite->setColor(Color3B(0.0f, 0.0f, 255.0f));
			}
			sprite->setOpacity(128);
			sprite->visit();
			auto drawnode = DrawNode::create();
			drawnode->drawRect(Vec2(0, 0), roomSize, Color4F::GREEN);
			drawnode->setPosition(room.key + (offset - roomSize / 2));
			drawnode->autorelease();
			drawnode->visit(renderer, parentTransform, FLAGS_TRANSFORM_DIRTY);

			id++;
		}
		// 通路表示
		auto& lineData = mapGen.GetEdgeData();
		for (auto line : lineData)
		{
			if (!line.used)
			{
				continue;
			}
			auto sprite = DrawNode::create();
			sprite->drawSegment(line.pair_vertex[0] + offset,
				line.pair_vertex[1] + offset, 1.5f, Color4F::RED);
			sprite->autorelease();
			sprite->visit(renderer, parentTransform, FLAGS_TRANSFORM_DIRTY);
		}
	}
	else
	{
		auto visibleSize = Director::getInstance()->getVisibleSize();
		Vec2 origin = Director::getInstance()->getVisibleOrigin();
		auto label = Label::createWithTTF("Map Not Found ", "fonts/PixelMplus12-Regular.ttf", 48);
		label->setPosition(Vec2(origin.x + visibleSize.width / 2,
			origin.y + visibleSize.height /2 - (label->getContentSize().height)));

		// add the label as a child to this layer
		this->addChild(label, 0);
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
#else
	//auto listener = EventListenerTouchOneByOne::create();
	//listener->onTouchBegan = [this](cocos2d::Touch* touch, cocos2d::Event* event)
	//{
	//	Director::getInstance()->popScene();
	//	return true;
	//};
#endif // (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

	this->setName("MapMenu");
	this->scheduleUpdate();
}

MapMenu::~MapMenu()
{
	delete oprtState_;
	tex->release();
}

void MapMenu::update(float delta)
{
	if (Director::getInstance()->getRunningScene()->getName() != "MapMenu")
	{
		return;
	}

	auto label1 = this->getChildByName("cancelBtn");
	if (label1 != nullptr && ((MenuItemImage*)label1)->isSelected())
	{
		((MenuItemImage*)label1)->unselected();
		Director::getInstance()->popScene();
	}
}

cocos2d::Scene* MapMenu::CreateMapMenu(GameMap& gameMap)
{
	MapMenu* pRet = new(std::nothrow) MapMenu(gameMap);
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
