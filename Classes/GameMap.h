#pragma once
#include "cocos2d.h"
#include <array>
#include <vector>
#include <memory>
#include "Direction/Direction.h"

using MapData = std::vector<cocos2d::TMXTiledMap*>;

struct MapChild
{
	int mapID;	// �q����mapID
	int nextParentID;		// ���̐eID
	MapDirection gateDir;	// �q���ɍs�����߂̃Q�[�g�̏ꏊ
	cocos2d::Vec2 nextPos;	// ���̃v���C���[���W
};
struct MapParentState
{
	int mapID;	// �e��mapID
	std::vector<MapChild> child;	// �q���ւ̃A�N�Z�X
};
struct MapParentList
{
	int nowID;	 // ���݂̐eID
	std::vector<MapParentState> mapParents;	// �e����
};
class Gate;
class Player;
class MapGenerator;
class GameMap
{
public:
	GameMap(cocos2d::Layer& layer);
	// �}�b�v���������� (�\�Ɨ��̓��ǂ� )
	void AddMap(std::string& mapPath);
	// �I�u�W�F�N�g���C����ǂݍ���Ŕz�u
	void CreateObject();
	cocos2d::TMXTiledMap* GetMap();
	void ReplaceMap(Player& player);
	void update(Player& pos);
	int GetNextChildID();
	cocos2d::Label* mapName;
private:
	// �p�X����}�b�v�ǂݍ��݂��s��
	cocos2d::TMXTiledMap* createMapFromPath(std::string& mapPath);
	
	cocos2d::Layer* mapLayer_;	// �}�b�v������V�[���̃��C���[
	cocos2d::Layer* objLayer_;	// �}�b�v�I�u�W�F�N�g�̃��C���[

	std::vector<std::string>pathList_;
	MapParentList mapParentsList_;
	
	MapData mapDatas_;	// 0�Ԃ��e
	std::vector<Gate*>objs_;
	int nextId;
	int frame_;
	std::array<cocos2d::Vec2, static_cast<int>(MapDirection::Max)>nextPosTbl;
	std::shared_ptr<MapGenerator>mapGenerator_;

	

};

