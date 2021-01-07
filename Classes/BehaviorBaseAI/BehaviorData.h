#pragma once
#include <vector>
#include <stack>
#include <map>
#include "BehaviorBaseAI/BehaviorTree.h"

class NodeBase;
class Enemy;

// Behavior�ۑ��f�[�^
class BehaviorData
{
public:
	BehaviorData();
	// �V�[�P���X�m�[�h�̃v�b�V��
	void PushSequenceNode(NodeBase* node);
	// �V�[�P���X�m�[�h�̃|�b�v
	NodeBase* PopSequenceNode();
	// �m�[�h�g�p����
	bool IsNodeUsed(std::string name);
	// �g�p�ς݃m�[�h�ɓo�^
	void EntryUsedNode(std::string name);
	// �V�[�P���X�X�e�b�v�̎擾
	int GetSequenceStep(std::string name);
	// �V�[�P���X�X�e�b�v�̃Z�b�g
	void SetSequenceStep(std::string name,int step);
	// ������
	void Init();
	// �g�p�ς݃m�[�h�̃��Z�b�g
	void ResetUsedNode(std::vector<NodeBase*>* resetHierarchy);

private:
	// �V�[�P���X�m�[�h�X�^�b�N
	std::stack<NodeBase*> sequenceStack_;
	// ���s�V�[�P���X�̃X�e�b�v�}�b�v
	std::map<std::string, int> runSequenceStepMap_;
	// �m�[�h�̎g�p����}�b�v
	std::map<std::string, bool> usedNodeMap_;

};

