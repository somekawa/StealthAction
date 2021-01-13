#include "MapMenu.h"
#include "Generator/MapGenerator.h"
#include "Scene/GameScene.h"
#include "Generator/MST.h"

USING_NS_CC;

MapMenu::MapMenu(MapGenerator& mapGen, int nowID)
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

	// ƒQ[ƒ€ƒV[ƒ“•\Ž¦
	gameScene->visit();
	cocos2d::Rect rect = cocos2d::Rect(0, 0, size.width, size.height);
	auto bg = Sprite::create();
	bg->setTextureRect(rect);
	bg->setColor({ 0, 0, 0 });
	bg->setOpacity(200);
	bg->setPosition(size.width / 2, size.height / 2);
	bg->retain();
	bg->visit();

	// •”‰®•\Ž¦
	auto& roomData = mapGen.GetMSTNode();
	auto& roomSize = mapGen.GetRoomData()[0].size;
	int id = 0; 
	auto offset = Vec2(size.width / 2 - roomData[nowID].key.x,
					   size.height / 2 - roomData[nowID].key.y);
	for (auto room : roomData)
	{
		cocos2d::Rect rect = cocos2d::Rect(0,0, roomSize.width, roomSize.height);
		auto sprite = Sprite::create();
		sprite->setTextureRect(rect);
		sprite->setPosition(room.key + offset);
		sprite->retain();
		if (id == nowID)
		{
			sprite->setColor(Color3B(0.0f, 0.0f, 255.0f));
		}
		sprite->visit();
		id++;
	}
	// ’Ê˜H•\Ž¦
	auto& lineData = mapGen.GetEdgeData();
	auto renderer = _director->getRenderer();
	auto& parentTransform = _director->getMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
	for (auto line : lineData)
	{
		if (!line.used)
		{
			continue;
		}
		auto sprite = DrawNode::create();
		sprite->drawSegment(line.pair_vertex[0] + offset,
			line.pair_vertex[1] + offset, 1.5f, Color4F::RED);
		sprite->retain();
		sprite->visit(renderer, parentTransform, FLAGS_TRANSFORM_DIRTY);
	}

	tex->end();
	// “ü—ÍŒn“
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	auto listener = cocos2d::EventListenerKeyboard::create();
	listener->onKeyPressed = [this](cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* keyEvent)
	{
		if (keyCode == EventKeyboard::KeyCode::KEY_M)
		{
			Director::getInstance()->popScene();
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

MapMenu::~MapMenu()
{
}

cocos2d::Scene* MapMenu::CreateMapMenu(MapGenerator& mapGen, int nowID)
{
	MapMenu* pRet = new(std::nothrow) MapMenu(mapGen,nowID);
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
