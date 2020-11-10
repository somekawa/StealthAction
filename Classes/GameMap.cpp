#include "GameMap.h"
#include "_Debug/_DebugConOut.h"
#include "Scene/LoadScene.h"
#include "obj/Player.h"

USING_NS_CC;

GameMap::GameMap(cocos2d::Layer& layer)
{
	MapParentState mapParent;
	mapParent.mapType = MapType::OMOTE;
	mapLayer_ = &layer;
	// パスをリスト化
	pathList_.push_back({ "image/Environment/test.tmx", "image/Environment/uratest.tmx" });
	pathList_.push_back({ "image/Environment/map2.tmx", "image/Environment/uratest.tmx" });

	// 最初のマップデータ作成(データ化)
	AddMap(pathList_[0][0], pathList_[0][1]);
	AddMap(pathList_[1][0], pathList_[1][1]);
	{		
		mapParent.mapID = 0;	// サイズが1だったらIDは0
		MapChild mapChild;
		mapChild.mapID = 1;
		mapChild.nextParentID = 1;
		// 子供を入れる処理 
		mapParent.child.push_back(mapChild);
		// 親リストに登録
		mapParentsList_.mapParents.push_back(mapParent);
	}
	{
		mapParent.mapID = 1;	// サイズが1だったらIDは0
		MapChild mapChild;
		mapChild.mapID = 0;
		mapChild.nextParentID = 0;
		// 子供を入れる処理 
		mapParent.child.push_back(mapChild);
		// 親リストに登録
		mapParentsList_.mapParents.push_back(mapParent);
	}
	mapParentsList_.nowID = 0;
	for (auto data : mapDatas_)
	{
		if (mapDatas_[mapParentsList_.nowID] == data)
		{
			continue;
		}
		data[0]->setVisible(false);
		data[1]->setVisible(false);
	}
}

void GameMap::AddMap( std::string omotePath, std::string uraPath)
{
	auto mapArray = createMapFromPath(omotePath, uraPath); 
	mapDatas_.push_back(mapArray);
}

void GameMap::AddNextMap( std::string omotePath, std::string uraPath)
{
	auto mapArray = createMapFromPath(omotePath, uraPath);
	mapDatas_.push_back(mapArray);
}

void GameMap::ReplaceMap()
{
}

cocos2d::TMXTiledMap* GameMap::GetMap()
{
	auto nowMapParent = mapParentsList_.mapParents[mapParentsList_.nowID];
	return mapDatas_[static_cast<int>(nowMapParent.mapID)][static_cast<int>(nowMapParent.mapType)];
}

void GameMap::SetMapInfo(MapType mapType)
{
	auto nowMapParent = mapParentsList_.mapParents[mapParentsList_.nowID];
	mapDatas_[static_cast<int>(nowMapParent.mapID)][static_cast<int>(mapType)]->setVisible(true);
	mapDatas_[static_cast<int>(nowMapParent.mapID)][static_cast<int>(mapType)]->setName("MapData");
	mapDatas_[static_cast<int>(nowMapParent.mapID)][static_cast<int>(mapType) ^ !0]->setVisible(false);
	mapDatas_[static_cast<int>(nowMapParent.mapID)][static_cast<int>(mapType) ^ !0]->setName("");
	nowMapParent.mapType = mapType;
}

void GameMap::update(Player& player)
{
	auto nowMapParent = mapParentsList_.mapParents[mapParentsList_.nowID];
	auto col = mapDatas_[static_cast<int>(nowMapParent.mapID)][static_cast<int>(nowMapParent.mapType)]->getLayer("gate");
	if (col == nullptr)
	{
		return;
	}
	auto pPos = player.getPosition();
	pPos = pPos / 48;
	pPos = { pPos.x,col->getLayerSize().height - pPos.y - 1 };
	auto gid = col->getTileGIDAt(pPos);
	auto proparties = mapDatas_[static_cast<int>(nowMapParent.mapID)][static_cast<int>(nowMapParent.mapType)]->getPropertiesForGID(gid);
	if (proparties.isNull())
	{
		return;
	}
	auto director = Director::getInstance();
	auto mapProp = proparties.asValueMap();
	bool gateFlag = mapProp["gate"].asBool();
	if (gateFlag)
	{		
		auto fade = TransitionFade::create(3.0f, LoadScene::CreateLoadScene(player,mapDatas_,mapParentsList_), Color3B::BLACK);
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
