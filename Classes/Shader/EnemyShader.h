#pragma once
#include <cocos2d.h>
class EnemyShader
{
public:
	EnemyShader();
	~EnemyShader();
	// �m�[�h�ɃV�F�[�_��ݒ�
	void SetShader(cocos2d::Node& node, cocos2d::Vec3 point);

private:

	cocos2d::backend::Program* program;
};

