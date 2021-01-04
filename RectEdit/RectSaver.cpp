#include <stdio.h>
#include <iostream>
#include <sstream>
#include <fstream>

#include "RectSaver.h"

struct DataHeader
{
	// 1ｱﾆﾒｰｼｮﾝ当たりの総ﾌﾚｰﾑ数
	int frameNum_;
	// そのｱﾆﾒｰｼｮﾝの1ﾌﾚｰﾑ当たりの矩形数
	int rectNum_;

	int frame_;
};

bool RectSaver::SaveRect(const std::vector <SharedActionRect>& actionRect,
						 const std::string& animationName,
						 const std::string& actorName)
{
	DataHeader expData;
	int frame = 0;
	auto fileName = "Data/" + actorName + "_" + animationName + ".map";
	fopen_s(&file_, fileName.c_str(), "wb");
	expData.frameNum_ = actionRect.size();
	fwrite(&expData.frameNum_, sizeof(int), 1, file_);

	for (auto rects:actionRect)
	{
		expData.rectNum_= rects.size();
		fwrite(&expData.rectNum_, sizeof(int), 1, file_);
		for (auto r : rects)
		{
			fwrite(&r->GetData().begin_.x, sizeof(int), 1, file_);
			fwrite(&r->GetData().begin_.y, sizeof(int), 1, file_);
			fwrite(&r->GetData().end_.x, sizeof(int), 1, file_);
			fwrite(&r->GetData().end_.y, sizeof(int), 1, file_);
			fwrite(&r->GetData().size_.x, sizeof(int), 1, file_);
			fwrite(&r->GetData().size_.y, sizeof(int), 1, file_);
			fwrite(&r->GetData().frame_, sizeof(int), 1, file_);
			fwrite(&r->GetData().type_, sizeof(char), 1, file_);
		}
		frame++;
	}
	fclose(file_);

	// ﾃﾞﾊﾞｯｸﾞ用
	//FILE* fileSec_;
	//fopen_s(&fileSec_, fileName.c_str(), "rb");
	//int i = 0;
	//fread(&i, sizeof(int), 1, fileSec_);
	//fclose(fileSec_);

	return false;
}

bool RectSaver::WriteData(const std::vector<SharedActionRect>& actionRect, const std::string& animationName, const std::string& actorName)
{
	std::string fileName = "Data/" + actorName + "_" + animationName + ".dat";
	std::ofstream outFile(fileName);

	outFile << "<Infomation>";
	outFile << std::endl;
	// ｱｸﾀｰの名前出力
	outFile << "ActorName=";
	outFile << "/" + actorName + "/";

	// ｱﾆﾒｰｼｮﾝ名の出力
	outFile << "animationName=";
	outFile << "/" + animationName + "/";
	// ﾌﾚｰﾑ数の出力
	outFile << "frameNum=";
	outFile << "/" + std::to_string(actionRect.size()) + "/";
	outFile << std::endl;

	outFile << "<RectData>";
	auto firstRead = true;
	//for (int frame = 0; frame < actionRect.size(); frame++)
	{
		firstRead = true;
		//outFile << std::endl;
		for (auto rect : actionRect)
		{
			outFile << "rectNum=/" + std::to_string(rect.size())+"/";
			for (auto r : rect)
			{
				if (r->GetData().type_ == 0)
				{
					outFile << "<attack>";
					outFile << " begin=/" + std::to_string(r->GetData().begin_.x) + "," + std::to_string(r->GetData().begin_.y) + "/";
					outFile << " end=/" + std::to_string(r->GetData().end_.x) + "," + std::to_string(r->GetData().end_.y) + "/";
					outFile << " frame=/" + std::to_string(r->GetData().frame_) + "/";
					outFile << " size=/" + std::to_string(r->GetData().size_.x) + "," + std::to_string(r->GetData().size_.y) + "/";
					//outFile << std::endl;
				}
				else
				{
					outFile << "<damage>";
					outFile << " begin=/" + std::to_string(r->GetData().begin_.x) + "," + std::to_string(r->GetData().begin_.y) + "/";
					outFile << " end=/" + std::to_string(r->GetData().end_.x) + "," + std::to_string(r->GetData().end_.y) + "/";
					outFile << " frame=/" + std::to_string(r->GetData().frame_) + "/";
					outFile << " size=/" + std::to_string(r->GetData().size_.x) + "," + std::to_string(r->GetData().size_.y) + "/";
					//outFile << std::endl;
				}
			}
		}
	}

	return false;
}

bool RectSaver::LoadRect(std::vector <SharedActionRect>& actionRect,
	const std::string& animationName,
	const std::string& actorName)
{
	std::vector <SharedActionRect> loadRect;
	FILE* f_;
	auto fileName = "Data/player_attack1.map";
	fopen_s(&f_, fileName, "rb");

	DataHeader expData;
	fread(&expData.frameNum_, sizeof(int), 1, f_);
	actionRect.resize(expData.frameNum_);
	for (auto& rects : actionRect)
	{
		fread(&expData.rectNum_, sizeof(int), 1, f_);
		rects.resize(expData.rectNum_);
		for (int i = 0;i < rects.size();i++)
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
			rects[i] = std::make_shared<ActionRect>(data.begin_, data.end_,Vector2I(0,0), data.type_,data.frame_);
		}
	}
	//frame++;
	fclose(f_);


	return false;
}
