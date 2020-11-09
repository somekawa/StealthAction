#include "CameraManager.h"
#include "Scene/GameScene.h"

USING_NS_CC;

void CameraManager::AddCamera(cocos2d::Scene& scene, cocos2d::Size zoomSize, CameraType cameraType, cocos2d::CameraFlag cameraFlag)
{
	auto defaultcamera = scene.getDefaultCamera();
	auto camera = Camera::createOrthographic(zoomSize.width, zoomSize.height, defaultcamera->getNearPlane() - 768, defaultcamera->getFarPlane());
	camera->setName(StringUtils::toString(static_cast<int>(cameraType)));
	camera->setCameraFlag(cameraFlag);
	scene.addChild(camera);
	cameras_[static_cast<int>(cameraType)] = camera;
}


void CameraManager::NotOutRangeSetPos(cocos2d::Vec2& pos,CameraType cameraType, float scale)
{
	auto director = Director::getInstance();
	auto map = (TMXTiledMap*)director->getRunningScene()->getChildByTag((int)zOlder::BG)->getChildByName("MapData");
	if (map == nullptr)
	{
		return;
	}
	auto size = director->getWinSize();
	float mapMoveX = cameras_[static_cast<int>(cameraType)]->getPositionX();
	float mapMoveY = cameras_[static_cast<int>(cameraType)]->getPositionY();
	if (pos.x > size.width / 2 * scale && pos.x < map->getContentSize().width - size.width / 2 * scale)
	{
		mapMoveX = pos.x - size.width / 2 * scale;
	}
	if (pos.y > size.height / 2 * scale && pos.y < map->getContentSize().height - size.height / 2 * scale)
	{
		mapMoveY = pos.y - size.height / 2 * scale;
	}
	cameras_[static_cast<int>(cameraType)]->setPosition(mapMoveX, mapMoveY);
}

void CameraManager::ZoomCamera(cocos2d::Vec2 pos, float scale, CameraType cameraType)
{
	cameras_[static_cast<int>(cameraType)]->setScale(scale);
	NotOutRangeSetPos(pos, cameraType, scale);
}

void CameraManager::ScrollCamera(cocos2d::Vec2 pos, CameraType cameraType)
{
	NotOutRangeSetPos(pos, cameraType);
}
