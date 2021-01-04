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
	int id;
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
	bool AddSound(std::string soundName, std::string path, SoundType type);	// �T�E���h�ǉ�
	void PlayBySoundName(std::string soundName, float volume = 1.0f);								// �o�^��������ނ𖼑O����Đ�����
	void SetPauseAll(bool pauseF);			// �T�E���h��S�Ĉꎞ��~
	void Pause(std::string soundName);	// �T�E���h���ꎞ��~
	void Resume(std::string soundName);	// �T�E���h���ĊJ
private:
	SoundMng();
	std::map<std::string, Sound>soundList_;

	static std::unique_ptr<SoundMng> s_Instance;
};

