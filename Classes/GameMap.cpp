#include "GameMap.h"
#include "_Debug/_DebugConOut.h"
#include "Scene/LoadScene.h"

USING_NS_CC;

void GameMap::CreateMap(cocos2d::Layer& layer, std::string omotePath, std::string uraPath)
{
	createMapFromPath(omotePath, layer);
	if (uraPath != "")
	{
		createMapFromPath(uraPath, layer);
	} 
	maps_[0]->setName("MapData");
	maps_[1]->setVisible(false);
	mapLayer_ = &layer;	
	
}

void GameMap::ReplaceMap()
{
}

cocos2d::TMXTiledMap* GameMap::GetMap()
{
	return maps_[static_cast<int>(mapType_)];
}

void GameMap::SetMapInfo(MapType mapType)
{
	maps_[static_cast<int>(mapType)]->setVisible(true);
	maps_[static_cast<int>(mapType)]->setName("MapData");
	maps_[(static_cast<int>(mapType) ^! 0)]->setVisible(false);
	maps_[(static_cast<int>(mapType) ^ !0)]->setName("");
	mapType_ = mapType;
}

void GameMap::update(cocos2d::Vec2 pos)
{
	auto col = maps_[static_cast<int>(mapType_)]->getLayer("gate");
	if (col == nullptr)
	{
		return;
	}
	auto pPos = pos;
	pPos = pos / 48;
	pPos = { pPos.x,col->getLayerSize().height - pPos.y - 1 };
	auto gid = col->getTileGIDAt(pPos);
	auto proparties = maps_[static_cast<int>(mapType_)]->getPropertiesForGID(gid);
	if (proparties.isNull())
	{
		return;
	}
	auto director = Director::getInstance();
	//auto scene = director->getRunningScene();
	auto mapProp = proparties.asValueMap();
	bool gateFlag = mapProp["gate"].asBool();
	if (gateFlag)
	{
		auto fade = TransitionFade::create(3.0f, LoadScene::create(), Color3B::BLACK);
		director->pushScene(fade);

	}
	
	TRACE( "%d\n", gateFlag);
}


void GameMap::createMapFromPath(std::string& path, cocos2d::Layer& layer)
{
	
	TMXTiledMap* map = TMXTiledMap::create(path);
	auto colLayer = map->getLayer("Collision");
	colLayer->setName("col");
	layer.addChild(map);
	//map->setPosition(map->getPosition());
	maps_.emplace_back(map);
}
