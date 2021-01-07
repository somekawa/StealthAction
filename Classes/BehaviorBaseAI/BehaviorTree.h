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
	// 実行ノードを推論する
	NodeBase* Inference(Enemy* enemy, BehaviorData* data);
	// シーケンスノードから推論開始
	NodeBase* SequenceBack(NodeBase* sequenceNode, Enemy* enemy, BehaviorData* data);
	// ノード追加
	void AddNode(std::string searchName, std::string entryName, int priority, SelectRule selRule, JudgementAIBase* judgment, ActionBase* action);
	// 実行
	NodeBase* Run(Enemy* enemy, NodeBase* actionNode, BehaviorData* data);

private:
	// ルートノード
	NodeBase* root_;
};

