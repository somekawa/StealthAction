#include "GameMap.h"
#include "_Debug/_DebugConOut.h"

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
	auto pPos = pos;
	pPos = pos / 48;
	pPos = { pPos.x,col->getLayerSize().height - pPos.y - 1 };
	auto gid = col->getTileGIDAt(pPos);
	auto a = maps_[static_cast<int>(mapType_)]->getPropertiesForGID(gid);
	TRACE("%d\n", a);
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
