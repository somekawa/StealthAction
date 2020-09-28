#pragma once
#include <map>
#include <string>
#include <vector>
#include "Controller.h"

struct GamePad :
	public Controller
{
	CONTROLLER_TYPE GetType(void) override;		// �����̓��͑��u��Ԃ�
	bool Setup(void) override;					// ������(������������true)
	void Update(void)  override;				// �����擾����_data�Ɋi�[�ł���悤�ɂ���
	void ChangeStage(int no,std::string str) override;	// ����X�e�[�W�̐ؑ�
	void Rotate(int no) override;
private:
	std::map<InputID, int> _inputTbl;	// �Ή��\
	//int PadInput;
	//int padNum;
	std::vector<int> padInpVec;
	std::vector<int> padVec;
};