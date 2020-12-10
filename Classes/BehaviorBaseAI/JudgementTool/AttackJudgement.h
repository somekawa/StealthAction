#include "BehaviorBaseAI/JudgementAIBase.h"

class AttackJudgement :public JudgementAIBase
{
public:
	static AttackJudgement* Instance()
	{
		static AttackJudgement sIns_;
		return &sIns_;
	}

	// ”»’è
	virtual bool Judgement(Enemy* enemy);
private:
};