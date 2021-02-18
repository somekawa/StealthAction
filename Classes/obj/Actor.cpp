#include "Actor.h"
#include "Scene/GameScene.h"
#include "Gravity.h"
#include "_Debug/_DebugConOut.h"
#include "ActionRect.h"
#include "Skill/SkillMng.h"

USING_NS_CC;

//Actor::Actor()
//{
//}

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
	onDamaged_ = false;
	animationFrame_ = 0.0f;
	// 自分自身を参照で渡し、生成
	gravity_ = std::make_unique<Gravity>(*this);

	// 攻撃の際に出現するｵﾌﾞｼﾞｪｸﾄのﾚｲﾔｰ
	attackLayer_ = Layer::create();
	// ｼｰﾝのﾚｲﾔｰにattackLayer_をぶら下げる
	myLayer.addChild(attackLayer_, 2, "attack");

	std::list<std::string> path;
	path.push_back("skill_data");
	fileLoad_ = lpFileLoder.Directory(path);							// playerとenemyの階層
	// 初期化のためにchangealldataを使用する
	for (auto data : fileLoad_)
	{
		// データのテスト
		lpSkillMng.ChangeAllData(data.first, { "dataTest",1,0,180 });
	}
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
	auto duration = lpAnimMng.GetAnimationCache(type_, currentAnimation_)->getDuration();
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
	TRACE("animEndFlag:%d\n", isAnimEnd_);
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

void Actor::SetPos(Vector2I pos)
{
	pos_ = pos;
}

void Actor::SetDirection(Direction dir)
{
	direction_ = dir;
}

const int& Actor::GetAnimationFrameInt(void)
{
	// 毎フレーム加算される値(animationFrame)に1フレームに要する時間(delayPerUnit)を引き
	// delayPerUnitで割ると現在のフレーム値がintで取得可能　
	auto delay = lpAnimMng.GetAnimationCache(type_, currentAnimation_)->getDelayPerUnit();
	auto val = (int)(animationFrame_ * 100.0f) / (int)(delay * 100.0f);
	return val;
}

const int& Actor::GetAnimationFrameInt(std::string str)
{
	// 毎フレーム加算される値(animationFrame)に1フレームに要する時間(delayPerUnit)を引き
	// delayPerUnitで割ると現在のフレーム値がintで取得可能　
	auto delay = lpAnimMng.GetAnimationCache(type_, str)->getDelayPerUnit();
	auto val = (int)(animationFrame_ * 100.0f) / (int)(delay * 100.0f);
	return val;
}

void Actor::CheckMapObjHit(float delta)
{
	auto director = Director::getInstance();
	auto a = director->getRunningScene()->getChildByTag((int)zOlder::BG);
	if ((TMXLayer*)director->getRunningScene()->getChildByTag((int)zOlder::BG)->getChildByName("MapData") == nullptr)
	{
		return;
	}
	auto position = this->getPosition();

	auto CollisionData = (TMXLayer*)director->getRunningScene()->getChildByTag((int)zOlder::BG)->getChildByName("MapData")->getChildByName("col");
	// コリジョンレイヤーの縦横
	auto ColSize = CollisionData->getLayerSize();
	const int chipSize = CollisionData->getMapTileSize().width;
	auto colSize = Vec2::ZERO;

	for (auto col : currentCol_)
	{
		if (col->GetData().type_ == 1)
		{
			colSize = Vec2{ (float)col->GetData().size_.x,(float)col->GetData().size_.y };
		}
	}
	this->setAnchorPoint({ 0.5f,0.0f });
	// 自分の隣が壁かそうでないか
	auto checkID = [&](Vec2 point) {
		auto gridPos = Vec2{ point } / chipSize;
		auto bottom = Vec2(gridPos.x, ColSize.height - gridPos.y);
		auto posGid = CollisionData->getTileGIDAt(bottom);
		if (posGid != 0)
		{
			// 補正が必要なときにtrue
			return true;
		}
		else
		{
			return false;
		}
		return false;
	};
	// 足元のポジション
	auto bottomPos = Vec2{ position.x,position.y - (0.3f * (delta * 50)) };
	// 自分の横隣のポジション
	// speed_はdirによって-になったり+になったりするので変数を活用
	auto nextPos = Vec2{ position.x + speed_.x,position.y };
	// 足元のポジションを基準に升目単位に直す
	auto nextYGrid = Vec2{ bottomPos.x,bottomPos.y } / chipSize;
	// 横隣のポジションを基準に升目単位に直す
	auto nextXGrid = Vec2{ nextPos.x,nextPos.y } / chipSize;

	auto gridYPos = Vec2(nextYGrid.x, ColSize.height - nextYGrid.y);
	auto gridYGid = CollisionData->getTileGIDAt(gridYPos);

	auto gridXPos = Vec2(nextXGrid.x, ColSize.height - nextXGrid.y);
	auto gridXGid = CollisionData->getTileGIDAt(gridXPos);
	// 範囲外check
	if (gridXPos.x > ColSize.width || gridXPos.x < 0 ||
		gridXPos.y > ColSize.height || gridXPos.y < 0)
	{
		return;
	}
	// 範囲外check
	if (gridYPos.x > ColSize.width || gridYPos.x < 0 ||
		gridYPos.y > ColSize.height || gridYPos.y < 0)
	{
		return;
	}
	if (gridXGid != 0)
	{
		isHitWall_ = true;
	}
	else
	{
		isHitWall_ = false;
	}
	if (gridYGid != 0)
	{
		onFloor_ = true;
	}
	else
	{
		onFloor_ = false;
	}
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
	attackCol_ = cocos2d::Rect(0, 0, 0, 0);
	// ﾀﾞﾒｰｼﾞ矩形情報のｾﾞﾛｸﾘ
	damageCol_ = cocos2d::Rect(0, 0, 0, 0);

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
					attackCol_.origin = getPosition() + (attackCol_.size/2);
				}
				else if (direction_ == Direction::Left)
				{
					// 攻撃矩形のﾎﾟｼﾞｼｮﾝ
					// direction毎に+ or - してやらないといけない
					attackCol_.origin = Vec2(getPosition().x - (attackCol_.size.width/2/* + 15.0f*/), getPosition().y + attackCol_.size.height);
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
			}
			/*auto damagedraw = DrawNode::create();
			damagedraw->drawRect(attackCol_.origin, attackCol_.origin - attackCol_.size, Color4F::GRAY);
			addChild(damagedraw);*/
		}
		else
		{
			// ﾀﾞﾒｰｼﾞ矩形のｻｲｽﾞ
			damageCol_.size = Size(colData.size_.x * getScale(), colData.size_.y * getScale());
			if (type_ == ActorType::Player)
			{
				// ﾀﾞﾒｰｼﾞ矩形のﾎﾟｼﾞｼｮﾝ
				damageCol_.origin = Vec2(getPosition().x, getPosition().y);
				/*auto damagedraw = DrawNode::create();
				damagedraw->drawDot(damageCol_.origin,3.0f, Color4F::GRAY);
				addChild(damagedraw);*/
			}
			else
			{
				if (direction_ == Direction::Left)
				{
					// ﾀﾞﾒｰｼﾞ矩形のﾎﾟｼﾞｼｮﾝ
					damageCol_.origin = Vec2(getPosition().x + (damageCol_.size.width / 4), getPosition().y);
				}
				else if (direction_ == Direction::Right)
				{
					// ﾀﾞﾒｰｼﾞ矩形のﾎﾟｼﾞｼｮﾝ
					damageCol_.origin = Vec2(getPosition().x - (damageCol_.size.width / 4), getPosition().y);
				}
			}
		}
	}
}

bool Actor::OnHit(const cocos2d::Rect& collision)
{
	auto flg = false;

	if (collision.size.width > 0.0f && collision.size.height > 0.0f)
	{
		flg = true;
	}
	// 矩形間の距離を測る
	auto distance = Vec2(collision.origin.x - damageCol_.origin.x, collision.origin.y - damageCol_.origin.y);
	auto sizediff = Vec2((damageCol_.size.width / 2.0f) + (collision.size.width / 2.0f),
					     (damageCol_.size.height / 2.0f) + (collision.size.height / 2.0f));
	if (flg)
	{
		if (!isHitAttack_)
		{
			if (abs(distance.x) <= sizediff.x && abs(distance.y) <= sizediff.y)
			{
				//onDamaged_ = true;
				isHitAttack_ = true;
				return true;
			}
		}
	}
	return false;
}

void Actor::SetIsHitAttack(bool flg)
{
	isHitAttack_ = flg;
}

