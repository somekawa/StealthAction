#include <DxLib.h>
#include "main.h"
#include "Scene/SceneMng.h"

// ==========WinMain�֐�
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) // Windows�ւ̖��߁@�K������
{
	//SetFontSize(48) ;							  //�T�C�Y��48�ɕύX
	//SetFontThickness(1);                          //������1�ɕύX
	//ChangeFont("HGS�n�p�p�߯�ߑ�");               //HGS�n�p�p�߯�ߑ̂ɕύX
	//ChangeFontType(DX_FONTTYPE_ANTIALIASING_EDGE);//�A���`�G�C���A�X���G�b�W�t���t�H���g�ɕύX
	//SetDrawMode(DX_DRAWMODE_BILINEAR);
	SceneMng::GetInstance().Run();
	SceneMng::Destroy();
	return 0;		//������۸��т̏I��
}
