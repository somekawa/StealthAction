#pragma once
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
#ifdef _DEBUG
#include <string>
#include <memory>
#include <Windows.h>
#include <cocos2d.h>

#define DEBUG_DrawRect(fmt, ...) _DebugDispOutCC::GetInstance().DrawRect(fmt, __VA_ARGS__)

class _DebugDispOutCC
{
public:
	static _DebugDispOutCC &GetInstance()
	{
		return (*s_Instance);
	}
	bool DrawRect(std::string spName, cocos2d::Vec2 pos, cocos2d::Vec2 posLU, cocos2d::Vec2 posRD, cocos2d::Color4F col);
private:
	struct _DebugDispOutDeleter
	{
		void operator()(_DebugDispOutCC* _debugContOut) const
		{
			delete _debugContOut;
		}
	};

	_DebugDispOutCC();
	~_DebugDispOutCC();
	int _DbgScreen;
	static std::unique_ptr<_DebugDispOutCC, _DebugDispOutDeleter> s_Instance;
};
#else
#define DEBUG_DrawRect(fmt, ...)
#endif	// _DEBUG
#endif

