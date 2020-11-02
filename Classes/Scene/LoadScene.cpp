#include "LoadScene.h"

USING_NS_CC;

Scene* LoadScene::CreateLoadScene()
{
	return LoadScene::create();
}

LoadScene::LoadScene()
{
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
		auto a = Director::getInstance()->getRunningScene();
	}
	
	return true;
}

void LoadScene::update(float delta)
{
	
}


