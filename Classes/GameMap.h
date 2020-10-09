#pragma once
#include "cocos2d.h"

enum class MapType
{
	OMOTE,
	URA
};

struct MapState
{
	std::vector<std::string> path;
};

class GameMap
{
public:
	// マップ生成をする (表と裏の二つを読む )
	void CreateMap(cocos2d::Layer& layer, std::string omotePath, std::string uraPath = "");
	cocos2d::TMXTiledMap* GetMap();
	void ReplaceMap();
	void SetMapInfo(MapType mapType);
private:
	// パスからマップ読み込みを行う
	void createMapFromPath(std::string& path, cocos2d::Layer& layer);
	std::vector<cocos2d::TMXTiledMap*> maps_;	// マップ
	cocos2d::Layer* mapLayer_;	// マップがあるシーンのレイヤー
	cocos2d::TMXLayer* colisionLayer_;	// マップの当たり判定があるレイヤー
	MapType mapType_;
};

