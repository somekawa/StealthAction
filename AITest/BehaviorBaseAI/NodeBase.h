#pragma once

#include <vector>
#include <string>
#include "BehaviorBaseAI/BehaviorTree.h"
#include "BehaviorBaseAI/ActionBase.h"

class JudgementAIBase;
class BehaviorData;

// �m�[�h
class NodeBase
{
public:
	NodeBase(std::string name, NodeBase* parent, NodeBase* sibling, int priority, BehaviorTree::SelectRule selRule, JudgementAIBase* judgement, ActionBase* action, int hierarchyNo) :
		name_(name),
		parent_(parent),
		sibling_(sibling),
		priority_(priority),
		selectRule_(selRule),
		judgement_(judgement),
		action_(action),
		hierarchyNo_(hierarchyNo),
		child_(NULL)
	{
	}

	// ���O�Q�b�^�[
	std::string GetName()
	{
		return name_;
	}

	// �e�m�[�h�Q�b�^�[
	NodeBase* GetParent()
	{
		return parent_;
	}

	// �q�m�[�h�Q�b�^�[
	NodeBase* GetChild(int idx)
	{
		if (child_.size() >= idx)
		{
			return NULL;
		}
		return child_[idx];
	}

	// �q�m�[�h�Q�b�^�[(����)
	NodeBase* GetLastChild()
	{
		if (child_.size() == 0)
		{
			return NULL;
		}

		return child_[child_.size() - 1];
	}

	// �q�m�[�h�Q�b�^�[(�擪)
	NodeBase* GetTopChild()
	{
		if (child_.size() == 0)
		{
			return NULL;
		}

		return child_[0];
	}

	// �Z��m�[�h�Q�b�^�[
	NodeBase* GetSibling(void)
	{
		return sibling_;
	}

	// �K�w�ԍ��Q�b�^�[
	int GetHirerchyNo()
	{
		return hierarchyNo_;
	}

	// �D�揇�ʃQ�b�^�[
	int GetPriority()
	{
		return priority_;
	}

	// �e�m�[�h�Z�b�^�[
	void SetParent(NodeBase* parent)
	{
		parent_ = parent;
	}

	// �q�m�[�h�ǉ�
	void AddChild(NodeBase* child)
	{
		child_.push_back(child);
	}

	// �Z��m�[�h�Z�b�^�[
	void SetSibling(NodeBase* sibling)
	{
		sibling_ = sibling;
	}

	// �s���f�[�^�������Ă��邩
	bool HasAction()
	{
		return action_ != NULL ? true : false;
	}

	// ���s�۔���
	bool Judgement(Enemy* enemy);

	// �D�揇�ʑI��
	NodeBase* SelectPriority(std::vector<NodeBase*>* list);
	// �����_���I��
	NodeBase* SelectRandom(std::vector<NodeBase*>* list);
	// �I���E�I�t�I��
	NodeBase* SelectOnOff(std::vector<NodeBase*>* list, BehaviorData* data);
	// �V�[�P���X�I��
	NodeBase* SelectSequence(std::vector<NodeBase*>* list, BehaviorData* data);
	// �m�[�h����
	NodeBase* SearchNode(std::string search_name);
	// �m�[�h���_
	NodeBase* Inference(Enemy* enemy, BehaviorData* data);
	// ���s
	State Run(Enemy* enemy);

protected:
	std::string name_;						// ���O
	BehaviorTree::SelectRule selectRule_;	// �I�����[��
	JudgementAIBase* judgement_;				// ����N���X
	ActionBase* action_;					// ���s�N���X
	unsigned int priority_;					// �D�揇��
	NodeBase* parent_;						// �e�m�[�h
	std::vector<NodeBase*> child_;			// �q�m�[�h
	NodeBase* sibling_;						// �Z��m�[�h
	int hierarchyNo_;						// �K�w�ԍ�
};

