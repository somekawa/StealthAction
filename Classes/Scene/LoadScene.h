#pragma once
#include "BaseScene.h"
#include <vector>
using MapData = std::vector<cocos2d::TMXTiledMap*>;

class Player;
class GameMap;
class LoadScene : public BaseScene
{
public:
	static Scene* CreateLoadScene(Player& player, GameMap& gameMap);
	LoadScene(Player& player,GameMap& gameMap);

	bool init()override;
	void update(float delta)override;
private:
	Scene* scene;
	Player& player_;
	GameMap* gameMap_;

};

