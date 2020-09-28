#include <DxLib.h>
#include "GameScene.h"
#include "MenuScene.h"

#define PI 3.141592653589793f

GameScene::GameScene()
{
	Init();
}

GameScene::~GameScene()
{
}

bool GameScene::Init(void)
{
	player_ = std::make_unique<Player>();
	return true;
}

unique_Base GameScene::Update(unique_Base own)
{
	player_->Update();

	// メニュー画面への切替
	if (CheckHitKey(KEY_INPUT_F1) == 1)
	{
		// 引数として、GameSceneの現在の状態と、画面をどうするかのフラグを入れる
		return std::make_unique<MenuScene>(std::move(own), false, false);
	}

	return std::move(own);
}

void GameScene::Draw(void)
{
	DrawFormatString(0, 0, 0xffffff, "GameScene");
	player_->Draw();
}