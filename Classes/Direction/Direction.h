#pragma once

// �����ϐ�
enum class Direction
{
	Right,
	Left,
	Max
};

// �}�b�v�����ϐ�
enum class MapDirection
{								  //angle - 1
	E_UP,	// ���̏�	�E���̏�	  0�` 44
	N_RIGHT,// �k�̉E	�㑤�̉E	 45�` 89
	N_LEFT,	// �k�̍�	�㑤�̍�	 90�`134
	W_UP,	// ���̏�	�����̏�	135�`179
	W_DOWN,	// ���̉�	�����̉�	180�`224
	S_LEFT,	// ��̍�	�����̍�	225�`269
	S_RIGHT,// ��̉E	�����̉E	270�`314
	E_Down,	// ���̉�	�E���̉�	315�`359
	Max
};
//0,3 : 1, 6 :  2,5 : 4,7

static Direction operator+(Direction& dir, int i)
{
	return (Direction)((int)dir + i);
}