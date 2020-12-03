#pragma once

#include <vector>
#include <string>
#include "BehaviorBaseAI/BehaviorTree.h"
#include "BehaviorBaseAI/ActionBase.h"

class JudgementAIBase;
class BehaviorData;

// ノード
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

	// 名前ゲッター
	std::string GetName()
	{
		return name_;
	}

	// 親ノードゲッター
	NodeBase* GetParent()
	{
		return parent_;
	}

	// 子ノードゲッター
	NodeBase* GetChild(int idx)
	{
		if (child_.size() >= idx)
		{
			return NULL;
		}
		return child_[idx];
	}

	// 子ノードゲッター(末尾)
	NodeBase* GetLastChild()
	{
		if (child_.size() == 0)
		{
			return NULL;
		}

		return child_[child_.size() - 1];
	}

	// 子ノードゲッター(先頭)
	NodeBase* GetTopChild()
	{
		if (child_.size() == 0)
		{
			return NULL;
		}

		return child_[0];
	}

	// 兄弟ノードゲッター
	NodeBase* GetSibling(void)
	{
		return sibling_;
	}

	// 階層番号ゲッター
	int GetHirerchyNo()
	{
		return hierarchyNo_;
	}

	// 優先順位ゲッター
	int GetPriority()
	{
		return priority_;
	}

	// 親ノードセッター
	void SetParent(NodeBase* parent)
	{
		parent_ = parent;
	}

	// 子ノード追加
	void AddChild(NodeBase* child)
	{
		child_.push_back(child);
	}

	// 兄弟ノードセッター
	void SetSibling(NodeBase* sibling)
	{
		sibling_ = sibling;
	}

	// 行動データを持っているか
	bool HasAction()
	{
		return action_ != NULL ? true : false;
	}

	// 実行可否判定
	bool Judgement(Enemy* enemy);

	// 優先順位選択
	NodeBase* SelectPriority(std::vector<NodeBase*>* list);
	// ランダム選択
	NodeBase* SelectRandom(std::vector<NodeBase*>* list);
	// オン・オフ選択
	NodeBase* SelectOnOff(std::vector<NodeBase*>* list, BehaviorData* data);
	// シーケンス選択
	NodeBase* SelectSequence(std::vector<NodeBase*>* list, BehaviorData* data);
	// ノード検索
	NodeBase* SearchNode(std::string search_name);
	// ノード推論
	NodeBase* Inference(Enemy* enemy, BehaviorData* data);
	// 実行
	State Run(Enemy* enemy);

protected:
	std::string name_;						// 名前
	BehaviorTree::SelectRule selectRule_;	// 選択ルール
	JudgementAIBase* judgement_;				// 判定クラス
	ActionBase* action_;					// 実行クラス
	unsigned int priority_;					// 優先順位
	NodeBase* parent_;						// 親ノード
	std::vector<NodeBase*> child_;			// 子ノード
	NodeBase* sibling_;						// 兄弟ノード
	int hierarchyNo_;						// 階層番号
};

