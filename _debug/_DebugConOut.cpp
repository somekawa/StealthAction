#ifdef _DEBUG

#include <Windows.h>
#include "_DebugConOut.h"

std::unique_ptr<_DebugConOut, _DebugConOut::_DebugConOutDeleter> _DebugConOut::sInstance(new _DebugConOut);

_DebugConOut::_DebugConOut()
{
	AllocConsole();
	freopen_s(&_debugFp, "CONOUT$", "w", stdout);
	freopen_s(&_debugFp, "CONIN$",  "r", stdin);
}


_DebugConOut::~_DebugConOut()
{
	FreeConsole();
}

#endif	//_DEBUG