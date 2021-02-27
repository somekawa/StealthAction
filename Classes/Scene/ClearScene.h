#pragma once
#include "BaseScene.h"

class ClearScene : public BaseScene
{
public:
	static Scene* CreateClearScene();
	ClearScene();

	bool init()override;
	void update(float delta)override;
private:
	// ƒV[ƒ“‘JˆÚ
	void ChangeScene();
	// ‘JˆÚ’†‚©‚Ç‚¤‚©
	bool isChanged_;
};