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
	// �V�[���J��
	void ChangeScene();
	// �J�ڒ����ǂ���
	bool isChanged_;
};