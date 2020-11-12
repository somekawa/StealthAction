#include "LoadScene.h"
#include "GameScene.h"
#include "GameMap.h"
#include "obj/Player.h"
#include <algorithm>

USING_NS_CC;

Scene* LoadScene::CreateLoadScene(
	Player& player,MapData& mapData,
	MapParentList& mpList,GameMap& gameMap)
{
	LoadScene* pRet = new(std::nothrow) LoadScene(player, mapData ,mpList, gameMap);
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

LoadScene::LoadScene(Player& player, 
	MapData& mapData,
	MapParentList& mpList,
	GameMap& gameMap)
	: player_(player), mapParentList_(mpList), mapData_(mapData), gameMap_(&gameMap)
{
	scene = nullptr;
	
}

bool LoadScene::init()
{
	if (!Scene::init())
	{
		return false;
	}
	//this->scheduleUpdate();
	if (Director::getInstance()->getRunningScene()->getName() == "GameScene")
	{
		auto size = Director::getInstance()->getWinSize();
	
		
		/*auto c = this->getDefaultCamera();
		auto camera = Camera::createOrthographic(size.width, size.height, c->getNearPlane() - 768, c->getFarPlane());
		camera->setCameraFlag(CameraFlag::USER1);*/
		
		scene = Director::getInstance()->getRunningScene();
		scene->retain();
	}
	
	this->setName("LoadScene");
	this->scheduleUpdate();
	return true;
}

void LoadScene::update(float delta)
{
	auto director = Director::getInstance();
	if (director->getRunningScene()->getName() != "LoadScene")
	{
		return;
	}
	
	int childId = gameMap_->GetNextChildID();
	auto mapState_ = mapParentList_.mapParents[mapParentList_.nowID];
	auto selectNextMap = mapData_[static_cast<int>(mapState_.child[childId].mapID)];
	auto nowMap = mapData_[static_cast<int>(mapState_.mapID)];
	auto mapType = static_cast<int>(mapState_.mapType);
	selectNextMap[mapType]->setVisible(true);
	selectNextMap[mapType]->setName("MapData");
	nowMap[mapType]->setVisible(false);
	nowMap[mapType]->setName("");
	mapParentList_.nowID = static_cast<int>(mapState_.child[childId].nextParentID);
	player_.setPosition(200, 300);
	gameMap_->CreateObject();
	//mapState_.mapID;
	director->popScene();
}


