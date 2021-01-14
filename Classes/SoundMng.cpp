#include "SoundMng.h"
#include <AudioEngine.h>

USING_NS_CC;
std::unique_ptr<SoundMng>SoundMng::s_Instance(new SoundMng);
namespace
{
	int id;
}
SoundMng::SoundMng()
{
	Director::getInstance()->retain();
}

SoundMng::~SoundMng()
{
	AudioEngine::end();
	Director::getInstance()->release();
}

bool SoundMng::AddSound(std::string soundName, std::string path, SoundType type)
{
	
	if (soundList_.find(soundName) == soundList_.end())
	{
		soundList_.emplace(soundName, Sound(type, path));
	}
	else
	{
		log("登録重複してるよ");
		assert(true);
	}
	return true;
}

void SoundMng::PlayBySoundName(std::string soundName, float volume)
{
	if (soundList_.find(soundName) != soundList_.end())
	{
		bool loop = soundList_[soundName].type == SoundType::BGM;
		soundList_[soundName].id = AudioEngine::play2d(soundList_[soundName].path, loop, volume);
	}
	else
	{
		log("サウンド初期化できてないよ");
		assert(true);
	}
}

void SoundMng::SetPauseAll(bool pauseF)
{
	if (pauseF)
	{
		AudioEngine::pauseAll();
	}
	else
	{
		AudioEngine::resumeAll();
	}
}

void SoundMng::Pause(std::string soundName)
{
	if (soundList_.find(soundName) != soundList_.end())
	{
		AudioEngine::pause(soundList_[soundName].id);
	}
	else
	{
		log("そんなサウンドはありません");
		assert(true);
	}
}

void SoundMng::Resume(std::string soundName)
{
	if (soundList_.find(soundName) != soundList_.end())
	{
		AudioEngine::resume(soundList_[soundName].id);
	}
	else
	{
		log("そんなサウンドはありません");
		assert(true);
	}
}
