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
	// unique_Base own : ©•ª‚ÌSceneƒ|ƒCƒ“ƒ^
	unique_Base Update(unique_Base own);
private:
	bool Init(void);					// ‰Šú‰»
	void Draw(void);					// •`‰æ

	std::unique_ptr<Player> player_;
};