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

	// �J������ǉ�����
	void AddCamera(cocos2d::Scene& scene,  cocos2d::Size zoomSize, CameraType cameraType, cocos2d::CameraFlag cameraFlag);
	//�J������pos�𒆐S�ɃY�[������
	void ZoomCamera(cocos2d::Vec2 pos, float scale, CameraType cameraType);	
	// �J�������ړ�����(�ʏ�X�e�[�W�ł͎g��Ȃ��ă{�X�����̂�)
	void ScrollCamera(cocos2d::Vec2 pos, CameraType cameraType);
	
private:
	// �J�����̃|�W�V�������}�b�v�O�������Ȃ��悤�Ɉړ�������
	void NotOutRangeSetPos(cocos2d::Vec2& pos, CameraType cameraType, float scale = 1.0f);
	std::array<cocos2d::Camera*,static_cast<int>(CameraType::MAX)>cameras_; // �J�����i�[�p
};

