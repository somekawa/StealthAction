#pragma once
#include "BaseScene.h"
#include <vector>
using MapData = std::vector<cocos2d::TMXTiledMap*>;

class Player;
struct MapParentList;
class GameMap;
class LoadScene : public BaseScene
{
public:
	static Scene* CreateLoadScene(Player& player, MapData& mapData, MapParentList& mpList , GameMap& gameMap);
	LoadScene(Player& player, MapData& mapData ,MapParentList& mpList, GameMap& gameMap);

	bool init()override;
	void update(float delta)override;
private:
	Scene* scene;
	Player& player_;
	MapParentList& mapParentList_;
	MapData& mapData_;
	GameMap* gameMap_;

};

