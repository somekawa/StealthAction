#pragma once
#include "cocos2d.h"
#include <array>
#include <vector>

enum class MapType
{
	OMOTE,
	URA
};
struct MapChild
{
	int ID;	// map��ID
	std::array<cocos2d::TMXTiledMap*, 2> own;
	cocos2d::Vec2 nextPos;
};

struct MapParentState
{
	int ID;	// map��ID
	std::array<cocos2d::TMXTiledMap*, 2> own;
	std::vector<MapChild> child;	// �q���ւ̃A�N�Z�X
	MapType mapType;
};
struct MapParentList
{
	int nowID;	// ����parentID
	std::vector<MapParentState> mapParents;	// �e����
};
class Player;
class GameMap
{
public:
	GameMap(cocos2d::Layer& layer);
	// �}�b�v���������� (�\�Ɨ��̓��ǂ� )
	void CreateMap(std::string omotePath, std::string uraPath = "");
	void AddNextMap( std::string omotePath, std::string uraPath = "");
	cocos2d::TMXTiledMap* GetMap();
	void ReplaceMap();
	void SetMapInfo(MapType mapType);
	void update(Player& pos);
private:
	// �p�X����}�b�v�ǂݍ��݂��s��
	std::array<cocos2d::TMXTiledMap*, 2> createMapFromPath(std::string& omotePath, std::string& uraPath);
	
	cocos2d::Layer* mapLayer_;	// �}�b�v������V�[���̃��C���[
	cocos2d::TMXLayer* colisionLayer_;	// �}�b�v�̓����蔻�肪���郌�C���[
	std::vector<std::array<std::string, 2>>pathList_;
	MapParentList mapParentsList_;
	MapParentState mapParents;	// �}�b�v
	//MapType mapType_;
	std::vector<std::array<cocos2d::TMXTiledMap*, 2>> mapDatas_;	// 0�Ԃ��e
	int nodeIdx_;
};

