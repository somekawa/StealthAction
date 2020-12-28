#include <stdio.h>
#include "ActionRect.h"
#include "CollisionLoader.h"

USING_NS_CC;

struct DataHeader
{
	// 1±ÆÒ°¼®İ“–‚½‚è‚Ì‘ÌÚ°Ñ”
	int frameNum_;
	// ‚»‚Ì±ÆÒ°¼®İ‚Ì1ÌÚ°Ñ“–‚½‚è‚Ì‹éŒ`”
	int rectNum_;

	int frame_;
};

void CollisionLoader::Load(std::unordered_map<std::string, std::vector<SharedRect>>& colliderBox,
						   std::string pathName)
{
	FILE* f_;
	auto fileName = "Data/" + pathName + ".map";
	fopen_s(&f_, fileName.c_str(), "rb");

	DataHeader expData;
	fread(&expData.frameNum_, sizeof(int), 1, f_);
	colliderBox[pathName].resize(expData.frameNum_);
	for (auto& rects : colliderBox[pathName])
	{
		fread(&expData.rectNum_, sizeof(int), 1, f_);
		rects.resize(expData.rectNum_);
		for (int i = 0; i < rects.size(); i++)
		{
			auto data = RectData({ 0,0 }, { 0,0 }, { 0,0 }, 0, 0);
			fread(&data.begin_.x, sizeof(int), 1, f_);
			fread(&data.begin_.y, sizeof(int), 1, f_);
			fread(&data.end_.x, sizeof(int), 1, f_);
			fread(&data.end_.y, sizeof(int), 1, f_);
			fread(&data.size_.x, sizeof(int), 1, f_);
			fread(&data.size_.y, sizeof(int), 1, f_);
			fread(&data.frame_, sizeof(int), 1, f_);
			fread(&data.type_, sizeof(char), 1, f_);
			rects[i] = std::make_shared<ActionRect>(data.begin_, data.end_,data.type_, data.frame_);
		}
	}
	fclose(f_);
}

CollisionLoader::CollisionLoader()
{
}

CollisionLoader::~CollisionLoader()
{
}
