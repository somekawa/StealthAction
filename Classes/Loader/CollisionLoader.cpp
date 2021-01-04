#include <stdio.h>
#include <sstream>
#include <fstream>
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

void CollisionLoader::ReadData(std::unordered_map<std::string, std::vector<SharedRect>>& colliderBox, std::string pathName)
{
	std::string filePath = "Data/" + pathName + ".dat";

	std::ifstream inputData(filePath);
	std::stringstream strStream1;
	std::stringstream strStream2;
	std::string str1;
	std::string str2;

	char c = 0;
	bool readyToSet = false;
	do {
		std::getline(inputData, str1);
		if (str1.find("ActorName=") != -1)
		{
			strStream1.str(str1);
			do {
				std::getline(strStream1, str1, '/');
				if (str1.find("frameNum=") != -1)
				{
					std::getline(strStream1, str2, '/');

					int parentsize = std::stoi(str2);
					// colliderBox‚ÌØ»²½Ş
					colliderBox[pathName].resize(parentsize);
					readyToSet = true;
				}
			} while (!strStream1.eof());
		}

		auto num = -1;
		auto boxNum = 0;
		// attack‚ÌŒãdamage‚ª—ˆ‚é‚Ì‚Åattack‚ª—ˆ‚½‚çnext‚Í1‚É‚È‚é
		// damage‚¾‚¯‚Ìê‡‚Ínext‚Í0
		char next = 0;
		auto data = RectData({ 0,0 }, { 0,0 }, { 0,0 }, 0, 0);
		bool readCmp = false;
		strStream1.seekg(0);
		//std::getline(strStream1, str1);
		if (str1.find("RectData") != -1)
		{
			strStream2.str(str1);

			do {
				std::getline(strStream2, str1, '/');
				if (str1.find("rectNum=") != -1)
				{
					readCmp = false;
					if (next == 0)
					{
						num++;
						boxNum = 0;
					}
					std::getline(strStream2, str2, '/');

					int childSize = std::stoi(str2);
					// colliderBox‚ÌÌÚ°Ñ–ˆ‚ÌŠi”[”‚ÌØ»²½Ş
					colliderBox[pathName][num].resize(childSize);
				}
				if (str1.find("<damage>") != -1)
				{
					data.type_ = 1;
					readCmp = false;
					next = 0;
				}
				if (str1.find("<attack>") != -1)
				{
					data.type_ = 0;
					next = 1;
				}
				if (str1.find("begin=") != -1)
				{
					std::getline(strStream2, str2, ',');
					data.begin_.x = std::stoi(str2);
					std::getline(strStream2, str2, '/');
					data.begin_.y = std::stoi(str2);
				}
				if (str1.find("end=") != -1)
				{
					std::getline(strStream2, str2, ',');
					data.end_.x = std::stoi(str2);
					std::getline(strStream2, str2, '/');
					data.end_.y = std::stoi(str2);
				}
				if (str1.find("frame=") != -1)
				{
					std::getline(strStream2, str2, '/');
					data.frame_ = std::stoi(str2);
				}
				if (str1.find("size=") != -1)
				{
					std::getline(strStream2, str2, ',');
					data.size_.x = std::stoi(str2);
					std::getline(strStream2, str2, '/');
					data.size_.y = std::stoi(str2);
					readCmp = true;
				}

				if (readCmp)
				{
					colliderBox[pathName][num][boxNum] = 
						std::make_shared<ActionRect>(data.begin_, data.end_, data.type_, data.frame_);
					boxNum++;
				}
				if (num >= colliderBox[pathName].size()-1 &&
					boxNum >= colliderBox[pathName][num].size())
				{
					break;
				}
			} while (num <= colliderBox[pathName].size());
		}
	} while (!inputData.eof());


}

CollisionLoader::CollisionLoader()
{
}

CollisionLoader::~CollisionLoader()
{
}
