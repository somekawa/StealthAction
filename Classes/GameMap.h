#pragma once
#include "cocos2d.h"
#include <array>
#include <vector>

enum class MapType
{
	OMOTE,
	URA
};

struct MapState
{
	std::array<cocos2d::TMXTiledMap*, 2> own;
	std::vector<MapState> child;
	//MapState() : own({ nullptr, nullptr }), child({}) {};
};

class GameMap
{
public:
	// �}�b�v���������� (�\�Ɨ��̓��ǂ� )
	void CreateMap(cocos2d::Layer& layer, std::string omotePath, std::string uraPath = "");
	void AddNextMap(cocos2d::Layer& layer, std::string omotePath, std::string uraPath = "");
	cocos2d::TMXTiledMap* GetMap();
	void ReplaceMap();
	void SetMapInfo(MapType mapType);
	void update(cocos2d::Vec2 pos);
private:
	// �p�X����}�b�v�ǂݍ��݂��s��
	std::array<cocos2d::TMXTiledMap*, 2> createMapFromPath(std::string& omotePath, std::string& uraPath, cocos2d::Layer& layer);
	MapState maps_;	// �}�b�v
	cocos2d::Layer* mapLayer_;	// �}�b�v������V�[���̃��C���[
	cocos2d::TMXLayer* colisionLayer_;	// �}�b�v�̓����蔻�肪���郌�C���[
	MapType mapType_;
	int nodeIdx_;
};

