// 担当場所
#pragma once
#include "BaseScene.h"

class TitleScene : public BaseScene
{
public:
	static Scene* CreateTitleScene();
	TitleScene();
	~TitleScene();

	bool init()override;
	void update(float delta)override;
private:
	// シーン遷移
	void ChangeScene();
	// 遷移中かどうか
	bool isChanged_;
	// キャラ反転
	bool flipFlg_;
	// キャラ移動速度
	float runSpeed_;
};