#pragma once
#ifdef _DEBUG
#include <windows.h>
#include <memory>
#include <assert.h>
// ÃÞÊÞ¯¸ÞÒ¯¾°¼Þ—p’è‹`

#define TRACE(fmt, ...) printf(fmt, __VA_ARGS__);

class _DebugConOut
{
public:
	static _DebugConOut& GetInstance()
	{
		return *sInstance;
	}

private:
	struct _DebugConOutDeleter
	{
		void operator()(_DebugConOut * _debugConOut) const
		{
			delete _debugConOut;
		}
	};

	_DebugConOut();
	~_DebugConOut();

	FILE* _debugFp = nullptr;
	static std::unique_ptr<_DebugConOut, _DebugConOutDeleter> sInstance;
};

#else	//_DEBUG
#define TRACE(fmt, ...)
#endif	//_DEBUG