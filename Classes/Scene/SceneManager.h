#pragma once
#include <array>
#include <cocos2d.h>
enum class SceneType
{
	Game,
	Load,
	Max
};

class BaseScene;
class SceneManager
{
public:
	static SceneManager& Instance()
	{
		static SceneManager sceneManager;
		return sceneManager;
	}

	// シーンをプッシュする	
	void PushScene(SceneType sceneType);

	// 今のシーンをポップする
	void PopScene();

	void ChangeScene(SceneType sceneType);
	~SceneManager() = default;
private:
	SceneManager();
	SceneManager(const SceneManager&) = delete;
	void operator=(const SceneManager&) = delete;

	std::array<cocos2d::Scene*, static_cast<int>(SceneType::Max)> scenes_;

};