#pragma once
#include <string>

class ActionBase;
class JudgementAIBase;
class NodeBase;
class BehaviorData;
class Enemy;

class BehaviorTree
{
public:
	enum SelectRule
	{
		Non,
		// �D�揇��
		Priority,
		// �V�[�P���X
		Sequence,
		// �V�[�P���V�������[�s���O
		SequencialLooping,
		// �����_��
		Random,
		// �I���I�t�@
		OnOff
	};

public:
	BehaviorTree();
	// ���s�m�[�h�𐄘_����
	NodeBase* Inference(Enemy* enemy, BehaviorData* data);
	// �V�[�P���X�m�[�h���琄�_�J�n
	NodeBase* SequenceBack(NodeBase* sequenceNode, Enemy* enemy, BehaviorData* data);
	// �m�[�h�ǉ�
	void AddNode(std::string searchName, std::string entryName, int priority, SelectRule selRule, JudgementAIBase* judgment, ActionBase* action);
	// ���s
	NodeBase* Run(Enemy* enemy, NodeBase* actionNode, BehaviorData* data);

private:
	// ���[�g�m�[�h
	NodeBase* root_;
};

