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
	// シーン遷移
	void ChangeScene();
	// 遷移中かどうか
	bool isChanged_;
};