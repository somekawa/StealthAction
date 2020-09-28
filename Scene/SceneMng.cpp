#include <DxLib.h>
#include "../input/KeyBoard.h"
#include "SceneMng.h"
#include "TitleScene.h"

SceneMng* SceneMng::s_Instance = nullptr;

bool SceneMng::Run()
{
	if (!SysInit())
	{
		return false;
	}

	//----------ループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{		
		activeScene_ = activeScene_->Update(std::move(activeScene_));
		Draw();
	}

	return true;
}

bool SceneMng::SysInit(void)
{
	// システム処理
	SetGraphMode(screen_sizeX, screen_sizeY, 16);	// 2人分サイズ
	ChangeWindowMode(true);
	SetWindowText("Prototype");

	if (DxLib_Init() == -1)
	{
		return false;
	}

	//SetDrawZ(0.0f)を使いたいならこの下の2つを設定しておくこと
	//SetUseZBufferFlag(TRUE);
	//SetWriteZBufferFlag(TRUE);

	activeScene_ = std::make_unique<TitleScene>();
	return true;
}

void SceneMng::Draw(void)
{
	SetDrawScreen(DX_SCREEN_BACK);
	ClsDrawScreen();

	activeScene_->Draw();

	ScreenFlip();
}

SceneMng::SceneMng() : 
	screen_sizeX(1280),screen_sizeY(800)
{
}

SceneMng::~SceneMng()
{
}