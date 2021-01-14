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
	auto nodeData = mapGenerator_->GetMSTNode();
	mapLayer_ = &layer;
	objLayer_ = Layer::create();

	isChangeFloor_ = false;

#ifdef _DEBUG
	mapName_ = Label::createWithTTF("部屋  0", "fonts/HGRGE.ttc", 24);
	mapName_->setPosition(100, 500);

	objLayer_->addChild(mapName_);
	// マップレイヤーより前へ
#endif // _DEBUG

	layer.addChild(objLayer_,layer.getLocalZOrder() + 1);	

	// パスをリスト化
	pathList_.push_back( "image/Environment/trueMap.tmx");

	nextPosTbl = {
		Vec2(100, 100),	// E_UP
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
	for (auto& node : nodeData)
	{
		MapParentState mapParent;
		mapParent.mapID = 0;	// サイズが1だったらIDは0
		mapParent.enemyNum = RandomHelper::random_int(3, 7);
		mapParent.isArrival = false;
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
	mapParentsList_.mapParents[mapParentsList_.nowID].isArrival = true;
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

void GameMap::LoadMap(Player& player)
{
	auto fade = TransitionFade::create(1.0f, LoadScene::CreateLoadScene(player, *this), Color3B::BLACK);
	Director::getInstance()->pushScene(fade);
}

void GameMap::ReplaceMap(Player& player)
{
	int childId = nextId_;
	auto mapState_ = mapParentsList_.mapParents[mapParentsList_.nowID];
	auto selectNextMap = mapDatas_[static_cast<int>(mapState_.child[childId].mapID)];
	auto nowMap = GetNowMap();
	if (nowMap != selectNextMap)
	{
		selectNextMap->setVisible(true);
		selectNextMap->setName("MapData");
		nowMap->setVisible(false);
		nowMap->setName("");
	}
	mapParentsList_.nowID = static_cast<int>(mapState_.child[childId].nextParentID);
	mapParentsList_.mapParents[mapParentsList_.nowID].isArrival = true;
	// オブジェクト作成
	CreateObject();
#ifdef _DEBUG
	auto str = StringUtils::format("部屋　%d", mapParentsList_.nowID);
	mapName_->setString(str);
#endif // _DEBUG
	// プレイヤーポジションセット
	player.setPosition(mapState_.child[childId].nextPos);
}

void GameMap::CreateObject()
{

	auto objGroup = GetNowMap()->getObjectGroup("gate");
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

	auto nowMapParent = mapParentsList_.mapParents[mapParentsList_.nowID];
	// TMXMapのオブジェクトレイヤーから情報取得
	auto& objs = objGroup->getObjects();
	for (auto& obj : objs)
	{
		ValueMap prop = obj.asValueMap();
		auto name = prop["name"].asString();
		auto x = prop["x"].asFloat();
		auto y = prop["y"].asFloat();
		auto w = prop["width"].asInt();
		auto h = prop["height"].asInt();
		
		// ゲートだった時(今はこれしか存在しない)
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

cocos2d::TMXTiledMap* GameMap::GetNowMap()
{
	auto nowMapParent = mapParentsList_.mapParents[mapParentsList_.nowID];
	return mapDatas_[static_cast<int>(nowMapParent.mapID)];
}

void GameMap::update(Player& player)
{
	// 常にフロア変更のフラグはfalseに
	isChangeFloor_ = false;
	for (auto obj : objs_)
	{
		obj->Update(player);
		
		if(obj->IsHit(player))
		{
			nextId_ = obj->GetGateNum();
			// プレイヤーがゲートをくぐった時にのみtrueにする
			isChangeFloor_ = true;
			LoadMap(player);
		}
	}
	frame_++;
}

MapParentList& GameMap::GetMapParentList()
{
	return mapParentsList_;
}

const int GameMap::GetNowID()
{
	return mapParentsList_.nowID;
}

int GameMap::GetEnemyNum()
{
	return mapParentsList_.mapParents[mapParentsList_.nowID].enemyNum;
}

MapGenerator& GameMap::GetMapGenerator()
{
	return *mapGenerator_;
}

cocos2d::TMXTiledMap* GameMap::createMapFromPath(std::string& mapPath)
{
	TMXTiledMap* map = TMXTiledMap::create(mapPath);
	auto colLayer = map->getLayer("Collision");
	auto wallSlideLayer = map->getLayer("WallSlide");
	colLayer->setName("col");
	wallSlideLayer->setName("slide");
	mapLayer_->addChild(map);
	map->setName("MapData");

	cocos2d::TMXTiledMap* mapArray = map;
	return mapArray;
}

const bool GameMap::ChangeFloor()
{
	return isChangeFloor_;
}
