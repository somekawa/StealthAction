#include "BehaviorBaseAI/NodeBase.h"
#include "BehaviorBaseAI/JudgementAIBase.h" 
#include "BehaviorBaseAI/BehaviorData.h"
#include "BehaviorBaseAI/ActionBase.h"

bool NodeBase::Judgement(Enemy* enemy)
{
	//　判定クラスを持っていれば実効なければtrue扱い
	if (judgement_ != NULL)
	{
		return judgement_->Judgement(enemy);
	}
	return true;
}

NodeBase* NodeBase::SelectPriority(std::vector<NodeBase*>* list)
{
	NodeBase* selectNode = NULL;
	int priority = -10000;

	// 一番優先順位が高いノードを探す
	for (auto itr = list->begin(); itr != list->end(); itr++)
	{
		if (priority < (*itr)->GetPriority())
		{
			selectNode = (*itr);
			priority = (*itr)->GetPriority();
		}
	}

	return selectNode;
}

NodeBase* NodeBase::SelectRandom(std::vector<NodeBase*>* list)
{
	int selectNo = rand() % list->size();
	return (*list)[selectNo];
}

NodeBase* NodeBase::SelectOnOff(std::vector<NodeBase*>* list, BehaviorData* data)
{
	std::vector<NodeBase*> offList;

	// 使用済みノードリストアップする
	for (auto itr = list->begin(); itr != list->end(); itr++)
	{
		if (data->IsNodeUsed((*itr)->GetName()) == false)
		{
			offList.push_back(*itr);
		}
	}

	// リストアップした数チェック
	if (offList.size() == 0)
	{
		// ノードを全てリセット
		data->ResetUsedNode(&child_);
		offList = *list;
	}

	// 使用したノードを登録
	data->EntryUsedNode(offList[0]->GetName());

	// リストの先頭が決定ノード
	return offList[0];
}

NodeBase* NodeBase::SelectSequence(std::vector<NodeBase*>* list, BehaviorData* data)
{
	// 今のノードのシーケンスのステップを取得
	int step = data->GetSequenceStep(GetName());

	// シーケンスが終わってたら終了
	if (static_cast<unsigned>(step) >= child_.size())
	{
		if (selectRule_ != BehaviorTree::SelectRule::SequencialLooping)
		{
			return NULL;
		}
		else 
		{
			// Loopingの場合は頭から実行
			step = 0;
		}
	}

	// 順番のノードが実行できるかのチェック
	for (auto itr = list->begin(); itr != list->end(); itr++)
	{
		if (child_[step]->GetName() == (*itr)->GetName())
		{
			// シーケンスノードを記録
			data->PushSequenceNode(this);
			// シーケンスステップを更新
			data->SetSequenceStep(GetName(), step + 1);
			return child_[step];
		}
	}
	return NULL;
}

NodeBase* NodeBase::SearchNode(std::string searchName)
{
	// 名前が一致
	if (name_ == searchName)
	{
		return this;
	}
	else 
	{
		// 子ノードで検索
		for (auto itr = child_.begin(); itr != child_.end(); itr++)
		{
			NodeBase* ret = (*itr)->SearchNode(searchName);

			if (ret != NULL)
			{
				return ret;
			}
		}
	}

	return NULL;
}

NodeBase* NodeBase::Inference(Enemy* enemy, BehaviorData* data)
{
	std::vector<NodeBase*> list;
	NodeBase* result = NULL;

	// 子ノードで実行可能なノードを探す
	for (unsigned int i = 0; i < child_.size(); i++)
	{
		if (child_[i]->judgement_ != NULL)
		{
			if (child_[i]->judgement_->Judgement(enemy) == true)
			{
				list.push_back(child_[i]);
			}
		}
		else 
		{
			// 判定クラスがなければ無条件に追加
			list.push_back(child_[i]);
		}
	}

	// 選択ルールでノード決め
	switch (selectRule_)
	{
		// 優先順位
	case BehaviorTree::SelectRule::Priority:
		result = SelectPriority(&list);
		break;
		// オン・オフ
	case BehaviorTree::SelectRule::OnOff:
		result = SelectOnOff(&list, data);
		break;
		// ランダム
	case BehaviorTree::SelectRule::Random:
		result = SelectRandom(&list);
		break;
		// シーケンス
	case BehaviorTree::SelectRule::Sequence:
	case BehaviorTree::SelectRule::SequencialLooping:
		result = SelectSequence(&list, data);
		break;
	}

	if (result != NULL)
	{
		// 行動があれば終了
		if (result->HasAction() == true)
		{
			return result;
		}
		else 
		{
			// 決まったノードで推論開始
			result = result->Inference(enemy, data);
		}
	}
	return result;
}

State NodeBase::Run(Enemy* enemy)
{
	// アクションノードがあれば実行して結果を返す、なければ失敗扱い
	if (action_ != NULL)
	{
		return action_->Run(enemy);
	}
	return State::Failed;
}