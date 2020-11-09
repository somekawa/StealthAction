#include "LoadScene.h"
#include "GameScene.h"
#include "GameMap.h"
#include "obj/Player.h"
#include <algorithm>

USING_NS_CC;

Scene* LoadScene::CreateLoadScene(
	Player& player,MapData& mapData, MapParentList& mpList)
{
	LoadScene* pRet = new(std::nothrow) LoadScene(player, mapData ,mpList );
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
	MapParentList& mpList)
	: player_(player), mapParentList_(mpList), mapData_(mapData)
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
	//auto fade = TransitionFade::create(3.0f, LoadScene::create(), Color3B::BLACK);
	player_.setPosition(200, 300);
	/*auto cameras = this->getCameras();
	std::find_if(cameras.begin(), cameras.end(),)*/

	auto mapState_ = mapParentList_.mapParents[mapParentList_.nowID];
	auto selectNextMap = mapData_[static_cast<int>(mapState_.child[0].mapID)];
	auto nowMap = mapData_[static_cast<int>(mapState_.mapID)];
	auto mapType = static_cast<int>(mapState_.mapType);
	selectNextMap[mapType]->setVisible(true);
	selectNextMap[mapType]->setName("MapData");
	nowMap[mapType]->setVisible(false);
	nowMap[mapType]->setName("");
	mapParentList_.nowID = static_cast<int>(mapState_.child[0].mapID);
	//mapState_.mapID;
	director->popScene();
}


