// �S���ꏊ
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
	// �V�[���J��
	void ChangeScene();
	// �J�ڒ����ǂ���
	bool isChanged_;
};