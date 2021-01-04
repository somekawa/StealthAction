#pragma once
#include "KeyInput.h"
#include "Mouse.h"

// ‰æ–Ê»²½Þ
constexpr int Screen_x = 960;
constexpr int Screen_y = 560;

class Scene;

class Application
{
public:

	static Application& GetInstance(void)
	{
		static Application s_instance;
		return s_instance;
	}

	void Run(void);

	void Terminate(void);

private:
	Application() = default;

	Application(const Application&) = delete;
	void operator=(const Application&) = delete;

	bool SystemInit(void);

	KeyInput* keyInput_;
	Mouse* mouse_;
	Scene* scene_;
};

