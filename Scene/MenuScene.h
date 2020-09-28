#pragma once
#include "BaseScene.h"
#include <map>
#include <string>

class MenuScene :
	public BaseScene
{
public:
	MenuScene(unique_Base childScene,int draw,int active);
	~MenuScene();
	unique_Base Update(unique_Base own);
private:
	bool Init(void);					// 初期化
	void Draw(void);					// 描画
	unique_Base childScene_;			// MenuSceneに切り替わる前のScene情報を保存
	int draw_;							// trueならchildSceneが描画される
	int active_;						// trueならchildSceneが動き続ける

	const int screen_sizeX;				// 横画面サイズ
	const int screen_sizeY;				// 縦画面サイズ
};