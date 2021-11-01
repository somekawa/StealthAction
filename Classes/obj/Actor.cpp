// データ読み込み処理(XmlActDataRead関数、split関数)を担当
#include "RapidXML/rapidxml.hpp"
#include "RapidXML/rapidxml_utils.hpp"
#include "_Debug/_DebugConOut.h"
#include "Scene/GameScene.h"
#include "Gravity.h"
#include "ActionRect.h"
#include "Skill/SkillMng.h"
#include "Actor.h"

USING_NS_CC;

Actor::Actor(int hp,Layer& myLayer):hp_(hp),myLayer_(myLayer)
{
	deleteFlag_ = false;
	isAnimEnd_ = false;
	onFloor_ = false;
	isHitWall_ = false;
	isFire_ = false;
	isHitAttack_ = false;
	// 生きているかのﾌﾗｸﾞの初期化
	isAlive_ = true;
	// 攻撃しているかのフラグの初期化
	isAttacking_ = false;
	// ダメージをくらったフラグの初期化
	//onDamaged_ = false;
	animationFrame_ = 0.0f;
	// 自分自身を参照で渡し、生成
	gravity_ = std::make_unique<Gravity>(*this);
	// 攻撃の際に出現するｵﾌﾞｼﾞｪｸﾄのﾚｲﾔｰ
	attackLayer_ = Layer::create();
	// ｼｰﾝのﾚｲﾔｰにattackLayer_をぶら下げる
	myLayer.addChild(attackLayer_, 2, "attack");
}

Actor::~Actor()
{
}

void Actor::Update(void)
{
	Action();
	lpSkillMng.UpDate();
}

// アニメーションの更新関数
void Actor::UpdateAnimation(float delta)
{
	// アニメーションカウントを毎フレームdelta値を加算
	animationFrame_ += delta;
	// あるアニメーションの終了までの継続時間の格納
	float duration = lpAnimMng.GetAnimationCache(type_, currentAnimation_)->getDuration();
	// アニメーションカウントが継続時間を超えていれば
	if (animationFrame_ >= duration)
	{
		// ループフラグがtrueの場合はループ再生
		if (lpAnimMng.GetIsLoop(type_, currentAnimation_))
		{
			animationFrame_ = 0.0f;
			isAttacking_ = false;
		}
		else
		{
			// falseの場合は1回の再生
			// 1アニメーションが終了したフラグを立てる
			isAnimEnd_ = true;
		}
	}
}

void Actor::ChangeAnimation(std::string animName)
{
	// 今の動きを止める
	this->stopAllActions();

	// 現在のアニメーションを変更先のアニメーション名に変更
	currentAnimation_ = animName;
	animationFrame_ = 0.0f;
	animationFrame_int_ = 0;
	// アニメーション終了フラグをfalseに
	isAnimEnd_ = false;

	if (lpAnimMng.GetIsLoop(type_, currentAnimation_))
	{
		auto action = RepeatForever::create(Animate::create(lpAnimMng.GetAnimationCache(type_, currentAnimation_)));
		this->runAction(action);
	}
	else
	{
		this->runAction(Animate::create(lpAnimMng.GetAnimationCache(type_, currentAnimation_)));
	}
}

std::string Actor::GetAction(void)
{
	return currentAnimation_;
}

void Actor::SetAction(std::string actName)
{
	currentAnimation_ = actName;
}

void Actor::AnimRegistrator(void)
{
}

void Actor::SetPos(Vector2I pos)
{
	pos_ = pos;
}

void Actor::SetDirection(Direction dir)
{
	direction_ = dir;
}

const int Actor::GetAnimationFrameInt(void)
{
	// 毎フレーム加算される値(animationFrame)に1フレームに要する時間(delayPerUnit)を引き
	// delayPerUnitで割ると現在のフレーム値がintで取得可能　
	auto delay = lpAnimMng.GetAnimationCache(type_, currentAnimation_)->getDelayPerUnit();
	return static_cast<int>(animationFrame_ * 100.0f) / static_cast<int>(delay * 100.0f);
}

const int Actor::GetAnimationFrameInt(std::string str)
{
	auto delay = lpAnimMng.GetAnimationCache(type_, str)->getDelayPerUnit();
	return static_cast<int>(animationFrame_ * 100.0f) / static_cast<int>(delay * 100.0f);
}

void Actor::SetIsAttacking(bool flg)
{
	isAttacking_ = flg;
}

void Actor::SetAttackOffset(cocos2d::Vec2 offset)
{
	attackRect_.offset_ = offset;
}

void Actor::SetCollider(void)
{
	// 攻撃矩形情報のｾﾞﾛｸﾘ
	attackCol_ = { 0.0f, 0.0f, 0.0f, 0.0f };
	// ﾀﾞﾒｰｼﾞ矩形情報のｾﾞﾛｸﾘ
	damageCol_ = { 0.0f, 0.0f, 0.0f, 0.0f };

	for (auto col : currentCol_)
	{
		// 現在のｺﾘｼﾞｮﾝのﾃﾞｰﾀを格納しておく
		auto colData = col->GetData();
		// 攻撃矩形だったら
		if (colData.type_ == 0)
		{
			// 攻撃矩形のｻｲｽﾞ
			attackCol_.size = Size(colData.size_.x * getScale(), colData.size_.y * getScale());
			// ﾌﾟﾚｲﾔｰだったら
			if (type_ == ActorType::Player)
			{
				// 向いている方向でﾎﾟｼﾞｼｮﾝの変更をかける
				if (direction_ == Direction::Right)
				{
					// 攻撃矩形のﾎﾟｼﾞｼｮﾝ
					// direction毎に+ or - してやらないといけない
					attackCol_.origin = getPosition() + (attackCol_.size / 2.0f);
				}
				else if (direction_ == Direction::Left)
				{
					// 攻撃矩形のﾎﾟｼﾞｼｮﾝ
					// direction毎に+ or - してやらないといけない
					attackCol_.origin = Vec2(getPosition().x - (attackCol_.size.width / 2.0f), getPosition().y + attackCol_.size.height);
				}
				else
				{
					// 何も処理を行わない
				}
			}
			else
			{
				if (direction_ == Direction::Right)
				{
					// 攻撃矩形のﾎﾟｼﾞｼｮﾝ
					// direction毎に+ or - してやらないといけない
					attackCol_.origin = getPosition() + attackCol_.size;
				}
				else if (direction_ == Direction::Left)
				{
					// 攻撃矩形のﾎﾟｼﾞｼｮﾝ
					// direction毎に+ or - してやらないといけない
					attackCol_.origin = Vec2(getPosition().x - attackCol_.size.width,getPosition().y + attackCol_.size.height);
				}
				else
				{
					// 何も処理を行わない
				}
			}
		}
		else
		{
			// ﾀﾞﾒｰｼﾞ矩形のｻｲｽﾞ
			damageCol_.size = Size(colData.size_.x * getScale(), colData.size_.y * getScale());
			if (type_ == ActorType::Player)
			{
				// ﾀﾞﾒｰｼﾞ矩形のﾎﾟｼﾞｼｮﾝ
				damageCol_.origin = Vec2(getPosition().x, getPosition().y);
			}
			else
			{
				if (direction_ == Direction::Left)
				{
					// ﾀﾞﾒｰｼﾞ矩形のﾎﾟｼﾞｼｮﾝ
					damageCol_.origin = Vec2(getPosition().x + (damageCol_.size.width / 4.0f), getPosition().y);
				}
				else if (direction_ == Direction::Right)
				{
					// ﾀﾞﾒｰｼﾞ矩形のﾎﾟｼﾞｼｮﾝ
					damageCol_.origin = Vec2(getPosition().x - (damageCol_.size.width / 4.0f), getPosition().y);
				}
				else
				{
					// 何も処理を行わない
				}
			}
		}
	}
}

bool Actor::OnHit(const cocos2d::Rect& collision)
{
	bool flg = false;

	if (collision.size.width > 0.0f && collision.size.height > 0.0f)
	{
		flg = true;
	}
	else
	{
		return false;
	}

	if (!isHitAttack_)
	{
		// 矩形間の距離を測る
		auto distance = Vec2(collision.origin.x - damageCol_.origin.x, collision.origin.y - damageCol_.origin.y);
		auto sizediff = Vec2((damageCol_.size.width / 2.0f) + (collision.size.width / 2.0f),
			(damageCol_.size.height / 2.0f) + (collision.size.height / 2.0f));

		if (abs(distance.x) <= sizediff.x && abs(distance.y) <= sizediff.y)
		{
			isHitAttack_ = true;
			return true;
		}
	}
	return false;
}

void Actor::SetIsHitAttack(bool flg)
{
	isHitAttack_ = flg;
}

void Actor::XmlActDataRead(std::string string, OPRT_state* oprt)
{
	std::string path = "../Resources/Data/moduleData/" + string + ".xml";

	// xml読み込み
	rapidxml::xml_document<> doc;
	rapidxml::file<> file(path.c_str());
	doc.parse<0>(file.data(), file.size());
	rapidxml::xml_node<>* parentNode = doc.first_node("data");

	// モジュール数をxmlデータから取得
	auto sumData = parentNode->first_node("moduleSum")->first_attribute()->value();
	std::vector<ActModule> act;
	act.resize(std::atoi(sumData));

	std::map<std::string, std::function<void(std::string, int num)>> dataFunc;

	dataFunc["actName"] = [&](std::string str, int num) {
		act[num].actName = str;
	};

	dataFunc["state"] = [&](std::string str, int num) {
		act[num].state = oprt;
	};

	dataFunc["button"] = [&](std::string str, int num) {
		act[num].button = static_cast<BUTTON>(std::atoi(str.c_str()));
	};

	dataFunc["vel"] = [&](std::string str, int num) {
		auto string = split(str, ",");	// カンマ区切り処理
		act[num].vel = Vec2(static_cast<float>(std::atof(string[0].c_str())), static_cast<float>(std::atof(string[1].c_str())));
	};

	dataFunc["checkPoint1"] = [&](std::string str, int num) {
		auto string = split(str, ",");
		act[num].checkPoint1 = Vec2(static_cast<float>(std::atof(string[0].c_str())), static_cast<float>(std::atof(string[1].c_str())));
	};

	dataFunc["checkPoint2"] = [&](std::string str, int num) {
		auto string = split(str, ",");
		act[num].checkPoint2 = Vec2(static_cast<float>(std::atof(string[0].c_str())), static_cast<float>(std::atof(string[1].c_str())));
	};

	dataFunc["checkPoint3"] = [&](std::string str, int num) {
		auto string = split(str, ",");
		act[num].checkPoint3 = Vec2(static_cast<float>(std::atof(string[0].c_str())), static_cast<float>(std::atof(string[1].c_str())));
	};

	dataFunc["checkPoint4"] = [&](std::string str, int num) {
		auto string = split(str, ",");
		act[num].checkPoint4 = Vec2(static_cast<float>(std::atof(string[0].c_str())), static_cast<float>(std::atof(string[1].c_str())));
	};

	dataFunc["touch"] = [&](std::string str, int num) {
		act[num].touch = static_cast<TOUCH_TIMMING>(std::atoi(str.c_str()));
	};

	dataFunc["gravity"] = [&](std::string str, int num) {
		auto string = split(str, ",");
		act[num].gravity = Vec2(static_cast<float>(std::atof(string[0].c_str())), static_cast<float>(std::atof(string[1].c_str())));
	};

	dataFunc["jumpVel"] = [&](std::string str, int num) {
		auto string = split(str, ",");
		act[num].jumpVel = Vec2(static_cast<float>(std::atof(string[0].c_str())), static_cast<float>(std::atof(string[1].c_str())));
	};

	dataFunc["flipFlg"] = [&](std::string str, int num) {
		act[num].flipFlg = static_cast<bool>(std::atoi(str.c_str()));
	};

	dataFunc["blackList"] = [&](std::string str, int num) {
		act[num].blackList.emplace_back(str);
	};

	dataFunc["whiteList"] = [&](std::string str, int num) {
		act[num].whiteList.emplace_back(str);
	};

	int num = 0;
	for (rapidxml::xml_node<char>* moduleItr = parentNode->first_node("module"); moduleItr != nullptr; moduleItr = moduleItr->next_sibling())
	{
		for (rapidxml::xml_attribute<char>* attr = moduleItr->first_node("act")->first_attribute(); attr; attr = attr->next_attribute())
		{
			// blackList以外の処理(name : =の左辺,value : =の右辺("〇〇"の中身))
			dataFunc[attr->name()](attr->value(), num);
		}
		for (rapidxml::xml_node<char>* itr = moduleItr->first_node("blackList")->first_node("name"); itr != nullptr; itr = itr->next_sibling())
		{
			// blackListの処理(blackListに登録したいactNameが入っている)
			dataFunc["blackList"](itr->value(), num);
		}
		for (rapidxml::xml_node<char>* itr = moduleItr->first_node("whiteList")->first_node("name"); itr != nullptr; itr = itr->next_sibling())
		{
			// whiteListの処理(whiteListに登録したいactNameが入っている)
			dataFunc["whiteList"](itr->value(), num);
		}

		// アクションの登録
		actCtl_.InitAct(type_, static_cast<AnimationType>(num + 1), act[num]);
		num++;
	}

	// 更新関数の登録
	actCtl_.InitUpdater(type_);
}

std::vector<std::string> Actor::split(std::string str, std::string separator)
{
	if (separator == "") return { str };
	std::vector<std::string> result;
	std::string tstr = str + separator;
	auto l = tstr.length(), sl = separator.length();
	std::string::size_type pos = 0, prev = 0;

	for (; pos < l && (pos = tstr.find(separator, pos)) != std::string::npos; prev = (pos += sl))
	{
		result.emplace_back(tstr, prev, pos - prev);
	}
	return result;
}