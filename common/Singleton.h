#pragma once
#include <mutex>
#include <memory>	// スマートポインタ

template <class T>
class Singleton
{
public:
	static T& GetInstance()		// 自分自身だからインスタンスできる
	{
		// <フラグ, 管理したい関数>
		static std::once_flag once;
		std::call_once(once, Create);
		//if (s_Instance == nullptr)		// まだ実体がないとき
		//{
			//s_Instance = new SceneMng();	// 実体を作る
		//}
		return (*s_Instance);			// 生成した実体もしくはすでに存在している実体をreturnする
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
		delete s_Instance;				// deleteだけだとnullptrにはならないから再びインスタンスしたいときにできなくなる
		s_Instance = nullptr;			// 絶対必要
	}

private:
	//Singleton();
	//~Singleton();
	static T* s_Instance;
};

#include "./details/Singleton.h"