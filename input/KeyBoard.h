#pragma once
#include <array>
#include <map>
#include "Controller.h"

using KEY_ARRAY = std::array<char, 256>;

struct KeyBoard :
	public Controller
{
	CONTROLLER_TYPE GetType(void) override;		// �����̓��͑��u��Ԃ�
	bool Setup(void) override;					// ������(������������true)
	void Update(void)  override;				// �����擾����_data�Ɋi�[�ł���悤�ɂ���
	void ChangeStage(int no, std::string str) override;	// ����X�e�[�W�̐ؑ�
	void Rotate(int no) override;
private:
	KEY_ARRAY _keyData;							// �R���e�i�ɐ��ŃA�N�Z�X�������Ƃ���data���悭�g��
	std::map<InputID, int> _inputTbl;			// �Ή��\
};