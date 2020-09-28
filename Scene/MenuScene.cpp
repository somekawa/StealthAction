#include <DxLib.h>
#include "TitleScene.h"
#include "GameScene.h"
#include "MenuScene.h"

#define PI 3.141592653589793f

// childSceneでGameSceneを入れておかないと、Menuに遷移した時点でGameSceneが破棄されるから
// GameSceneに戻ったときには、最初からになってしまう。

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
	// 画面遷移(GameSceneに戻るときならこう書く)
	// return std::move(childScene_);

	// active_のフラグがtrueなら、menuを開いている間もgamesceneは動き続ける
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
	// draw_のフラグがtrueなら、menuを開いている間もgamesceneの画面が描画される
	if (draw_)
	{
		childScene_->Draw();
	}

	DrawFormatString(0, 0, 0xffffff, "MenuScene");
}