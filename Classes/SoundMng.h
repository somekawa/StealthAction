#pragma once
#include <cocos2d.h>

#define lpSoundMng SoundMng::GetInstance()

enum class SoundType
{
	SE,
	BGM
};

struct Sound
{
	SoundType type;
	std::string path;
	Sound() {};
	Sound(SoundType t, std::string p):type(t), path(p){};
};

class SoundMng
{
public:
	static SoundMng & GetInstance(void)
	{
		return *s_Instance;
	}
	~SoundMng();
	bool AddSound(std::string soundName, std::string path, SoundType type);	// ｻｳﾝﾄﾞ追加
	void PlayBySoundName(std::string soundName, float volume = 1.0f);								// 登録したｻｳﾝﾄﾞを名前から再生する
	void SetPauseAll(bool pauseF);												// ｻｳﾝﾄﾞを全て一時停止

private:
	SoundMng();
	std::map<std::string, Sound>soundList_;

	static std::unique_ptr<SoundMng> s_Instance;
};

