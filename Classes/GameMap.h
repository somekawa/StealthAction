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
	// マップ生成をする (表と裏の二つを読む )
	void CreateMap(cocos2d::Layer& layer, std::string omotePath, std::string uraPath = "");
	void AddNextMap(cocos2d::Layer& layer, std::string omotePath, std::string uraPath = "");
	cocos2d::TMXTiledMap* GetMap();
	void ReplaceMap();
	void SetMapInfo(MapType mapType);
	void update(cocos2d::Vec2 pos);
private:
	// パスからマップ読み込みを行う
	std::array<cocos2d::TMXTiledMap*, 2> createMapFromPath(std::string& omotePath, std::string& uraPath, cocos2d::Layer& layer);
	MapState maps_;	// マップ
	cocos2d::Layer* mapLayer_;	// マップがあるシーンのレイヤー
	cocos2d::TMXLayer* colisionLayer_;	// マップの当たり判定があるレイヤー
	MapType mapType_;
	int nodeIdx_;
};

