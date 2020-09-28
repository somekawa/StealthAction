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
	unique_Base Update(unique_Base own);	// unique_Base own : ������Scene�|�C���^
private:
	bool Init(void);						// ������
	void Draw(void);						// �`��
	const int screen_sizeX;					// ����ʃT�C�Y
	const int screen_sizeY;					// �c��ʃT�C�Y
};