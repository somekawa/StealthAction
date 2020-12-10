#include "BehaviorBaseAI/BehaviorData.h"
#include "BehaviorBaseAI/NodeBase.h"
#include "obj/Enemy/Enemy.h"

BehaviorData::BehaviorData()
{
	Init();
}

void BehaviorData::PushSequenceNode(NodeBase* node)
{
	sequenceStack_.push(node);
}

NodeBase* BehaviorData::PopSequenceNode()
{
	if (!sequenceStack_.empty())
	{
		return NULL;
	}
	NodeBase* node = sequenceStack_.top();

	if (node != NULL)
	{
		sequenceStack_.pop();
	}

	return node;
}

bool BehaviorData::IsNodeUsed(std::string name)
{
	if (usedNodeMap_.count(name) == 0)
	{
		return false;
	}
	return usedNodeMap_[name];
}

void BehaviorData::EntryUsedNode(std::string name)
{
	usedNodeMap_[name] = true;
}

int BehaviorData::GetSequenceStep(std::string name)
{
	if (runSequenceStepMap_.count(name) == 0)
	{
		runSequenceStepMap_[name] = 0;
	}
	return runSequenceStepMap_[name];
}

void BehaviorData::SetSequenceStep(std::string name,int step)
{
	runSequenceStepMap_[name] = step;
}

void BehaviorData::Init()
{
	runSequenceStepMap_.clear();
	while (sequenceStack_.size() > 0)
	{
		sequenceStack_.pop();
	}
}

void BehaviorData::ResetUsedNode(std::vector<NodeBase*>* resetHierarchy)
{
	for (auto itr = resetHierarchy->begin(); itr != resetHierarchy->end(); itr++)
	{
		usedNodeMap_[(*itr)->GetName()] = false;
	}
}
