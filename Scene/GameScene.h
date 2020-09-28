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
	// unique_Base own : ������Scene�|�C���^
	unique_Base Update(unique_Base own);
private:
	bool Init(void);					// ������
	void Draw(void);					// �`��

	std::unique_ptr<Player> player_;
};