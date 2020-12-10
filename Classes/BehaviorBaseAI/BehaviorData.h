#pragma once
#include <vector>
#include <stack>
#include <map>
#include "BehaviorBaseAI/BehaviorTree.h"

class NodeBase;
class Enemy;

class BehaviorData
{
public:
	BehaviorData();

	void PushSequenceNode(NodeBase* node);

	NodeBase* PopSequenceNode();

	bool IsNodeUsed(std::string name);

	void EntryUsedNode(std::string name);

	int GetSequenceStep(std::string name);

	void SetSequenceStep(std::string name,int step);

	void Init();

	void ResetUsedNode(std::vector<NodeBase*>* resetHierarchy);

private:
	std::stack<NodeBase*> sequenceStack_;
	std::map<std::string, int> runSequenceStepMap_;
	std::map<std::string, bool> usedNodeMap_;

};

