#pragma once
#include <map>
#include <string>
#include "BaseScene.h"

class TitleScene :
	public BaseScene
{
public:
	TitleScene();
	~TitleScene();
	unique_Base Update(unique_Base own);	// unique_Base own : 自分のSceneポインタ
private:
	bool Init(void);						// 初期化
	void Draw(void);						// 描画
	const int screen_sizeX;					// 横画面サイズ
	const int screen_sizeY;					// 縦画面サイズ
};