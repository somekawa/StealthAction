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

	// ���j���[��ʂւ̐ؑ�
	if (CheckHitKey(KEY_INPUT_F1) == 1)
	{
		// �����Ƃ��āAGameScene�̌��݂̏�ԂƁA��ʂ��ǂ����邩�̃t���O������
		return std::make_unique<MenuScene>(std::move(own), false, false);
	}

	return std::move(own);
}

void GameScene::Draw(void)
{
	DrawFormatString(0, 0, 0xffffff, "GameScene");
	player_->Draw();
}