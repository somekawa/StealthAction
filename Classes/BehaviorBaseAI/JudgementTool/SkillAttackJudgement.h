#include "BehaviorBaseAI/JudgementAIBase.h"

class SkillAttackJudgement :public JudgementAIBase
{
public:
	static SkillAttackJudgement* Instance()
	{
		static SkillAttackJudgement sIns_;
		return &sIns_;
	}

	// ”»’è
	virtual bool Judgement(Enemy* enemy);
private:
};