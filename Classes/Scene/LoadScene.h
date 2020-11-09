#pragma once
#include "BaseScene.h"

class Player;
struct MapParentState;
class LoadScene : public BaseScene
{
public:
	static Scene* CreateLoadScene(Player& player,MapParentState& state );
	LoadScene(Player& player, MapParentState& state);

	bool init()override;
	void update(float delta)override;
private:
	Scene* scene;
	Player& player_;
	MapParentState& mapstate_;

};

