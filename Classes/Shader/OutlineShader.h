#pragma once
#include <cocos2d.h>

class cocos2d::backend::Program;
class OutlineShader
{
public:
	OutlineShader();
	~OutlineShader();
	// �m�[�h�ɃV�F�[�_��ݒ�
	void SetShader(cocos2d::Node& node, cocos2d::Vec3 color);

private:

	cocos2d::backend::Program* program;
};

