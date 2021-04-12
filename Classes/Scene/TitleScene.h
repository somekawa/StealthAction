// ’S“–êŠ
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
	// ƒV[ƒ“‘JˆÚ
	void ChangeScene();
	// ‘JˆÚ’†‚©‚Ç‚¤‚©
	bool isChanged_;
	// ƒLƒƒƒ‰”½“]
	bool flipFlg_;
	// ƒLƒƒƒ‰ˆÚ“®‘¬“x
	float runSpeed_;
};