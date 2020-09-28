#pragma once
#include <mutex>
#include <memory>	// スマートポインタ
#include <vector>
#include "../input/Controller.h"
#include "BaseScene.h"

#define lpSceneMng SceneMng::GetInstance() 

class SceneMng
{
public:
	static SceneMng& GetInstance()		// 自分自身だからインスタンスできる
	{
		// <フラグ, 管理したい関数>
		static std::once_flag once;
		std::call_once(once , Create);
		return (*s_Instance);			// 生成した実体もしくはすでに存在している実体をreturnする
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
		delete s_Instance;				// deleteだけだとnullptrにはならないから再びインスタンスしたいときにできなくなる
		s_Instance = nullptr;			// 絶対必要
	}

	bool Run();

private:
	bool SysInit(void);
	void Draw(void);

	const int screen_sizeX;				// 横画面サイズ
	const int screen_sizeY;				// 縦画面サイズ

	SceneMng();
	~SceneMng();
	static SceneMng* s_Instance;
	unique_Base activeScene_;			// activeのsceneを保持する
};