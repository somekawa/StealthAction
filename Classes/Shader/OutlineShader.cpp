#include "OutlineShader.h"
#include <renderer/backend/Device.h>

USING_NS_CC;

OutlineShader::OutlineShader()
{
	auto fileUtiles = FileUtils::getInstance();
	auto vertexSource = fileUtiles->getStringFromFile("Shader/OutLineTest.vert");
	auto fragmentSource = fileUtiles->getStringFromFile("Shader/OutLineTest.frag");
	program = backend::Device::getInstance()->newProgram(vertexSource.c_str(), fragmentSource.c_str());
}

OutlineShader::~OutlineShader()
{
	CC_SAFE_RELEASE(program);
}

void OutlineShader::SetShader(cocos2d::Node& node, cocos2d::Vec3 color)
{
	auto programState = new backend::ProgramState(program);
	auto psLoc = programState->getUniformLocation("u_OutlineColor");
	auto psValues = Vec3(1, 0, 0);
	programState->setUniform(psLoc, &psValues, sizeof(psValues));
	node.setProgramState(programState);
	CC_SAFE_RELEASE(programState);
}