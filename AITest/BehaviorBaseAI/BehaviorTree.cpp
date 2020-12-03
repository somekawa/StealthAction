#include "BehaviorBaseAI/BehaviorTree.h"
#include "BehaviorBaseAI/ActionBase.h"
#include "BehaviorBaseAI/NodeBase.h"
#include "BehaviorBaseAI/JudgementAIBase.h"
#include "obj/Enemy/Enemy.h"
#include "BehaviorBaseAI/BehaviorData.h"

BehaviorTree::BehaviorTree():
    root_(NULL)
{
}

NodeBase* BehaviorTree::Inference(Enemy* enemy, BehaviorData* data)
{
	// �f�[�^�����Z�b�g���ĊJ�n
	data->Init();
	return root_->Inference(enemy, data);
}

NodeBase* BehaviorTree::SequenceBack(NodeBase* sequenceNode, Enemy* enemy, BehaviorData* data)
{
	return sequenceNode->Inference(enemy, data);
}

void BehaviorTree::AddNode(std::string searchName, std::string entryName, int priority, SelectRule selRule, JudgementAIBase* judgment, ActionBase* action)
{
	if (searchName != "")
	{
		NodeBase* search_node = root_->SearchNode(searchName);

		if (search_node != NULL)
		{
			NodeBase* sibling = search_node->GetLastChild();
			NodeBase* add_node = new NodeBase(entryName, search_node, sibling, priority, selRule, judgment, action, search_node->GetHirerchyNo() + 1);

			search_node->AddChild(add_node);
		}
	}
	else {
		if (root_ == NULL)
		{
			root_ = new NodeBase(entryName, NULL, NULL, priority, selRule, judgment, action, 1);
		}
		else {
			printf("���[�g�͊��ɓo�^����Ă��܂�\n");
		}
	}
}

NodeBase* BehaviorTree::Run(Enemy* enemy, NodeBase* actionNode, BehaviorData* data)
{
	// �m�[�h���s
	State state = actionNode->Run(enemy);

	// ����I��
	if (state == State::Complete)
	{
		// �V�[�P���X�̓r�����𔻒f
		NodeBase* sequence_node = data->PopSequenceNode();

		// �r������Ȃ��Ȃ�I��
		if (sequence_node == NULL)
		{
			return NULL;
		}
		else {
			// �r���Ȃ炻������n�߂�
			return SequenceBack(sequence_node, enemy, data);
		}
		// ���s�͏I��
	}
	else if (state == State::Failed) {
		return NULL;
	}

	// ����ێ�
	return actionNode;
}
