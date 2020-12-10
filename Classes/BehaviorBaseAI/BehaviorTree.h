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
		// 優先順位
		Priority,
		// シーケンス
		Sequence,
		// シーケンシャルルーピング
		SequencialLooping,
		// ランダム
		Random,
		// オンオフ法
		OnOff
	};

public:
	BehaviorTree();

	NodeBase* Inference(Enemy* enemy, BehaviorData* data);

	NodeBase* SequenceBack(NodeBase* sequenceNode, Enemy* enemy, BehaviorData* data);

	void AddNode(std::string searchName, std::string entryName, int priority, SelectRule selRule, JudgementAIBase* judgment, ActionBase* action);

	NodeBase* Run(Enemy* enemy, NodeBase* actionNode, BehaviorData* data);

private:
	// ルートノード
	NodeBase* root_;
};

