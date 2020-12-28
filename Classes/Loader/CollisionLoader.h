#pragma once
#include <memory>
#include <array>
#include <string>
#include <vector>
#include <unordered_map>

#include "obj/Actor.h"
#include "scene/GameScene.h"

class ActionRect;

#define lpCol CollisionLoader::Instance()

using VecCol = std::vector<std::shared_ptr<ActionRect>>;
using ColliderData = std::unordered_map<std::string,VecCol>;

class CollisionLoader
{
public:
	static CollisionLoader& Instance(void)
	{
		static CollisionLoader sInstance_;
		return sInstance_;
	}

	void Load(std::unordered_map<std::string,std::vector<SharedRect>>& colliderBox,
			  std::string pathName);

private:
	CollisionLoader();
	~CollisionLoader();

	FILE* headerFile_;
	FILE* rawFile_;
};