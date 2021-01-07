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

	// 名前取得
	std::string GetName()
	{
		return name_;
	}

	// 親ノード取得
	NodeBase* GetParent()
	{
		return parent_;
	}

	// 子ノード取得
	NodeBase* GetChild(int idx)
	{
		if (child_.size() >= idx)
		{
			return NULL;
		}
		return child_[idx];
	}

	// 子ノード取得(末尾)
	NodeBase* GetLastChild()
	{
		if (child_.size() == 0)
		{
			return NULL;
		}

		return child_[child_.size() - 1];
	}

	// 子ノード取得(先頭)
	NodeBase* GetTopChild()
	{
		if (child_.size() == 0)
		{
			return NULL;
		}

		return child_[0];
	}

	// 兄弟ノード取得
	NodeBase* GetSibling(void)
	{
		return sibling_;
	}

	// 階層番号取得
	int GetHirerchyNo()
	{
		return hierarchyNo_;
	}

	// 優先順位取得
	int GetPriority()
	{
		return priority_;
	}

	// 親ノードセット
	void SetParent(NodeBase* parent)
	{
		parent_ = parent;
	}

	// 子ノード追加
	void AddChild(NodeBase* child)
	{
		child_.push_back(child);
	}

	// 兄弟ノードセット
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
	// 名前
	std::string name_;		
	// 選択ルール
	BehaviorTree::SelectRule selectRule_;
	// 判定クラス
	JudgementAIBase* judgement_;	
	// 実行クラス
	ActionBase* action_;
	// 優先順位
	unsigned int priority_;			
	// 親ノード
	NodeBase* parent_;	
	// 子ノード
	std::vector<NodeBase*> child_;		
	// 兄弟ノード
	NodeBase* sibling_;		
	// 階層番号
	int hierarchyNo_;						// 階層番号
};

