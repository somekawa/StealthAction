#pragma once
#include "cocos2d.h"
#include <array>
#include <memory>

enum class CameraType
{
	UI,
	PLAYER1,
	PLAYER2,
	MAX
};
class CameraManager
{
public:
	CameraManager() = default;
	~CameraManager() = default;

	// カメラを追加する
	void AddCamera(cocos2d::Scene& scene,  cocos2d::Size zoomSize, CameraType cameraType, cocos2d::CameraFlag cameraFlag);
	//カメラをposを中心にズームする
	void ZoomCamera(cocos2d::Vec2 pos, float scale, CameraType cameraType);	
	// カメラを移動する(通常ステージでは使わなくてボス部屋のみ)
	void ScrollCamera(cocos2d::Vec2 pos, CameraType cameraType);
	
private:
	// カメラのポジションをマップ外が見えないように移動させる
	void NotOutRangeSetPos(cocos2d::Vec2& pos, CameraType cameraType, float scale = 1.0f);
	std::array<cocos2d::Camera*,static_cast<int>(CameraType::MAX)>cameras_; // カメラ格納用
};

