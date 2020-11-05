#include "GameMap.h"
#include "_Debug/_DebugConOut.h"
#include "Scene/LoadScene.h"

USING_NS_CC;

void GameMap::CreateMap(cocos2d::Layer& layer, std::string omotePath, std::string uraPath)
{
	nodeIdx_ = 0;
	auto mapArray = createMapFromPath(omotePath, uraPath, layer); 
	maps_.own = mapArray;
	mapLayer_ = &layer;	
	
}

void GameMap::AddNextMap(cocos2d::Layer& layer, std::string omotePath, std::string uraPath)
{
	auto mapArray = createMapFromPath(omotePath, uraPath, layer);
	MapState mapS;
	mapS.own = mapArray;
	// éqãüÇì¸ÇÍÇÈèàóù - dataÇ≈îªíf
	maps_.child.push_back(mapS);
}

void GameMap::ReplaceMap()
{
}

cocos2d::TMXTiledMap* GameMap::GetMap()
{
	return maps_.own[static_cast<int>(mapType_)];
}

void GameMap::SetMapInfo(MapType mapType)
{
	maps_.own[static_cast<int>(mapType)]->setVisible(true);
	maps_.own[static_cast<int>(mapType)]->setName("MapData");
	maps_.own[static_cast<int>(mapType) ^ !0]->setVisible(false);
	maps_.own[static_cast<int>(mapType) ^ !0]->setName("");
	mapType_ = mapType;
}

void GameMap::update(cocos2d::Vec2 pos)
{
	auto col = maps_.own[static_cast<int>(mapType_)]->getLayer("gate");
	if (col == nullptr)
	{
		return;
	}
	auto pPos = pos;
	pPos = pos / 48;
	pPos = { pPos.x,col->getLayerSize().height - pPos.y - 1 };
	auto gid = col->getTileGIDAt(pPos);
	auto proparties = maps_.own[static_cast<int>(mapType_)]->getPropertiesForGID(gid);
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
		// mapsÇêÿÇËë÷Ç¶
		maps_.own = maps_.child[0].own;
		maps_.child = maps_.child[0].child;
		auto fade = TransitionFade::create(3.0f, LoadScene::create(), Color3B::BLACK);
		director->pushScene(fade);

	}
	
	TRACE( "%d\n", gateFlag);
}


std::array<cocos2d::TMXTiledMap*, 2> GameMap::createMapFromPath(std::string& omotePath, std::string& uraPath, cocos2d::Layer& layer)
{
	TMXTiledMap* omote = TMXTiledMap::create(omotePath);
	auto colLayer = omote->getLayer("Collision");
	colLayer->setName("col");
	layer.addChild(omote);
	TMXTiledMap* ura = TMXTiledMap::create(uraPath);
	auto colLayer2 = ura->getLayer("Collision");
	colLayer2->setName("col");
	layer.addChild(ura);
	//map->setPosition(map->getPosition());
	/*MapState map;
	maps_.own[0] = omote;
	maps_.own[0]->setName("MapData");
	maps_.own[1] = ura;
	maps_.own[1]->setVisible(false);*/

	std::array<cocos2d::TMXTiledMap*, 2> mapArray = {omote, ura};
	return mapArray;
}
