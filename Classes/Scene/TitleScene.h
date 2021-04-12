// �S���ꏊ
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
	// �V�[���J��
	void ChangeScene();
	// �J�ڒ����ǂ���
	bool isChanged_;
	// �L�������]
	bool flipFlg_;
	// �L�����ړ����x
	float runSpeed_;
};