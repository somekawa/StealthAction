#include <cocos2d.h>
#include <stdio.h>
#include <sstream>
#include <fstream>
#include "ActionRect.h"
#include "CollisionLoader.h"
#include "_Debug/_DebugConOut.h"

USING_NS_CC;

struct DataHeader
{
	// 1アニメーション当たりの総フレーム数
	int frameNum_;
	// そのアニメーションの1フレーム当たりの矩形数
	int rectNum_;

	int frame_;
};

CollisionLoader::CollisionLoader()
{
}

CollisionLoader::~CollisionLoader()
{
}

void CollisionLoader::ReadData(std::unordered_map<std::string, std::vector<SharedRect>>& colliderBox, std::string pathName)
{
	const std::string filePath = "Data/" + pathName + ".dat";
	const auto fileUtiles = FileUtils::getInstance();
	const std::string file = fileUtiles->getStringFromFile(filePath);
	std::stringstream inputData(file);
	std::stringstream strStream1;
	std::stringstream strStream2;
	std::string str1;
	std::string str2;

	bool readyToSet = false;
	do {
		if (!std::getline(inputData, str1))
		{
			cocos2d::log("%s 読み込めてない\n", filePath.c_str());
		}
		if (str1.find("ActorName=") != -1)
		{
			strStream1.str(str1);
			do {
				std::getline(strStream1, str1, '/');
				if (str1.find("frameNum=") != -1)
				{
					std::getline(strStream1, str2, '/');

					int parentsize = std::stoi(str2);
					// colliderBoxのリサイズ
					colliderBox[pathName].resize(parentsize);
					readyToSet = true;
				}
			} while (!strStream1.eof());
		}
		int num = -1;
		unsigned int boxNum = 0;
		// attackの後damageが来るのでattackが来たらnextは1になる
		// damageだけの場合はnextは0
		char next = 0;
		auto data = RectData({ 0,0 }, { 0,0 }, { 0,0 }, 0, 0);
		bool readCmp = false;
		strStream1.seekg(0);
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
					// colliderBoxのフレーム毎の格納数のリサイズ
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
						std::make_shared<ActionRect>(data.begin_, data.end_,data.type_, data.frame_);
					boxNum++;
				}
				if (num    >= colliderBox[pathName].size() - 1 &&
					boxNum >= colliderBox[pathName][num].size())
				{
					break;
				}
			} while (static_cast<unsigned>(num) <= colliderBox[pathName].size());
		}
	} while (!inputData.eof());
}