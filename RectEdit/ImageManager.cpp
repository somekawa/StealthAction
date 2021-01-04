#include <DxLib.h>

#include "ImageManager.h"

bool ImageManager::Load(const ActorType& actor,std::string filepath,Vector2I divsize,Vector2I divcount)
{
	SetUp(actor);
	if (resources_[static_cast<int>(actor)].imageHandle.find(filepath) ==
		resources_[static_cast<int>(actor)].imageHandle.end())
	{
		switch (actor)
		{
		case ActorType::Player:
			resources_[static_cast<int>(actor)].typeName = "Player";
			break;
		case ActorType::Imp:
			resources_[static_cast<int>(actor)].typeName = "imp";
			break;
		case ActorType::Assassin:
			resources_[static_cast<int>(actor)].typeName = "assassin";
			break;
		case ActorType::TwistedCultist:
			resources_[static_cast<int>(actor)].typeName = "twistedCultist";
			break;
		case ActorType::Cultist:
			resources_[static_cast<int>(actor)].typeName = "cultist";
			break;
		case ActorType::Max:
		default:
			return false;
			break;
		}

		auto pathName = "Images/" + resources_[static_cast<int>(actor)].typeName +
			"_" + filepath + ".png";
		
		resources_[static_cast<int>(actor)].imageHandle[filepath].resize(divcount.x*divcount.y);

		LoadDivGraph(pathName.c_str(), divcount.x * divcount.y,
			divcount.x, divcount.y, divsize.x, divsize.y,
			&resources_[static_cast<int>(actor)].imageHandle[filepath][0]);
	}
	return true;
}

const VecInt& ImageManager::GetID(const ActorType& actor, std::string filepath)
{
	return resources_[static_cast<int>(actor)].imageHandle[filepath];
}

void ImageManager::SetUp(ActorType actor)
{
	// êVÇµÇ¢±∆“∞ºÆ›ÇÃâÊëúÇí«â¡ÇµÇΩÇÁÇ±Ç±Ç…Ç‡í«â¡ÇµÇΩ±∆“∞ºÆ›Ç…ëŒâûÇ∑ÇÈactionNameSetÇtry_emplaceÇ∑ÇÈ
	// try_emplace(±∆“∞ºÆ›ñº,ÇªÇÃ±∆“∞ºÆ›ÇÃëçÉtÉåÅ[ÉÄêî)
	switch (actor)
	{
	case ActorType::Player:
		resources_[static_cast<int>(actor)].actionNameSet.try_emplace("attack1",10);
		resources_[static_cast<int>(actor)].actionNameSet.try_emplace("attack2",10);
		resources_[static_cast<int>(actor)].actionNameSet.try_emplace("attack3",10);
		resources_[static_cast<int>(actor)].actionNameSet.try_emplace("run",10);
		resources_[static_cast<int>(actor)].actionNameSet.try_emplace("jump",4);
		resources_[static_cast<int>(actor)].actionNameSet.try_emplace("fall",4);
		resources_[static_cast<int>(actor)].actionNameSet.try_emplace("idle", 3);

		break;
	case ActorType::Imp:
		resources_[static_cast<int>(actor)].actionNameSet.try_emplace("attackFirst", 6);
		resources_[static_cast<int>(actor)].actionNameSet.try_emplace("death", 5);
		resources_[static_cast<int>(actor)].actionNameSet.try_emplace("walk", 6);
		break;
	case ActorType::Assassin:
		resources_[static_cast<int>(actor)].actionNameSet.try_emplace("ambush", 13);
		resources_[static_cast<int>(actor)].actionNameSet.try_emplace("arise", 8);
		resources_[static_cast<int>(actor)].actionNameSet.try_emplace("attack", 13);
		resources_[static_cast<int>(actor)].actionNameSet.try_emplace("death", 16);
		resources_[static_cast<int>(actor)].actionNameSet.try_emplace("death_noEffect", 16);
		resources_[static_cast<int>(actor)].actionNameSet.try_emplace("fall", 3);
		resources_[static_cast<int>(actor)].actionNameSet.try_emplace("hit", 3);
		resources_[static_cast<int>(actor)].actionNameSet.try_emplace("hit_noEffect", 3);
		resources_[static_cast<int>(actor)].actionNameSet.try_emplace("idle", 8);
		resources_[static_cast<int>(actor)].actionNameSet.try_emplace("jump", 3);
		resources_[static_cast<int>(actor)].actionNameSet.try_emplace("run", 8);
		resources_[static_cast<int>(actor)].actionNameSet.try_emplace("run_blink", 8);
		resources_[static_cast<int>(actor)].actionNameSet.try_emplace("uptoFall", 2);
		resources_[static_cast<int>(actor)].actionNameSet.try_emplace("vanish", 8);
		break;
	case ActorType::TwistedCultist:
		resources_[static_cast<int>(actor)].actionNameSet.try_emplace("attack", 7);
		resources_[static_cast<int>(actor)].actionNameSet.try_emplace("death", 12);
		resources_[static_cast<int>(actor)].actionNameSet.try_emplace("death_noEffect", 3);
		resources_[static_cast<int>(actor)].actionNameSet.try_emplace("fall", 3);
		resources_[static_cast<int>(actor)].actionNameSet.try_emplace("hit", 3);
		resources_[static_cast<int>(actor)].actionNameSet.try_emplace("hit_noEffect", 3);
		resources_[static_cast<int>(actor)].actionNameSet.try_emplace("idle", 6);
		resources_[static_cast<int>(actor)].actionNameSet.try_emplace("idle_closeEye", 6);
		resources_[static_cast<int>(actor)].actionNameSet.try_emplace("jump", 3);
		resources_[static_cast<int>(actor)].actionNameSet.try_emplace("twist", 12);
		resources_[static_cast<int>(actor)].actionNameSet.try_emplace("uptoFall", 2);
		resources_[static_cast<int>(actor)].actionNameSet.try_emplace("walk", 8);
		resources_[static_cast<int>(actor)].actionNameSet.try_emplace("walk_closeEye", 8);
	case ActorType::Cultist:
		resources_[static_cast<int>(actor)].actionNameSet.try_emplace("attack", 10);
		resources_[static_cast<int>(actor)].actionNameSet.try_emplace("death", 12);
		resources_[static_cast<int>(actor)].actionNameSet.try_emplace("death_noEffect", 12);
		resources_[static_cast<int>(actor)].actionNameSet.try_emplace("fall", 3);
		resources_[static_cast<int>(actor)].actionNameSet.try_emplace("hit", 3);
		resources_[static_cast<int>(actor)].actionNameSet.try_emplace("hit_noEffect", 3);
		resources_[static_cast<int>(actor)].actionNameSet.try_emplace("idle", 6);
		resources_[static_cast<int>(actor)].actionNameSet.try_emplace("jump", 3);
		resources_[static_cast<int>(actor)].actionNameSet.try_emplace("uptoFall", 2);
		resources_[static_cast<int>(actor)].actionNameSet.try_emplace("walk", 8);
	case ActorType::Max:
		break;
	default:
		break;
	}
}
