#include "BehaviorBaseAI/NodeBase.h"
#include "BehaviorBaseAI/JudgementAIBase.h" 
#include "BehaviorBaseAI/BehaviorData.h"
#include "BehaviorBaseAI/ActionBase.h"

bool NodeBase::Judgement(Enemy* enemy)
{
	//�@����N���X�������Ă���Ύ����Ȃ����true����
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

	// ��ԗD�揇�ʂ������m�[�h��T��
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

	// �g�p�ς݃m�[�h���X�g�A�b�v����
	for (auto itr = list->begin(); itr != list->end(); itr++)
	{
		if (data->IsNodeUsed((*itr)->GetName()) == false)
		{
			offList.push_back(*itr);
		}
	}

	// ���X�g�A�b�v�������`�F�b�N
	if (offList.size() == 0)
	{
		// �m�[�h��S�ă��Z�b�g
		data->ResetUsedNode(&child_);
		offList = *list;
	}

	// �g�p�����m�[�h��o�^
	data->EntryUsedNode(offList[0]->GetName());

	// ���X�g�̐擪������m�[�h
	return offList[0];
}

NodeBase* NodeBase::SelectSequence(std::vector<NodeBase*>* list, BehaviorData* data)
{
	// ���̃m�[�h�̃V�[�P���X�̃X�e�b�v���擾
	int step = data->GetSequenceStep(GetName());

	// �V�[�P���X���I����Ă���I��
	if (static_cast<unsigned>(step) >= child_.size())
	{
		if (selectRule_ != BehaviorTree::SelectRule::SequencialLooping)
		{
			return NULL;
		}
		else 
		{
			// Looping�̏ꍇ�͓�������s
			step = 0;
		}
	}

	// ���Ԃ̃m�[�h�����s�ł��邩�̃`�F�b�N
	for (auto itr = list->begin(); itr != list->end(); itr++)
	{
		if (child_[step]->GetName() == (*itr)->GetName())
		{
			// �V�[�P���X�m�[�h���L�^
			data->PushSequenceNode(this);
			// �V�[�P���X�X�e�b�v���X�V
			data->SetSequenceStep(GetName(), step + 1);
			return child_[step];
		}
	}
	return NULL;
}

NodeBase* NodeBase::SearchNode(std::string searchName)
{
	// ���O����v
	if (name_ == searchName)
	{
		return this;
	}
	else 
	{
		// �q�m�[�h�Ō���
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

	// �q�m�[�h�Ŏ��s�\�ȃm�[�h��T��
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
			// ����N���X���Ȃ���Ζ������ɒǉ�
			list.push_back(child_[i]);
		}
	}

	// �I�����[���Ńm�[�h����
	switch (selectRule_)
	{
		// �D�揇��
	case BehaviorTree::SelectRule::Priority:
		result = SelectPriority(&list);
		break;
		// �I���E�I�t
	case BehaviorTree::SelectRule::OnOff:
		result = SelectOnOff(&list, data);
		break;
		// �����_��
	case BehaviorTree::SelectRule::Random:
		result = SelectRandom(&list);
		break;
		// �V�[�P���X
	case BehaviorTree::SelectRule::Sequence:
	case BehaviorTree::SelectRule::SequencialLooping:
		result = SelectSequence(&list, data);
		break;
	}

	if (result != NULL)
	{
		// �s��������ΏI��
		if (result->HasAction() == true)
		{
			return result;
		}
		else 
		{
			// ���܂����m�[�h�Ő��_�J�n
			result = result->Inference(enemy, data);
		}
	}
	return result;
}

State NodeBase::Run(Enemy* enemy)
{
	// �A�N�V�����m�[�h������Ύ��s���Č��ʂ�Ԃ��A�Ȃ���Ύ��s����
	if (action_ != NULL)
	{
		return action_->Run(enemy);
	}
	return State::Failed;
}