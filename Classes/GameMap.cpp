#include "GameMap.h"
#include "_Debug/_DebugConOut.h"
#include "Scene/LoadScene.h"
#include "obj/Player.h"

USING_NS_CC;

GameMap::GameMap(cocos2d::Layer& layer)
{
	
	mapParents.mapType = MapType::OMOTE;
	mapLayer_ = &layer;
	// パスをリスト化
	pathList_.push_back({ "image/Environment/test.tmx", "image/Environment/uratest.tmx" });
	pathList_.push_back({ "image/Environment/map2.tmx", "image/Environment/uratest.tmx" });

	// 最初のマップデータ作成
	CreateMap(pathList_[0][0], pathList_[0][1]);
	AddNextMap(pathList_[1][0], pathList_[1][1]);

	mapParentsList_.nowID = 0;
	mapParentsList_.mapParents.push_back(mapParents);
}

void GameMap::CreateMap( std::string omotePath, std::string uraPath)
{
	nodeIdx_ = 0;
	auto mapArray = createMapFromPath(omotePath, uraPath); 
	mapDatas_.push_back(mapArray);
	mapParents.ID = mapDatas_.size() - 1;	// サイズが1だったらIDは0
}

void GameMap::AddNextMap(std::string omotePath, std::string uraPath)
{
	auto mapArray = createMapFromPath(omotePath, uraPath);
	MapChild mapChild;
	mapChild.own = mapArray;
	for (auto map : mapChild.own)
	{
		map->setVisible(false);
	}
	// 子供を入れる処理 - dataで判断
	mapParents.child.push_back(mapChild);
}

void GameMap::ReplaceMap()
{
}

cocos2d::TMXTiledMap* GameMap::GetMap()
{
	return mapParents.own[static_cast<int>(mapParents.mapType)];
}

void GameMap::SetMapInfo(MapType mapType)
{
	mapParents.own[static_cast<int>(mapType)]->setVisible(true);
	mapParents.own[static_cast<int>(mapType)]->setName("MapData");
	mapParents.own[static_cast<int>(mapType) ^ !0]->setVisible(false);
	mapParents.own[static_cast<int>(mapType) ^ !0]->setName("");
	mapParents.mapType = mapType;
}

void GameMap::update(Player& player)
{
	auto col = mapParents.own[static_cast<int>(mapParents.mapType)]->getLayer("gate");
	if (col == nullptr)
	{
		return;
	}
	auto pPos = player.getPosition();
	pPos = pPos / 48;
	pPos = { pPos.x,col->getLayerSize().height - pPos.y - 1 };
	auto gid = col->getTileGIDAt(pPos);
	auto proparties = mapParents.own[static_cast<int>(mapParents.mapType)]->getPropertiesForGID(gid);
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
		auto fade = TransitionFade::create(3.0f, LoadScene::CreateLoadScene(player, mapParents), Color3B::BLACK);
		director->pushScene(fade);	
	}
	
	TRACE( "%d\n", gateFlag);
}


std::array<cocos2d::TMXTiledMap*, 2> GameMap::createMapFromPath(std::string& omotePath, std::string& uraPath)
{
	TMXTiledMap* omote = TMXTiledMap::create(omotePath);
	auto colLayer = omote->getLayer("Collision");
	colLayer->setName("col");
	mapLayer_->addChild(omote);
	TMXTiledMap* ura = TMXTiledMap::create(uraPath);
	auto colLayer2 = ura->getLayer("Collision");
	colLayer2->setName("col");
	mapLayer_->addChild(ura);
	//map->setPosition(map->getPosition());

	omote->setName("MapData");
	ura->setVisible(false);

	std::array<cocos2d::TMXTiledMap*, 2> mapArray = {omote, ura};
	return mapArray;
}
