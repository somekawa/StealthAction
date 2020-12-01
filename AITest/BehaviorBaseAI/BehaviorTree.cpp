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
	// データをリセットして開始
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
			printf("ルートは既に登録されています\n");
		}
	}
}

NodeBase* BehaviorTree::Run(Enemy* enemy, NodeBase* actionNode, BehaviorData* data)
{
	// ノード実行
	State state = actionNode->Run(enemy);

	// 正常終了
	if (state == State::Complete)
	{
		// シーケンスの途中かを判断
		NodeBase* sequence_node = data->PopSequenceNode();

		// 途中じゃないなら終了
		if (sequence_node == NULL)
		{
			return NULL;
		}
		else {
			// 途中ならそこから始める
			return SequenceBack(sequence_node, enemy, data);
		}
		// 失敗は終了
	}
	else if (state == State::Failed) {
		return NULL;
	}

	// 現状維持
	return actionNode;
}
