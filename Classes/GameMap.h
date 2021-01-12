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
	int enemyNum;
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
	// ���̃}�b�v���擾
	cocos2d::TMXTiledMap* GetNowMap();
	// �}�b�v�����[�h�J�n
	void LoadMap(Player& player);
	// ���̃}�b�v�����[�h�������̃}�b�v�ɐ؂�ւ�(�v���C���[�̃|�W�V����)
	void ReplaceMap(Player& player);
	void update(Player& player);
	// ���̃}�b�vID�擾
	int GetNextChildID();
	// �G�l�~�[�̐�
	int GetEnemyNum();
	// �۱�ύX���׸ގ擾
	const bool ChangeFloor(void)
	{
		return isChangeFloor_;
	}
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

	// �۱�ύX�����Ƃ���true�ɂȂ�
	bool isChangeFloor_;
};

