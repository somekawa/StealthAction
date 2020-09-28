#include <DxLib.h>
#include "TitleScene.h"
#include "GameScene.h"
#include "MenuScene.h"

#define PI 3.141592653589793f

// childScene��GameScene�����Ă����Ȃ��ƁAMenu�ɑJ�ڂ������_��GameScene���j������邩��
// GameScene�ɖ߂����Ƃ��ɂ́A�ŏ�����ɂȂ��Ă��܂��B

MenuScene::MenuScene(unique_Base childScene, int draw, int active):screen_sizeX(1280), screen_sizeY(800)
{
	childScene_ = std::move(childScene);
	draw_ = draw;
	active_ = active;
	Init();
}

MenuScene::~MenuScene()
{
}

bool MenuScene::Init(void)
{
	return true;
}

unique_Base MenuScene::Update(unique_Base own)
{
	// ��ʑJ��(GameScene�ɖ߂�Ƃ��Ȃ炱������)
	// return std::move(childScene_);

	// active_�̃t���O��true�Ȃ�Amenu���J���Ă���Ԃ�gamescene�͓���������
	if (active_)
	{
		childScene_ = childScene_->Update(std::move(childScene_));
	}

	if (CheckHitKey(KEY_INPUT_F2) == 1)
	{
		return std::move(childScene_);
	}

	return std::move(own);
}

void MenuScene::Draw(void)
{
	// draw_�̃t���O��true�Ȃ�Amenu���J���Ă���Ԃ�gamescene�̉�ʂ��`�悳���
	if (draw_)
	{
		childScene_->Draw();
	}

	DrawFormatString(0, 0, 0xffffff, "MenuScene");
}