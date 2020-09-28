#pragma once
#include <memory>
#include "../common/VECTOR2.h"
#include "BaseScene.h"
#include "../Player.h"

class GameScene :
	public BaseScene
{
public:
	GameScene();
	~GameScene();
	// unique_Base own : 自分のSceneポインタ
	unique_Base Update(unique_Base own);
private:
	bool Init(void);					// 初期化
	void Draw(void);					// 描画

	std::unique_ptr<Player> player_;
};