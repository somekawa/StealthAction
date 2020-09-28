#pragma once
#include <memory>
#include "../common/VECTOR2.h"

// プロトタイプ宣言
class BaseScene;
struct Controller;

// 省略用
using unique_Base = std::unique_ptr<BaseScene>;

class BaseScene
{
public:
	virtual ~BaseScene();

	// unique_Base own : 自分のSceneポインタ
	virtual unique_Base Update(unique_Base own) = 0;
	virtual bool Init(void) = 0;	
	virtual void Draw(void) = 0;
};