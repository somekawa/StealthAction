#include "GameMap.h"

USING_NS_CC;

void GameMap::CreateMap(cocos2d::Layer& layer, std::string omotePath, std::string uraPath)
{
	createMapFromPath(omotePath, layer);
	if (uraPath != "")
	{
		createMapFromPath(uraPath, layer);
	} 
	//maps_[0]->setPositionZ(-50);
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
	maps_[(static_cast<int>(mapType) ^! 0)]->setVisible(false);
	mapType_ = mapType;
}


void GameMap::createMapFromPath(std::string& path, cocos2d::Layer& layer)
{
	
	TMXTiledMap* map = TMXTiledMap::create(path);
	map->setName("MapData");
	auto colLayer = map->getLayer("Collision");
	colLayer->setName("col");
	layer.addChild(map);
	//map->setPosition(map->getPosition());
	maps_.emplace_back(map);
}
