#include "GameMap.h"
#include "_Debug/_DebugConOut.h"
#include "Scene/LoadScene.h"
#include "obj/Player.h"
#include "Gate.h"

USING_NS_CC;

GameMap::GameMap(cocos2d::Layer& layer)
{
	mapLayer_ = &layer;
	objLayer_ = Layer::create();
	
	// �}�b�v���C���[���O��
	layer.addChild(objLayer_,layer.getLocalZOrder() + 1);	

	// �p�X�����X�g��
	pathList_.push_back({ "image/Environment/test.tmx", "image/Environment/uratest.tmx" });
	pathList_.push_back({ "image/Environment/map2.tmx", "image/Environment/uratest.tmx" });
	pathList_.push_back({ "image/Environment/map3.tmx", "image/Environment/uratest.tmx" });

	// �ŏ��̃}�b�v�f�[�^�쐬(�f�[�^��)
	AddMap(pathList_[0][0], pathList_[0][1]);
	AddMap(pathList_[1][0], pathList_[1][1]);	
	AddMap(pathList_[2][0], pathList_[2][1]);	
	{		
		MapParentState mapParent;
		mapParent.mapType = MapType::OMOTE;
		mapParent.mapID = 0;	// �T�C�Y��1��������ID��0
		MapChild mapChild;
		mapChild.mapID = 1;
		mapChild.nextParentID = 1;
		// �q�������鏈�� 
		mapParent.child.push_back(mapChild);
		// �e���X�g�ɓo�^
		mapParentsList_.mapParents.push_back(mapParent);
	}
	{
		MapParentState mapParent;
		mapParent.mapType = MapType::OMOTE;
		mapParent.mapID = 1;	
		MapChild mapChild;
		mapChild.mapID = 2;
		mapChild.nextParentID = 2;
		// �q�������鏈�� 
		mapParent.child.push_back(mapChild);
		// �e���X�g�ɓo�^
		mapParentsList_.mapParents.push_back(mapParent);
	}
	{
		MapParentState mapParent;
		mapParent.mapType = MapType::OMOTE;
		mapParent.mapID = 2;	
		MapChild mapChild;
		mapChild.mapID = 0;
		mapChild.nextParentID = 0;
		// �q�������鏈�� 
		mapParent.child.push_back(mapChild);
		mapChild.mapID = 1;
		mapChild.nextParentID = 1;
		mapParent.child.push_back(mapChild);
		// �e���X�g�ɓo�^
		mapParentsList_.mapParents.push_back(mapParent);
	}
	mapParentsList_.nowID = 0;
	// �ŏ��̃}�b�v�̃I�u�W�F�N�g����鏈��
	CreateObject();
	// �ŏ��̃}�b�v�ȊO�������Ȃ����鏈��
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

void GameMap::ReplaceMap(Player& player)
{
	auto fade = TransitionFade::create(3.0f, LoadScene::CreateLoadScene(player, mapDatas_, mapParentsList_, *this), Color3B::BLACK);
	Director::getInstance()->pushScene(fade);
}

void GameMap::CreateObject()
{
	auto nowMapParent = mapParentsList_.mapParents[mapParentsList_.nowID];
	auto nowMap = mapDatas_[static_cast<int>(nowMapParent.mapID)][static_cast<int>(nowMapParent.mapType)];
	auto objGroup = nowMap->getObjectGroup("gate");
	if (objGroup == nullptr)
	{
		return;
	}
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
		auto gateNum = prop["gateNum"].asInt();
		if (name == "gate")
		{
			auto gate = Gate::CreateGate({x, y}, gateNum);
			gate->setCameraMask(static_cast<int>(CameraFlag::USER1));
			objs_.push_back(gate);
		}
		objLayer_->addChild(objs_.back());
	}

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
	for (auto obj : objs_)
	{
		if(obj->IsHit(player))
		{
			nextId = obj->GetGateNum();
			ReplaceMap(player);
		}
	}
	/*auto nowMapParent = mapParentsList_.mapParents[mapParentsList_.nowID];
	auto nowMap =  mapDatas_[static_cast<int>(nowMapParent.mapID)][static_cast<int>(nowMapParent.mapType)];
	for (int y = 0; y < 22; y++)
	{
		for (int x = 0; x < 42; x++)
		{
			nowMap->getLayer("Collision")->setTileGID(0, Vec2(x, y));
		}
	}*/
	
	/*auto nowMapParent = mapParentsList_.mapParents[mapParentsList_.nowID];
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
	*/
	
	//TRACE( "%d\n", gateFlag);
}

int GameMap::GetNextChildID()
{
	return nextId;
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
