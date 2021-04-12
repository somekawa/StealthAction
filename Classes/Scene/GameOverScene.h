// ’S“–êŠ
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
	// ƒV[ƒ“‘JˆÚ
	void ChangeScene();
	// ‘JˆÚ’†‚©‚Ç‚¤‚©
	bool isChanged_;
};