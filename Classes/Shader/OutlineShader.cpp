#include "OutlineShader.h"
#include <renderer/backend/Device.h>

USING_NS_CC;

OutlineShader::OutlineShader()
{
	auto fileUtiles = FileUtils::getInstance();
	auto vertexSource = fileUtiles->getStringFromFile("Shader/OutLineTest.vert");
	auto fragmentSource = fileUtiles->getStringFromFile("Shader/OutLineTest.frag");
	program_ = backend::Device::getInstance()->newProgram(vertexSource.c_str(), fragmentSource.c_str());
}

OutlineShader::~OutlineShader()
{
	CC_SAFE_RELEASE(program_);
}

void OutlineShader::SetShader(cocos2d::Node& node, cocos2d::Vec3 color)
{
	auto programState = new backend::ProgramState(program_);
	auto psLoc = programState->getUniformLocation("u_OutlineColor");
	
	programState->setUniform(psLoc, &color, sizeof(color));
	node.setProgramState(programState);
	CC_SAFE_RELEASE(programState);
}