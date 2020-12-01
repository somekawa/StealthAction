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

	// �V�[�����v�b�V������	
	void PushScene(SceneType sceneType);

	// ���̃V�[�����|�b�v����
	void PopScene();

	void ChangeScene(SceneType sceneType);
	~SceneManager() = default;
private:
	SceneManager();
	SceneManager(const SceneManager&) = delete;
	void operator=(const SceneManager&) = delete;

	std::array<cocos2d::Scene*, static_cast<int>(SceneType::Max)> scenes_;

};