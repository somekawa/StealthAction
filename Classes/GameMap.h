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
	int ID;	// mapのID
	std::array<cocos2d::TMXTiledMap*, 2> own;
	cocos2d::Vec2 nextPos;
};

struct MapParentState
{
	int ID;	// mapのID
	std::array<cocos2d::TMXTiledMap*, 2> own;
	std::vector<MapChild> child;	// 子供へのアクセス
	MapType mapType;
};
struct MapParentList
{
	int nowID;	// 今のparentID
	std::vector<MapParentState> mapParents;	// 親たち
};
class Player;
class GameMap
{
public:
	GameMap(cocos2d::Layer& layer);
	// マップ生成をする (表と裏の二つを読む )
	void CreateMap(std::string omotePath, std::string uraPath = "");
	void AddNextMap( std::string omotePath, std::string uraPath = "");
	cocos2d::TMXTiledMap* GetMap();
	void ReplaceMap();
	void SetMapInfo(MapType mapType);
	void update(Player& pos);
private:
	// パスからマップ読み込みを行う
	std::array<cocos2d::TMXTiledMap*, 2> createMapFromPath(std::string& omotePath, std::string& uraPath);
	
	cocos2d::Layer* mapLayer_;	// マップがあるシーンのレイヤー
	cocos2d::TMXLayer* colisionLayer_;	// マップの当たり判定があるレイヤー
	std::vector<std::array<std::string, 2>>pathList_;
	MapParentList mapParentsList_;
	MapParentState mapParents;	// マップ
	//MapType mapType_;
	std::vector<std::array<cocos2d::TMXTiledMap*, 2>> mapDatas_;	// 0番が親
	int nodeIdx_;
};

