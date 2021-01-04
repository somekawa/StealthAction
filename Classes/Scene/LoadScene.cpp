#include "LoadScene.h"
#include "GameScene.h"
#include "GameMap.h"
#include "obj/Player.h"
#include <algorithm>

USING_NS_CC;
#pragma execution_character_set("utf-8")

Scene* LoadScene::CreateLoadScene(Player& player,GameMap& gameMap)
{
	LoadScene* pRet = new(std::nothrow) LoadScene(player, gameMap);
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

LoadScene::LoadScene(Player& player, GameMap& gameMap): player_(player), gameMap_(&gameMap)
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
	gameMap_->ReplaceMap(player_);
	director->popScene();
}


