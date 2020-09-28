#pragma once
#include <memory>
#include "../common/VECTOR2.h"

// �v���g�^�C�v�錾
class BaseScene;
struct Controller;

// �ȗ��p
using unique_Base = std::unique_ptr<BaseScene>;

class BaseScene
{
public:
	virtual ~BaseScene();

	// unique_Base own : ������Scene�|�C���^
	virtual unique_Base Update(unique_Base own) = 0;
	virtual bool Init(void) = 0;	
	virtual void Draw(void) = 0;
};