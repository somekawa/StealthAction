#pragma once
#include <mutex>
#include <memory>	// �X�}�[�g�|�C���^

template <class T>
class Singleton
{
public:
	static T& GetInstance()		// �������g������C���X�^���X�ł���
	{
		// <�t���O, �Ǘ��������֐�>
		static std::once_flag once;
		std::call_once(once, Create);
		//if (s_Instance == nullptr)		// �܂����̂��Ȃ��Ƃ�
		//{
			//s_Instance = new SceneMng();	// ���̂����
		//}
		return (*s_Instance);			// �����������̂������͂��łɑ��݂��Ă�����̂�return����
	}

	static void Create()
	{
		if (s_Instance == nullptr)
		{
			s_Instance = new T;
		}
	}

	static void Destroy()
	{
		delete s_Instance;				// delete��������nullptr�ɂ͂Ȃ�Ȃ�����ĂуC���X�^���X�������Ƃ��ɂł��Ȃ��Ȃ�
		s_Instance = nullptr;			// ��ΕK�v
	}

private:
	//Singleton();
	//~Singleton();
	static T* s_Instance;
};

#include "./details/Singleton.h"