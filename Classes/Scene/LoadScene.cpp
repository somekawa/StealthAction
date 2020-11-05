#include "LoadScene.h"
#include "GameScene.h"
#include "GameMap.h"

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
	director->popScene();
}


