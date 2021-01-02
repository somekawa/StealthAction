#include "GameMap.h"
#include "_Debug/_DebugConOut.h"
#include "Scene/LoadScene.h"
#include "obj/Player.h"
#include "Gate.h"
#include "Generator/MapGenerator.h"
#include "Generator/MST.h"

USING_NS_CC;
#pragma execution_character_set("utf-8")

GameMap::GameMap(cocos2d::Layer& layer)
{
	mapGenerator_ = std::make_shared<MapGenerator>();
	mapGenerator_->Call();
	auto a = mapGenerator_->GetMSTNode();
	mapLayer_ = &layer;
	objLayer_ = Layer::create();

	isChangeFloor_ = false;
	
	mapName =  Label::createWithTTF("部屋  0", "fonts/HGRGE.ttc", 24);
	mapName->setPosition(100, 500);

	objLayer_->addChild(mapName);
	// マップレイヤーより前へ
	layer.addChild(objLayer_,layer.getLocalZOrder() + 1);	

	// パスをリスト化
	pathList_.push_back( "image/Environment/gatetest.tmx");

	nextPosTbl = { Vec2(100, 100),	// E_UP
			   Vec2(300, 100),	// N_RIGHT
			   Vec2(700, 100),	// N_LEFT
			   Vec2(900, 100),	// W_UP
			   Vec2(900, 500),	// W_DOWN
			   Vec2(700, 500),	// S_LEFT
			   Vec2(300, 500),	// S_RIGHT
			   Vec2(700, 500),	// E_DOWN 
	};

	// 最初のマップデータ作成(データ化)
	AddMap(pathList_[0]);

	
	MapChild mapChild;
	for (auto& node : a)
	{
		MapParentState mapParent;
		mapParent.mapID = 0;	// サイズが1だったらIDは0
		for (auto& child : node.childData)
		{
			mapChild.mapID = 0;
			mapChild.nextParentID = child.childId;
		
			mapChild.gateDir = child.dir;
			mapChild.nextPos = nextPosTbl[static_cast<int>(mapChild.gateDir)];
			
			mapParent.child.push_back(mapChild);
		}
		mapParentsList_.mapParents.push_back(mapParent);
	}

	mapParentsList_.nowID = 0;
	// 最初のマップのオブジェクトを作る処理
	CreateObject();
	// 最初のマップ以外を見えなくする処理
	for (auto data : mapDatas_)
	{
		if (mapDatas_[mapParentsList_.nowID] == data)
		{
			continue;
		}
		data->setVisible(false);
	}

	frame_ = 0;
}

void GameMap::AddMap(std::string& mapPath)
{
	auto mapArray = createMapFromPath(mapPath);
	mapDatas_.push_back(mapArray);
}

void GameMap::ReplaceMap(Player& player)
{
	auto fade = TransitionFade::create(1.0f, LoadScene::CreateLoadScene(player, mapDatas_, mapParentsList_, *this), Color3B::BLACK);
	Director::getInstance()->pushScene(fade);
	
}

void GameMap::CreateObject()
{
	auto nowMapParent = mapParentsList_.mapParents[mapParentsList_.nowID];
	auto nowMap = mapDatas_[static_cast<int>(nowMapParent.mapID)];
	auto objGroup = nowMap->getObjectGroup("gate");
	if (objGroup == nullptr)
	{
		return;
	}
	// オブジェクトが既にあったら削除する
	if (objs_.size() > 0)
	{
		for (auto obj : objs_)
		{
			obj->removeFromParent();
		}
		objs_.clear();
	}

	auto objs = objGroup->getObjects();
	for (auto obj : objs)
	{
		ValueMap prop = obj.asValueMap();
		auto name = prop["name"].asString();
		auto x = prop["x"].asFloat();
		auto y = prop["y"].asFloat();
		auto w = prop["width"].asInt();
		auto h = prop["height"].asInt();
		
		if (name == "gate")
		{
			auto gateNum = prop["gateNum"].asInt();
			for (int i = 0; i < nowMapParent.child.size(); ++i)
			{
				if (static_cast<int>(nowMapParent.child[i].gateDir) != gateNum)
				{
					continue;
				}
				auto gate = Gate::CreateGate({ x, y }, i);
				gate->setCameraMask(static_cast<int>(CameraFlag::USER1));
				objs_.push_back(gate);
			}
		}
		
	}

	// オブジェクトをセット
	for (auto obj : objs_)
	{
		objLayer_->addChild(obj);
	}
}

cocos2d::TMXTiledMap* GameMap::GetMap()
{
	auto nowMapParent = mapParentsList_.mapParents[mapParentsList_.nowID];
	return mapDatas_[static_cast<int>(nowMapParent.mapID)];
}

//void GameMap::SetMapInfo(MapType mapType)
//{
//	auto nowMapParent = mapParentsList_.mapParents[mapParentsList_.nowID];
//	mapDatas_[static_cast<int>(nowMapParent.mapID)][static_cast<int>(mapType)]->setVisible(true);
//	mapDatas_[static_cast<int>(nowMapParent.mapID)][static_cast<int>(mapType)]->setName("MapData");
//	mapDatas_[static_cast<int>(nowMapParent.mapID)][static_cast<int>(mapType) ^ !0]->setVisible(false);
//	mapDatas_[static_cast<int>(nowMapParent.mapID)][static_cast<int>(mapType) ^ !0]->setName("");
//	nowMapParent.mapType = mapType;
//}

void GameMap::update(Player& player)
{
	// 常にﾌﾛｱ変更のﾌﾗｸﾞはfalseに
	isChangeFloor_ = false;
	for (auto obj : objs_)
	{
		obj->Update(player);
		
		if(obj->IsHit(player))
		{
			nextId = obj->GetGateNum();
			// ﾌﾟﾚｲﾔｰがｹﾞｰﾄをくぐった時にのみtrueにする
			isChangeFloor_ = true;
			ReplaceMap(player);
		}
	}
	frame_++;
}

int GameMap::GetNextChildID()
{
	return nextId;
}

cocos2d::TMXTiledMap* GameMap::createMapFromPath(std::string& mapPath)
{
	TMXTiledMap* map = TMXTiledMap::create(mapPath);
	auto colLayer = map->getLayer("Collision");
	auto wallSlideLayer = map->getLayer("WallSlide");
	colLayer->setName("col");
	wallSlideLayer->setName("slide");
	mapLayer_->addChild(map);
	//map->setPosition(map->getPosition());

	map->setName("MapData");

	cocos2d::TMXTiledMap* mapArray = map;
	return mapArray;
}
