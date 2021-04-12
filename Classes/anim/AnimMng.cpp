// データ読み込み処理を担当
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
		// lightとdarkでファイルを分ける
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

	// アニメーションキャッシュはシングルトン
	AnimationCache *animationCache = AnimationCache::getInstance();

	//スプライトシートの準備
	const auto cache = SpriteFrameCache::getInstance();

	// パス指定
	const auto pListStr = actorName + "_" + animName;
	cache->addSpriteFramesWithFile(pListStr + ".plist");

	// アニメーション画像追加
	Animation* animation = Animation::create();
	
	for (int i = 0; i < frame; i++)
	{
		const std::string string = animName + "%d.png";		// plistの中だからパスじゃない
		const std::string str = StringUtils::format(string.c_str(), i);
		SpriteFrame* sprite = cache->getSpriteFrameByName(name + "_" + str);
		if (sprite == nullptr)
		{
			log("sprite is null");
		}
		animation->addSpriteFrame(sprite);
	}

	// アニメーションの間隔
	animation->setDelayPerUnit(duration);

	// アニメーション終了後に最初に戻すかどうか
	animation->setRestoreOriginalFrame(true);

	// 出来たアニメーションをキャッシュに登録
	animationCache->addAnimation(animation, name + "_" + animName);

	// 1アニメーションのキャッシュデータを格納する処理
	CacheRegistration(animationCache, type,name + "_" + animName,isLoop);

	if (frameNum_[static_cast<int>(type)].find(animName) == frameNum_[static_cast<int>(type)].end())
	{
		frameNum_[static_cast<int>(type)].emplace(animName, frame);
	}
}

void AnimMng::addAnimationCache(std::string name, std::string animName, int frame, float duration, bool isLoop)
{
	const std::string fileName = name + "/" + animName;
	// アニメーションキャッシュはシングルトン
	AnimationCache* animationCache = AnimationCache::getInstance();

	//スプライトシートの準備
	const auto cache = SpriteFrameCache::getInstance();

	cache->addSpriteFramesWithFile(fileName + ".plist");

	// アニメーション画像追加
	Animation* animation = Animation::create();

	for (int i = 0; i < frame; i++)
	{
		const std::string string = "%d.png";		// plistの中だからパスじゃない
		const std::string str = StringUtils::format(string.c_str(), i);
		SpriteFrame* sprite = cache->getSpriteFrameByName(animName + "_" + str);

		animation->addSpriteFrame(sprite);
	}

	// アニメーションの間隔
	animation->setDelayPerUnit(duration);

	// アニメーション終了後に最初に戻すかどうか
	animation->setRestoreOriginalFrame(true);

	// 出来たアニメーションをキャッシュに登録
	animationCache->addAnimation(animation,animName);

	// 1アニメーションのキャッシュデータを格納する処理
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
	// 今の動きを止める
	sprite.stopAllActions();
	
	// キーによるアニメーションの取り出し
	Animation* animation = caches_[static_cast<int>(type)][name];
	// フレームアニメーションは繰り返し
	if (loop)
	{
		RepeatForever* action = RepeatForever::create(Animate::create(animation));
		//アニメーションを実行
		sprite.runAction(action);
	}
	else
	{
		auto action_ = Repeat::create(Animate::create(animation), 1);
		//アニメーションを実行
		sprite.runAction(action_);
	}
}

void AnimMng::CacheRegistration(cocos2d::AnimationCache* animCache, const ActorType& type, std::string animName,bool isLoop)
{
	if (caches_[static_cast<int>(type)].count(animName) <= 0)
	{
		// キャラのタイプ別のアニメーションキャッシュに1アニメーションデータを格納
		caches_[static_cast<int>(type)].emplace(animName, animCache->getAnimation(animName));
		// ｱﾆﾒｰｼｮﾝの文字列格納
		animations_[static_cast<int>(type)].emplace_back(animName);
		// 1アニメーションにかかる時間の格納
		animMaxFrame_[static_cast<int>(type)].emplace(animName, caches_[static_cast<int>(type)][animName]->getDelayPerUnit() *
			caches_[static_cast<int>(type)][animName]->getFrames().size());

		// ループフラグを設定
		isLoop_[static_cast<int>(type)][animName] = isLoop;
	}
}

void AnimMng::CacheRegistration(cocos2d::AnimationCache* animCache, std::string animName, bool isLoop)
{
	if (cachesExceptCharacter_.count(animName) <= 0)
	{
		// キャラのタイプ別のアニメーションキャッシュに1アニメーションデータを格納
		cachesExceptCharacter_.emplace(animName, animCache->getAnimation(animName));
		// ループフラグを設定
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

	// xml読み込み
	rapidxml::xml_document<> doc;
	rapidxml::file<> file(path.c_str());
	doc.parse<0>(file.data());
	rapidxml::xml_node<>* parentNode = doc.first_node("data");

	// 構造体をvectorにし、xmlからデータ数を取得しリサイズする
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
			// nameがdataFuncのキーと一致していたらnumで指定した配列場所にデータを格納する
			dataFunc[attr->name()](attr->value(), num);
		}
		num++;
	}

	// vectorに格納したデータを回してアニメーション登録を行う
	for (auto data : animStVec)
	{
		addAnimationCache(parentNode->first_node("path")->value(), data.animName, data.frame, data.duration, type, data.loop);
	}
}

std::vector<PlParam> AnimMng::XmlPlParamRead(void)
{
	// 返り値でデータを格納したものを返す
	const std::string path = "../Resources/Data/AnimData/playerParam.xml";

	rapidxml::xml_document<> doc;
	rapidxml::file<> file(path.c_str());
	doc.parse<0>(file.data());
	rapidxml::xml_node<>* parentNode = doc.first_node("data");

	// 構造体をvectorにし、xmlからデータ数を取得しリサイズする
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

	// データ読み込み
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
		// 一時保存データだったものを登録する
		param[num].chainAnimName = std::move(attackDataKeep);
		num++;
	}

	return param;
}