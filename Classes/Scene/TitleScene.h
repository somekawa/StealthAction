#pragma once
#include "BaseScene.h"

class TitleScene : public BaseScene
{
public:
	static Scene* CreateTitleScene();
	TitleScene();

	bool init()override;
	void update(float delta)override;
private:
	void ChangeScene();
};