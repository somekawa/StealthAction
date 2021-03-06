#pragma once
#include "BaseScene.h"

class GameOverScene : public BaseScene
{
public:
	static Scene* CreateGameOverScene();
	GameOverScene();

	bool init()override;
	void update(float delta)override;
private:
	// シーン遷移
	void ChangeScene();
	// 遷移中かどうか
	bool isChanged_;
};