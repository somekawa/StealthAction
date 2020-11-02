#pragma once
#include "BaseScene.h"

class LoadScene : public BaseScene
{
public:
	static Scene* CreateLoadScene();
	LoadScene();

	bool init()override;
	void update(float delta)override;


	CREATE_FUNC(LoadScene);
};

