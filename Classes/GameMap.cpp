#include "GameMap.h"

USING_NS_CC;

void GameMap::CreateMap(cocos2d::Layer& layer, std::string omotePath, std::string uraPath)
{
	createMapFromPath(omotePath, layer);
	if (uraPath != "")
	{
		createMapFromPath(uraPath, layer);
	} 
	//maps_[1].setPosition(maps_[1].)
	mapLayer_ = &layer;	
}

void GameMap::ReplaceMap()
{
}

void GameMap::createMapFromPath(std::string& omotePath, cocos2d::Layer& layer)
{
	TMXTiledMap* map = TMXTiledMap::create(omotePath);
	layer.addChild(map);
	//map->setPosition(map->getPosition());
	//maps_.emplace_back(map);
}
