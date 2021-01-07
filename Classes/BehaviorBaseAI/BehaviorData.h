#pragma once
#include <vector>
#include <stack>
#include <map>
#include "BehaviorBaseAI/BehaviorTree.h"

class NodeBase;
class Enemy;

// Behavior保存データ
class BehaviorData
{
public:
	BehaviorData();
	// シーケンスノードのプッシュ
	void PushSequenceNode(NodeBase* node);
	// シーケンスノードのポップ
	NodeBase* PopSequenceNode();
	// ノード使用判定
	bool IsNodeUsed(std::string name);
	// 使用済みノードに登録
	void EntryUsedNode(std::string name);
	// シーケンスステップの取得
	int GetSequenceStep(std::string name);
	// シーケンスステップのセット
	void SetSequenceStep(std::string name,int step);
	// 初期化
	void Init();
	// 使用済みノードのリセット
	void ResetUsedNode(std::vector<NodeBase*>* resetHierarchy);

private:
	// シーケンスノードスタック
	std::stack<NodeBase*> sequenceStack_;
	// 実行シーケンスのステップマップ
	std::map<std::string, int> runSequenceStepMap_;
	// ノードの使用判定マップ
	std::map<std::string, bool> usedNodeMap_;

};

