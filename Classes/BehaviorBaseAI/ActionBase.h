#pragma once

class Enemy;

// ���s��
enum State
{
	// ���s��
	Run,
	// ���s
	Failed,
	// ����
	Complete
};

// �s���������s�N���X
class ActionBase
{
public:

	virtual State Run(Enemy* enemy) = 0;
};

