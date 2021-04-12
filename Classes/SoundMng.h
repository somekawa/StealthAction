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
	SoundType type;		// サウンドの種類
	std::string path;	// パス
	int id;				// サウンドID
	Sound() {};
	Sound(SoundType t, std::string p):type(t), path(p), id(-1){};
};

class SoundMng
{
public:
	static SoundMng & GetInstance(void)
	{
		return *s_Instance;
	}
	~SoundMng();
	bool AddSound(std::string soundName, std::string path, SoundType type);	// サウンド追加
	void PlayBySoundName(std::string soundName, float volume = 1.0f);		// 登録したサウンドを名前から再生する
	void SetPauseAll(bool pauseF);											// サウンドを全て一時停止
	void Pause(std::string soundName);										// サウンドを一時停止
	void Resume(std::string soundName);										// サウンドを再開
private:
	SoundMng();
	std::map<std::string, Sound>soundList_;

	static std::unique_ptr<SoundMng> s_Instance;
};