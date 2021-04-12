#include "ResShadowShader.h"
#include <renderer/backend/Device.h>

USING_NS_CC;

ResShadowShader::ResShadowShader()
{
	auto fileUtiles = FileUtils::getInstance();
	auto vertexSource = fileUtiles->getStringFromFile("Shader/Blink.vert");
	auto fragmentSource = fileUtiles->getStringFromFile("Shader/Blink.frag");
	program_ = backend::Device::getInstance()->newProgram(vertexSource.c_str(), fragmentSource.c_str());
}

ResShadowShader::~ResShadowShader()
{
	CC_SAFE_RELEASE(program_);
}

void ResShadowShader::SetShader(cocos2d::Node& node)
{
	auto programState = new backend::ProgramState(program_);
	node.setProgramState(programState);
	CC_SAFE_RELEASE(programState);
}