//#include "SkillMng.h"
#include "SkillBase.h"
#include "SkillCode/SkillB.h"

//SkillBase::SkillBase()
//{
//	skillMng_ = std::make_shared<SkillMng>();
//}

std::vector<SkillBase*> SkillBase::skillList_;

cocos2d::Sprite* SkillBase::createSkillBase()
{
	SkillBase* pRet = new(std::nothrow)SkillBase();
	skillList_.clear();
	if (pRet)
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

SkillBase::~SkillBase()
{
}

void SkillBase::AddActiveSkill(SkillBase* ptr)
{
	skillList_.push_back(ptr);
}

void SkillBase::RemoveActiveSkill(void)
{
	// activationがfalseなら消すver.
	//for (auto itr = skillList_.begin();itr != skillList_.end();)
	//{
	//	if ((*itr)->param.activation == false)
	//	{
	//		(*itr)->removeFromParent();
	//		itr = skillList_.erase(itr);
	//	}
	//	else
	//	{
	//		itr++;
	//	}
	//}

	// 削除OKになったスキルだけを消す
	for (auto itr = skillList_.begin(); itr != skillList_.end();)
	{
		if ((*itr)->param.removeFlg == true)
		{
			(*itr)->removeFromParent();
			itr = skillList_.erase(itr);
		}
		else
		{
			++itr;
		}
	}
}

void SkillBase::UpDate(float delta)
{
	effectData_ = cocos2d::Rect();
	for (auto data : skillList_)
	{
		effectData_ = cocos2d::Rect(data->fx_.sprite_->getPosition() + data->GetEffectData().size/2, data->GetEffectData().size);
		data->UpDate(delta);
	}
	lpEffectMng.Update(delta);
	RemoveActiveSkill();
}

void SkillBase::Render(void)
{
}

std::vector<SkillBase*> SkillBase::GetSkillList(void)
{
	return skillList_;
}

cocos2d::Vec2 SkillBase::GetPlayerPos(void)
{
	return playerPos_;
}

const Direction& SkillBase::GetPlayerDirection(void)
{
	return playerDirection_;
}

cocos2d::Vec2 SkillBase::GetTargetPos(void)
{
	return targetPos_;
}

void SkillBase::SetPlayerPos(cocos2d::Vec2 playerPos)
{
	playerPos_ = playerPos;
}

void SkillBase::SetTargetPos(cocos2d::Vec2 targetPos)
{
	targetPos_ = targetPos;
}

void SkillBase::SetPlayerDirection(const Direction& direction)
{
	playerDirection_ = direction;
}

bool SkillBase::GetAllSkillActivate(void)
{
	bool tmp = false;

	if (skillList_.size() > 0)
	{
		for (auto itr = skillList_.begin(); itr != skillList_.end();)
		{
			tmp |= (*itr)->param.activation;
			++itr;
		}
	}

	return tmp;
}

bool SkillBase::GetSkillCT(std::string name)
{
	bool tmp = true;

	if (skillList_.size() > 0)
	{
		for (auto itr = skillList_.begin(); itr != skillList_.end();)
		{
			// 探しているskillの名前と一致したら
			if ((*itr)->param.name == name)
			{
				// そのskillのCTが0より大きいならfalse
				if ((*itr)->param.ct > 0)
				{
					tmp = false;
					break;		// 目当てのスキルが見つかったからbreakで抜ける
				}
				else
				{
					++itr;
				}
			}
			else
			{
				// 探しているskillと一致しないときは回し続ける
				++itr;
			}
		}
	}

	return tmp;
}

int SkillBase::GetSkillCTTime(std::string name)
{
	int time = 0;

	if (skillList_.size() > 0)
	{
		for (auto itr = skillList_.begin(); itr != skillList_.end();)
		{
			// 探しているskillの名前と一致したら
			if ((*itr)->param.name == name)
			{
				// そのskillのCTが0より大きいならfalse
				if ((*itr)->param.ct > 0)
				{
					time = (*itr)->param.ct;
					break;		// 目当てのスキルが見つかったからbreakで抜ける
				}
				else
				{
					++itr;
				}
			}
			else
			{
				// 探しているskillと一致しないときは回し続ける
				++itr;
			}
		}
	}

	return time / 60;
}

