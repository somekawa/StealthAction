#pragma once
#include "cocos2d.h"
#include <array>
#include <vector>
#include <memory>
#include "Direction/Direction.h"

using MapData = std::vector<cocos2d::TMXTiledMap*>;

// 子供ステータス
struct MapChild
{
	int mapID;	// 子供のmapID
	int nextParentID;		// 次の親ID
	MapDirection gateDir;	// 子供に行くためのゲートの場所
	cocos2d::Vec2 nextPos;	// 次のプレイヤー座標
};

// 親のステータス
struct MapParentState
{
	int mapID;	// 親のmapID
	std::vector<MapChild> child;	// 子供へのアクセス
	int enemyNum;	// 敵の数
	bool isArrival;
};
// 親リスト
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

	// マップをロード開始
	void LoadMap(Player& player);
	// 今のマップをロードした次のマップに切り替え(プレイヤーのポジション)
	void ReplaceMap(Player& player);
	void update(Player& player);

	// 今のマップを取得
	cocos2d::TMXTiledMap* GetNowMap();
	// 今のマップID取得
	int GetNowID();
	// エネミーの数
	int GetEnemyNum();
	// マップジェネレータ取得
	MapGenerator& GetMapGenerator();
	// フロア変更のフラグ取得
	const bool ChangeFloor(void);

private:
	// パスからマップ読み込みを行う
	cocos2d::TMXTiledMap* createMapFromPath(std::string& mapPath);
	
	cocos2d::Layer* mapLayer_;	// マップがあるシーンのレイヤー
	cocos2d::Layer* objLayer_;	// マップオブジェクトのレイヤー

	std::vector<std::string>pathList_;
	MapParentList mapParentsList_;
	
	MapData mapDatas_;	// 0番が親
	std::vector<Gate*>objs_; // 現在はゲートのみ
	int nextId_;
	int frame_;
	std::array<cocos2d::Vec2, static_cast<int>(MapDirection::Max)>nextPosTbl;
	std::shared_ptr<MapGenerator>mapGenerator_;

	// デバッグ用
#ifdef _DEBUG
	cocos2d::Label* mapName_;
#endif // _DEBUG

	// フロア変更したときにtrueになる
	bool isChangeFloor_;
};

