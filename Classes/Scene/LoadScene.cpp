#include "LoadScene.h"
#include "GameScene.h"
#include "GameMap.h"
#include "obj/Player.h"

USING_NS_CC;

Scene* LoadScene::CreateLoadScene(Player& player, MapParentState& state)
{
	LoadScene* pRet = new(std::nothrow) LoadScene(player, state);
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

LoadScene::LoadScene(Player& player, MapParentState& state) : player_(player), mapstate_(state)
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
	mapstate_.child[0].own[static_cast<int>(mapstate_.mapType)]->setVisible(true);
	mapstate_.child[0].own[static_cast<int>(mapstate_.mapType)]->setName("MapData");
	mapstate_.own[static_cast<int>(mapstate_.mapType)]->setVisible(false);
	mapstate_.own[static_cast<int>(mapstate_.mapType)]->setName("");
	mapstate_.own = mapstate_.child[0].own;
	director->popScene();
}


