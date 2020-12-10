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

	NodeBase* Inference(Enemy* enemy, BehaviorData* data);

	NodeBase* SequenceBack(NodeBase* sequenceNode, Enemy* enemy, BehaviorData* data);

	void AddNode(std::string searchName, std::string entryName, int priority, SelectRule selRule, JudgementAIBase* judgment, ActionBase* action);

	NodeBase* Run(Enemy* enemy, NodeBase* actionNode, BehaviorData* data);

private:
	// ���[�g�m�[�h
	NodeBase* root_;
};

