#pragma once
#include "BaseScene.h"
#include <vector>
using MapData = std::vector<std::array<cocos2d::TMXTiledMap*, 2>>;

class Player;
struct MapParentList;
class LoadScene : public BaseScene
{
public:
	static Scene* CreateLoadScene(Player& player, MapData& mapData, MapParentList& mpList );
	LoadScene(Player& player, MapData& mapData ,MapParentList& mpList);

	bool init()override;
	void update(float delta)override;
private:
	Scene* scene;
	Player& player_;
	MapParentList& mapParentList_;
	MapData& mapData_;

};

