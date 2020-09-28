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
	bool Init(void);					// ������
	void Draw(void);					// �`��
	unique_Base childScene_;			// MenuScene�ɐ؂�ւ��O��Scene����ۑ�
	int draw_;							// true�Ȃ�childScene���`�悳���
	int active_;						// true�Ȃ�childScene������������

	const int screen_sizeX;				// ����ʃT�C�Y
	const int screen_sizeY;				// �c��ʃT�C�Y
};