#include <DxLib.h>
#include "Application.h"
#include "Scene.h"

void Application::Run(void)
{
	if (!SystemInit())
	{
		return;
	}

	while (!CheckHitKey(KEY_INPUT_ESCAPE))
	{
		keyInput_->UpDate();
		mouse_->UpDate();
		scene_->UpDate((*keyInput_),(*mouse_));
	}
}

void Application::Terminate(void)
{
	DxLib_End();
}

bool Application::SystemInit(void)
{
	SetGraphMode(Screen_x, Screen_y, 32);
	ChangeWindowMode(true);
	SetWindowText("RectEdit");
	if (DxLib_Init() == 1)
	{
		return false;
	}
	scene_ = new Scene();
	keyInput_ = new KeyInput();
	mouse_ = new Mouse();

	mouse_->SetUp();

	return true;
}
