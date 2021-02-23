#include "EnemyShader.h"
#include <renderer/backend/Device.h>
USING_NS_CC;

EnemyShader::EnemyShader()
{
	auto fileUtiles = FileUtils::getInstance();
	auto vertexSource = fileUtiles->getStringFromFile("Shader/EnemyShader.vert");
	auto fragmentSource = fileUtiles->getStringFromFile("Shader/EnemyShader.frag");
	program = backend::Device::getInstance()->newProgram(vertexSource.c_str(), fragmentSource.c_str());
}

EnemyShader::~EnemyShader()
{
	CC_SAFE_RELEASE(program);
}

void EnemyShader::SetShader(Node& node, cocos2d::Vec3 point)
{
	auto programState = new backend::ProgramState(program);
	auto psLoc = programState->getUniformLocation("u_OutlineColor");
	//programState->setUniform(psLoc, &color, sizeof(color));

	node.setProgramState(programState);
	CC_SAFE_RELEASE(programState);
}
