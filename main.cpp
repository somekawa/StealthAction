#include <DxLib.h>
#include "main.h"
#include "Scene/SceneMng.h"

// ==========WinMain関数
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) // Windowsへの命令　必ず書く
{
	//SetFontSize(48) ;							  //サイズを48に変更
	//SetFontThickness(1);                          //太さを1に変更
	//ChangeFont("HGS創英角ﾎﾟｯﾌﾟ体");               //HGS創英角ﾎﾟｯﾌﾟ体に変更
	//ChangeFontType(DX_FONTTYPE_ANTIALIASING_EDGE);//アンチエイリアス＆エッジ付きフォントに変更
	//SetDrawMode(DX_DRAWMODE_BILINEAR);
	SceneMng::GetInstance().Run();
	SceneMng::Destroy();
	return 0;		//このﾌﾟﾛｸﾞﾗﾑの終了
}
