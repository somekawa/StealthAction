#pragma once
#include <mutex>
#include <memory>	// �X�}�[�g�|�C���^
#include <vector>
#include "../input/Controller.h"
#include "BaseScene.h"

#define lpSceneMng SceneMng::GetInstance() 

class SceneMng
{
public:
	static SceneMng& GetInstance()		// �������g������C���X�^���X�ł���
	{
		// <�t���O, �Ǘ��������֐�>
		static std::once_flag once;
		std::call_once(once , Create);
		return (*s_Instance);			// �����������̂������͂��łɑ��݂��Ă�����̂�return����
	}

	static void Create()
	{
		if (s_Instance == nullptr)		
		{
			s_Instance = new SceneMng();	
		}
	}

	static void Destroy()
	{
		delete s_Instance;				// delete��������nullptr�ɂ͂Ȃ�Ȃ�����ĂуC���X�^���X�������Ƃ��ɂł��Ȃ��Ȃ�
		s_Instance = nullptr;			// ��ΕK�v
	}

	bool Run();

private:
	bool SysInit(void);
	void Draw(void);

	const int screen_sizeX;				// ����ʃT�C�Y
	const int screen_sizeY;				// �c��ʃT�C�Y

	SceneMng();
	~SceneMng();
	static SceneMng* s_Instance;
	unique_Base activeScene_;			// active��scene��ێ�����
};