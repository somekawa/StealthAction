// �f�[�^�ǂݍ��ݏ�����S��
#include "RapidXML/rapidxml.hpp"
#include "RapidXML/rapidxml_utils.hpp"
#include "AnimMng.h"

USING_NS_CC;

std::unique_ptr<AnimMng>AnimMng::s_instance(new AnimMng);

AnimMng::AnimMng()
{
}

AnimMng::~AnimMng()
{
}

void AnimMng::addAnimationCache(std::string actorName, std::string animName, int frame, float duration, ActorType type,bool isLoop)
{
	actorNames_[static_cast<int>(type)] = actorName;
	std::string name = "";
	switch (type)
	{
	case ActorType::Player:
		// light��dark�Ńt�@�C���𕪂���
		if (actorName == "image/PlayerAnimationAsset/player/Light/player_Light")
		{
			name = "player_Light";
		}
		else
		{
			name = "player_Dark";
		}
		break;
	case ActorType::Assassin:
		name = "assassin";
		break;
	case ActorType::TwistedCultist:
		name = "twistedCultist";
		break;
	case ActorType::Cultist:
		name = "cultist";
		break;
	case ActorType::Max:
		break;
	default:
		break;
	}

	// �A�j���[�V�����L���b�V���̓V���O���g��
	AnimationCache *animationCache = AnimationCache::getInstance();

	//�X�v���C�g�V�[�g�̏���
	const auto cache = SpriteFrameCache::getInstance();

	// �p�X�w��
	const auto pListStr = actorName + "_" + animName;
	cache->addSpriteFramesWithFile(pListStr + ".plist");

	// �A�j���[�V�����摜�ǉ�
	Animation* animation = Animation::create();
	
	for (int i = 0; i < frame; i++)
	{
		const std::string string = animName + "%d.png";		// plist�̒�������p�X����Ȃ�
		const std::string str = StringUtils::format(string.c_str(), i);
		SpriteFrame* sprite = cache->getSpriteFrameByName(name + "_" + str);
		if (sprite == nullptr)
		{
			log("sprite is null");
		}
		animation->addSpriteFrame(sprite);
	}

	// �A�j���[�V�����̊Ԋu
	animation->setDelayPerUnit(duration);

	// �A�j���[�V�����I����ɍŏ��ɖ߂����ǂ���
	animation->setRestoreOriginalFrame(true);

	// �o�����A�j���[�V�������L���b�V���ɓo�^
	animationCache->addAnimation(animation, name + "_" + animName);

	// 1�A�j���[�V�����̃L���b�V���f�[�^���i�[���鏈��
	CacheRegistration(animationCache, type,name + "_" + animName,isLoop);

	if (frameNum_[static_cast<int>(type)].find(animName) == frameNum_[static_cast<int>(type)].end())
	{
		frameNum_[static_cast<int>(type)].emplace(animName, frame);
	}
}

void AnimMng::addAnimationCache(std::string name, std::string animName, int frame, float duration, bool isLoop)
{
	const std::string fileName = name + "/" + animName;
	// �A�j���[�V�����L���b�V���̓V���O���g��
	AnimationCache* animationCache = AnimationCache::getInstance();

	//�X�v���C�g�V�[�g�̏���
	const auto cache = SpriteFrameCache::getInstance();

	cache->addSpriteFramesWithFile(fileName + ".plist");

	// �A�j���[�V�����摜�ǉ�
	Animation* animation = Animation::create();

	for (int i = 0; i < frame; i++)
	{
		const std::string string = "%d.png";		// plist�̒�������p�X����Ȃ�
		const std::string str = StringUtils::format(string.c_str(), i);
		SpriteFrame* sprite = cache->getSpriteFrameByName(animName + "_" + str);

		animation->addSpriteFrame(sprite);
	}

	// �A�j���[�V�����̊Ԋu
	animation->setDelayPerUnit(duration);

	// �A�j���[�V�����I����ɍŏ��ɖ߂����ǂ���
	animation->setRestoreOriginalFrame(true);

	// �o�����A�j���[�V�������L���b�V���ɓo�^
	animationCache->addAnimation(animation,animName);

	// 1�A�j���[�V�����̃L���b�V���f�[�^���i�[���鏈��
	CacheRegistration(animationCache,animName, isLoop);
}

void AnimMng::InitAnimation(cocos2d::Sprite& sprite, ActorType type, std::string animName)
{
	Animation* animation = caches_[static_cast<int>(type)][animName];

	RepeatForever* action = RepeatForever::create(Animate::create(animation));

	sprite.runAction(action);
}

void AnimMng::ChangeAnimation(cocos2d::Sprite& sprite, std::string name, bool loop, ActorType type)
{
	// ���̓������~�߂�
	sprite.stopAllActions();
	
	// �L�[�ɂ��A�j���[�V�����̎��o��
	Animation* animation = caches_[static_cast<int>(type)][name];
	// �t���[���A�j���[�V�����͌J��Ԃ�
	if (loop)
	{
		RepeatForever* action = RepeatForever::create(Animate::create(animation));
		//�A�j���[�V���������s
		sprite.runAction(action);
	}
	else
	{
		auto action_ = Repeat::create(Animate::create(animation), 1);
		//�A�j���[�V���������s
		sprite.runAction(action_);
	}
}

void AnimMng::CacheRegistration(cocos2d::AnimationCache* animCache, const ActorType& type, std::string animName,bool isLoop)
{
	if (caches_[static_cast<int>(type)].count(animName) <= 0)
	{
		// �L�����̃^�C�v�ʂ̃A�j���[�V�����L���b�V����1�A�j���[�V�����f�[�^���i�[
		caches_[static_cast<int>(type)].emplace(animName, animCache->getAnimation(animName));
		// ��Ұ��݂̕�����i�[
		animations_[static_cast<int>(type)].emplace_back(animName);
		// 1�A�j���[�V�����ɂ����鎞�Ԃ̊i�[
		animMaxFrame_[static_cast<int>(type)].emplace(animName, caches_[static_cast<int>(type)][animName]->getDelayPerUnit() *
			caches_[static_cast<int>(type)][animName]->getFrames().size());

		// ���[�v�t���O��ݒ�
		isLoop_[static_cast<int>(type)][animName] = isLoop;
	}
}

void AnimMng::CacheRegistration(cocos2d::AnimationCache* animCache, std::string animName, bool isLoop)
{
	if (cachesExceptCharacter_.count(animName) <= 0)
	{
		// �L�����̃^�C�v�ʂ̃A�j���[�V�����L���b�V����1�A�j���[�V�����f�[�^���i�[
		cachesExceptCharacter_.emplace(animName, animCache->getAnimation(animName));
		// ���[�v�t���O��ݒ�
		isLoopExceptCharacter_.emplace(animName, isLoop);
	}
}

bool AnimMng::IsAnimEnd(const float& delta, ActorType type, std::string animName)
{
	if (delta >= animMaxFrame_[static_cast<int>(type)][animName] - 0.2f)
	{
		return true;
	}
	return false;
}

void AnimMng::AnimDataClear(void)
{
	for (auto& data : caches_)
	{
		data.clear();
	}
}

void AnimMng::FireBallClear(void)
{
	cachesExceptCharacter_.clear();
}

int AnimMng::GetAnimationMaxNum(ActorType type, std::string animationName)
{
	return caches_[static_cast<int>(type)][animationName]->getFrames().size();
}

void AnimMng::XmlAnimDataRead(std::string string, ActorType type)
{
	const std::string path = "../Resources/Data/AnimData/" + string + ".xml";

	// xml�ǂݍ���
	rapidxml::xml_document<> doc;
	rapidxml::file<> file(path.c_str());
	doc.parse<0>(file.data());
	rapidxml::xml_node<>* parentNode = doc.first_node("data");

	// �\���̂�vector�ɂ��Axml����f�[�^�����擾�����T�C�Y����
	std::vector<AnimSt> animStVec;
	animStVec.resize(std::atoi(parentNode->first_node("dataSum")->value()));

	std::map<std::string, std::function<void(std::string, int num)>> dataFunc;
	dataFunc["animName"] = [&](std::string str, int num) {
		animStVec[num].animName = str;
	};
	dataFunc["frame"] = [&](std::string str, int num) {
		animStVec[num].frame = std::atoi(str.c_str());
	};
	dataFunc["duration"] = [&](std::string str, int num) {
		animStVec[num].duration = static_cast<float>(std::atof(str.c_str()));
	};
	dataFunc["loop"] = [&](std::string str, int num) {
		animStVec[num].loop = static_cast<bool>(std::atoi(str.c_str()));
	};

	int num = 0;
	for (rapidxml::xml_node<char>* animItr = parentNode->first_node("animation"); animItr != nullptr; animItr = animItr->next_sibling())
	{
		for (rapidxml::xml_attribute<char>* attr = animItr->first_node("anim")->first_attribute(); attr; attr = attr->next_attribute())
		{
			// name��dataFunc�̃L�[�ƈ�v���Ă�����num�Ŏw�肵���z��ꏊ�Ƀf�[�^���i�[����
			dataFunc[attr->name()](attr->value(), num);
		}
		num++;
	}

	// vector�Ɋi�[�����f�[�^���񂵂ăA�j���[�V�����o�^���s��
	for (auto data : animStVec)
	{
		addAnimationCache(parentNode->first_node("path")->value(), data.animName, data.frame, data.duration, type, data.loop);
	}
}

std::vector<PlParam> AnimMng::XmlPlParamRead(void)
{
	// �Ԃ�l�Ńf�[�^���i�[�������̂�Ԃ�
	const std::string path = "../Resources/Data/AnimData/playerParam.xml";

	rapidxml::xml_document<> doc;
	rapidxml::file<> file(path.c_str());
	doc.parse<0>(file.data());
	rapidxml::xml_node<>* parentNode = doc.first_node("data");

	// �\���̂�vector�ɂ��Axml����f�[�^�����擾�����T�C�Y����
	std::vector<PlParam> param;
	param.resize(2);

	std::vector<std::pair<std::string,float>> attackDataKeep;

	std::map<std::string, std::function<void(std::string, int num)>> dataFunc;
	dataFunc["modeName"] = [&](std::string str, int num) {
		param[num].modeName = str;
	};
	dataFunc["runSpeedUp"] = [&](std::string str, int num) {
		param[num].runSpeedUp = static_cast<float>(std::atof(str.c_str()));
	};
	dataFunc["attackDef"] = [&](std::string str, int num) {
		param[num].attackDef  = static_cast<float>(std::atof(str.c_str()));
	};
	dataFunc["attackChain"] = [&](std::string str, int num) {
		param[num].attackChain = std::atoi(str.c_str());
		attackDataKeep.resize(param[num].attackChain);
	};
	dataFunc["animName"] = [&](std::string str, int num) {
		attackDataKeep[num].first = str;
	};
	dataFunc["attackBonus"] = [&](std::string str, int num) {
		attackDataKeep[num].second = static_cast<float>(std::atof(str.c_str()));
	};

	// �f�[�^�ǂݍ���
	int num = 0;
	for (rapidxml::xml_node<char>* paramItr = parentNode->first_node("color"); paramItr != nullptr; paramItr = paramItr->next_sibling())
	{
		for (rapidxml::xml_attribute<char>* attr = paramItr->first_node("param")->first_attribute(); attr; attr = attr->next_attribute())
		{
			dataFunc[attr->name()](attr->value(), num);
		}

		int num2 = 0;
		for (rapidxml::xml_node<char>* tmpItr = paramItr->first_node("chain"); tmpItr != nullptr; tmpItr = tmpItr->next_sibling())
		{
			for (rapidxml::xml_attribute<char>* attr = tmpItr->first_attribute(); attr; attr = attr->next_attribute())
			{
				dataFunc[attr->name()](attr->value(), num2);
			}
			num2++;
		}
		// �ꎞ�ۑ��f�[�^���������̂�o�^����
		param[num].chainAnimName = std::move(attackDataKeep);
		num++;
	}

	return param;
}