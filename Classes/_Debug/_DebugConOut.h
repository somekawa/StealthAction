#pragma once
#ifdef _DEBUG
#include<memory>

#define TRACE(fmt, ...) printf(fmt, __VA_ARGS__)
#define _debugConOut() _DebugConOut::GetInstance()	// ﾃﾞﾊﾞｯｸ用文字列出力

//#define AST() {\
//	CHAR 
//};

class _DebugConOut
{
public:
	static _DebugConOut& GetInstance()
	{
		return (*s_Instance);
	}
private:
	// ﾃﾞﾘｰﾀｰ
	struct _DebugConOutDeleter
	{
		void operator()(_DebugConOut* _debugConOut) const
		{
			delete _debugConOut;
		}
	};

	_DebugConOut();
	~_DebugConOut();
	FILE* _debugFp;
	//static _DebugConOut *s_Instance;
	static std::unique_ptr<_DebugConOut, _DebugConOutDeleter> s_Instance;
};
#else
#define TRACE(fmt, ...)
#define _debugConOut()

#endif // _DEBUG

