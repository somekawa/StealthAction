#ifdef _DEBUG
#include <windows.h>
#include <DxLib.h>
#include "_DebugDispOut.h"
#include "_DebugConOut.h"

std::unique_ptr<_DebugDispOut, _DebugDispOut::_DebugDispOutDeleter> _DebugDispOut::s_Instance(new _DebugDispOut);
_DebugDispOut::_DebugDispOut()
{
	_DbgScreen = -1;
	_waitTime  = 0;
	_alpha = 255;
	_dispFlag = true;
	_endKey[0] = 0;
	_endKey[1] = 0;
	_ghBefor = 0;
	_clsFlag = true;
}

_DebugDispOut::~_DebugDispOut()
{
}

void _DebugDispOut::SetScreen(void)
{
	_ghBefor = GetDrawScreen();
	SetDrawScreen(_DbgScreen);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, _alpha);
}

void _DebugDispOut::RevScreen(void)
{
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	SetDrawScreen(_ghBefor);
}

void _DebugDispOut::WaitMode(void)
{
	if (CheckHitKey(KEY_INPUT_PGDN))
	{
		_waitTime+=10.0;
		TRACE("スロー：%f\n", _waitTime);
	}
	if (CheckHitKey(KEY_INPUT_PGUP))
	{
		_waitTime -= 10.0;
		if (_waitTime < 0.0)
		{
			_waitTime = 0.0;
		}
		TRACE("スロー：%f\n", _waitTime);
	}
	if (CheckHitKey(KEY_INPUT_PAUSE))
	{
		TRACE("一時停止\n");
		_waitTime = -1.0;
	}
	if (_waitTime)
	{
		_startTime = std::chrono::system_clock::now();
		do {
			if (ProcessMessage() != 0 || CheckHitKey(KEY_INPUT_ESCAPE) == 1)
			{
				break;
			}
			if (CheckHitKey(KEY_INPUT_END))
			{
				_waitTime = 0.0;
				TRACE("スロー/一時停止　解除\n");
			}
			_endTime = std::chrono::system_clock::now();
		} while (std::chrono::duration_cast<std::chrono::milliseconds>(_endTime - _startTime).count() < _waitTime || _waitTime < 0.0);
	}
	_endKey[1] = _endKey[0];
	_endKey[0] = CheckHitKey(KEY_INPUT_HOME);
	if (_endKey[0] && !_endKey[1])
	{
		_clsFlag ^= 1;
		TRACE("デバッグ表示クリアー機能：%d\n",_clsFlag);
	}
}

int _DebugDispOut::DrawGraph(int x, int y, int GrHandle, int TransFlag)
{
	SetScreen();
	int rtnFlag = DxLib::DrawGraph(x , y , GrHandle, TransFlag);
	RevScreen();
	return rtnFlag;
}

int _DebugDispOut::DrawBox(int x1, int y1, int x2, int y2, unsigned int Color, int FillFlag)
{
	SetScreen();
	int rtnFlag = DxLib::DrawBox(x1 , y1 , x2 , y2 , Color, FillFlag);
	RevScreen();
	return rtnFlag;
}

int _DebugDispOut::DrawString(int x, int y, char* String, unsigned int Color)
{
	SetScreen();
	int rtnFlag = DxLib::DrawString(x, y, String, Color);
	RevScreen();
	return rtnFlag;
}
//
//int _DebugDispOut::DrawFormatString(int x, int y, unsigned int Color, std::string FormatString, ...)
//{
//	va_list arglist;
//	va_start(arglist, FormatString);
//	SetScreen();
//	int rtnFlag = DxLib::DrawFormatString(x , y , Color, FormatString.c_str(), va_arg(arglist, int), va_arg(arglist, int));
//	RevScreen();
//	va_end(arglist);
//	return rtnFlag;
//}

int _DebugDispOut::DrawLine(int x1, int y1, int x2, int y2, unsigned int Color)
{
	SetScreen();
	int rtnFlag = DxLib::DrawLine(x1 , y1 , x2 , y2 , Color);
	RevScreen();
	return rtnFlag;
}

int _DebugDispOut::DrawCircle(int x, int y, int r, unsigned int Color, int FillFlag)
{
	SetScreen();
	int rtnFlag = DxLib::DrawCircle(x , y , r, Color, FillFlag);
	RevScreen();
	return rtnFlag;
}

int _DebugDispOut::DrawPixel(int x, int y, unsigned int Color)
{
	SetScreen();
	int rtnFlag = DxLib::DrawPixel(x , y , Color);
	RevScreen();
	return rtnFlag;
}

bool _DebugDispOut::StartDrawDebug(void)
{
	int ghBefor;
	ghBefor = GetDrawScreen();
	SetDrawScreen(_DbgScreen);
	if (_clsFlag)
	{
		ClsDrawScreen();
	}
	SetDrawScreen(ghBefor);
	return true;
}

bool _DebugDispOut::AddDrawDebug(void)
{
	if (CheckHitKey(KEY_INPUT_INSERT))
	{
		TRACE("デバッグ表示ON\n");
		_dispFlag = true;
	}
	if (CheckHitKey(KEY_INPUT_DELETE))
	{
		TRACE("デバッグ表示OFF\n");
		_dispFlag = false;
	}
	if (_dispFlag)
	{
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		DxLib::DrawGraph(0, 0, _DbgScreen, true);
		//lpSceneMng.AddDrawQue({ _DbgScreen,lpSceneMng.ScreenSize.x/2,lpSceneMng.ScreenSize.y / 2,0,INT_MAX,LAYER::UI,DX_BLENDMODE_NOBLEND,255 });
	}
	WaitMode();
	return true;
}

bool _DebugDispOut::SetAlpha(int alpha)
{
	_alpha = alpha;

	return true;
}

bool _DebugDispOut::Setup(int screenSizeX, int screenSizeY,int alpha)
{
	if (_DbgScreen == -1)
	{
		_DbgScreen = MakeScreen(screenSizeX, screenSizeY, true);
	}
	SetAlpha(alpha);
	return false;
}

bool _DebugDispOut::SetWait(double timeCnt)
{
	_waitTime = timeCnt;
	return true;
}

#endif	// _DEBUG
