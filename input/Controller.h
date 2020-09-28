#pragma once
#include <array>
#include <map>
#include "InputID.h"

// ���͑��u�̎��
enum class CONTROLLER_TYPE {
	KEYBOARD,
	MOUSE,
	GAMEPAD,
	GAMEPAD_2DAI,	// 2�ȏ�̐ڑ��m�F��
	MAX
};

// �g���K�[
enum class TRG
{
	NOW,
	OLD,
	MAX
};

// ���݂�1�t���[���O�̏���ۑ����Ă���
using TrgBool = std::array<bool, static_cast<size_t>(TRG::MAX)>;
// TrgBool��InputID�����݂���悤�ɂ���
using CtlData = std::map<InputID,TrgBool>;

struct Controller
{
	void operator()()
	{
		Update();
	}
	const CtlData& GetCtlData(void)				// �ǂݎ���p
	{
		return _data;	
	}

	virtual CONTROLLER_TYPE GetType(void) = 0;	// �����̓��͑��u��Ԃ�
	virtual bool Setup(void) = 0;				// ������(������������true):������myID?
	virtual void Rotate(int no) = 0;
	virtual void ChangeStage(int no, std::string str) = 0;
private:
	virtual void Update(void) = 0;				// �����擾����_data�Ɋi�[�ł���悤�ɂ���
protected:
	CtlData _data;								// �O���Ŏg����̂͂���
};

