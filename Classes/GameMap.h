#pragma once
#include "cocos2d.h"
#include <array>
#include <vector>
#include <memory>
#include "Direction/Direction.h"

using MapData = std::vector<std::array<cocos2d::TMXTiledMap*, 2>>;

enum class MapType
{
	OMOTE,
	URA
};
struct MapChild
{
	int mapID;	// mapのID
	int nextParentID;
	cocos2d::Vec2 nextPos;
};
struct MapParentState
{
	int mapID;	// mapのID
	std::vector<MapChild> child;	// 子供へのアクセス
	MapType mapType;
};
struct MapParentList
{
	int nowID;	 // 現在の親ID
	std::vector<MapParentState> mapParents;	// 親たち
};
class Gate;
class Player;
class GameMap
{
public:
	GameMap(cocos2d::Layer& layer);
	// マップ生成をする (表と裏の二つを読む )
	void AddMap(std::string omotePath, std::string uraPath = "");
	// オブジェクトレイヤを読み込んで配置
	void CreateObject();
	cocos2d::TMXTiledMap* GetMap();
	void ReplaceMap(Player& player);
	void SetMapInfo(MapType mapType);
	void update(Player& pos);
	int GetNextChildID();
private:
	// パスからマップ読み込みを行う
	std::array<cocos2d::TMXTiledMap*, 2> createMapFromPath(std::string& omotePath, std::string& uraPath);
	
	cocos2d::Layer* mapLayer_;	// マップがあるシーンのレイヤー
	cocos2d::Layer* objLayer_;	// マップがあるシーンのレイヤー
	cocos2d::TMXLayer* colisionLayer_;	// マップの当たり判定があるレイヤー

	std::vector<std::array<std::string, 2>>pathList_;
	MapParentList mapParentsList_;
	
	//MapType mapType_;
	MapData mapDatas_;	// 0番が親
	int nowIdx;
	std::vector<Gate*>objs_;
	int nextId;
};

