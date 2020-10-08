#pragma once
#include "cocos2d.h"

struct MapState
{
	std::vector<std::string> path;
};

class GameMap
{
public:
	// �}�b�v���������� (�\�Ɨ��̓��ǂ� )
	void CreateMap(cocos2d::Layer& layer, std::string omotePath, std::string uraPath = "");
	
	void ReplaceMap();
private:
	// �p�X����}�b�v�ǂݍ��݂��s��
	void createMapFromPath(std::string& path, cocos2d::Layer& layer);
	std::vector<cocos2d::TMXTiledMap> maps_;	// �}�b�v
	cocos2d::Layer* mapLayer_;	// �}�b�v������V�[���̃��C���[
	cocos2d::TMXLayer* colisionLayer_;	// �}�b�v�̓����蔻�肪���郌�C���[
};

