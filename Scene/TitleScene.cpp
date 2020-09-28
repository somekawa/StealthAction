#include <DxLib.h>
#include <math.h>
#include "TitleScene.h"
#include "GameScene.h"

// doubleÇÕfloatÇÃ2î{ÇÃê∏ìxÇ™Ç†ÇÈÇ©ÇÁdoubleÇ∆Ç¢Ç§à”ñ°
#define PI 3.141592653589793f

TitleScene::TitleScene():screen_sizeX(1280), screen_sizeY(800)
{
	Init();
}

TitleScene::~TitleScene()
{
}

bool TitleScene::Init(void)
{
	return true;
}

unique_Base TitleScene::Update(unique_Base own)
{
	if (CheckHitKey(KEY_INPUT_SPACE) == 1)
	{
		return std::make_unique<GameScene>();
	}

	return std::move(own);
}

void TitleScene::Draw(void)
{
	DrawFormatString(0, 0, 0xffffff, "TitleScene");
}