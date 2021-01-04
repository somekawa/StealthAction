#pragma once
#include <vector>
#include <map>
#include <string>
#include <array>

#include "ActorType.h"
#include "Geometory.h"

using VecInt = std::vector<int>;

struct Resource
{
	// ©•ª‚ÌactorType‚Ì–¼‘O
	std::string typeName;
	std::map<std::string,int> actionNameSet;

	// ÌÚ°Ñ”
	int frame = 0;
	// ±ÆÒ°¼®İ‚ğ•¶š—ñ‚Å”»•Ê
	std::map<std::string,VecInt> imageHandle;
};

class ImageManager
{
public:

	static ImageManager& GetInstance(void)
	{
		static ImageManager s_instance;
		return s_instance;
	}

	bool Load(const ActorType& actor,std::string filepath,Vector2I divsize,Vector2I divcount);
	const VecInt& GetID(const ActorType& actor,std::string filepath);

	const std::map<std::string,int>& GetActionSet(ActorType actor)
	{
		return resources_[static_cast<int>(actor)].actionNameSet;
	}
	const std::string& GetAnimationName(ActorType actor)
	{
		return resources_[static_cast<int>(actor)].typeName;
	}
private:
	ImageManager() = default;
	ImageManager(const ImageManager&) = delete;
	void operator=(const ImageManager&) = delete;

	std::array<Resource, static_cast<int>(ActorType::Max)> resources_;
	void SetUp(ActorType actor);

};