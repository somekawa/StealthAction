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
	
	if (soundList_.find(soundName) != soundList_.end())
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
		AudioEngine::play2d(soundList_[soundName].path, true, 1.0);
	}
	else
	{
		log("サウンド初期化できてないよ");
		assert(true);
	}
}