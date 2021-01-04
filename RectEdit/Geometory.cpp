#include <DxLib.h>
#include "Geometory.h"

void 
Rect::Draw(void) const
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DxLib::DrawBox((int)Left(), (int)Top(), (int)Right(), (int)Bottom(), 0xff0000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
