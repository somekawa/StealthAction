#pragma once
#include "cocos2d.h"
#include <array>
#include <vector>
#include <memory>
#include "Direction/Direction.h"

using MapData = std::vector<cocos2d::TMXTiledMap*>;

struct MapChild
{
	int mapID;	// 子供のmapID
	int nextParentID;		// 次の親ID
	MapDirection gateDir;	// 子供に行くためのゲートの場所
	cocos2d::Vec2 nextPos;	// 次のプレイヤー座標
};
struct MapParentState
{
	int mapID;	// 親のmapID
	std::vector<MapChild> child;	// 子供へのアクセス
};
struct MapParentList
{
	int nowID;	 // 現在の親ID
	std::vector<MapParentState> mapParents;	// 親たち
};
class Gate;
class Player;
class MapGenerator;
class GameMap
{
public:
	GameMap(cocos2d::Layer& layer);
	// マップ生成をする (表と裏の二つを読む )
	void AddMap(std::string& mapPath);
	// オブジェクトレイヤを読み込んで配置
	void CreateObject();
	cocos2d::TMXTiledMap* GetMap();
	void ReplaceMap(Player& player);
	void update(Player& pos);
	int GetNextChildID();
	cocos2d::Label* mapName;
private:
	// パスからマップ読み込みを行う
	cocos2d::TMXTiledMap* createMapFromPath(std::string& mapPath);
	
	cocos2d::Layer* mapLayer_;	// マップがあるシーンのレイヤー
	cocos2d::Layer* objLayer_;	// マップオブジェクトのレイヤー

	std::vector<std::string>pathList_;
	MapParentList mapParentsList_;
	
	MapData mapDatas_;	// 0番が親
	std::vector<Gate*>objs_;
	int nextId;
	int frame_;
	std::array<cocos2d::Vec2, static_cast<int>(MapDirection::Max)>nextPosTbl;
	std::shared_ptr<MapGenerator>mapGenerator_;

	

};

