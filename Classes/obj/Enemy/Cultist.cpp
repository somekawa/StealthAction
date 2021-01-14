#include "Cultist.h"
#include "Gravity.h"
#include "_Debug/_DebugConOut.h"
#include "ActionRect.h"
#include "Loader/CollisionLoader.h"
#include "anim/AnimMng.h"
#include "obj/Player.h"

#include "BehaviorBaseAI/BehaviorData.h"
#include "BehaviorBaseAI/NodeBase.h"

#include "obj/Objects/Fireball.h"

USING_NS_CC;

Cultist::Cultist(Vec2 pos, Player& player, BehaviorTree* aiTree, VisionRange visionRange, int hp,Layer& myLayer):
    Enemy(pos,player, aiTree, visionRange, hp,myLayer)
{
	attackCnt_ = 0;
	myName_ = "cultist";

	//pos_ = { 500,500 };
	//this->setPosition(Vec2(pos_.x, pos_.y));
	this->setScale(2.0f);
	flipFlag_ = FlipX::create(true);
	type_ = ActorType::Cultist;

	// アニメーションの登録
	AnimRegistrator();
	actModuleRegistration();
	// ActModuleの登録
	//ActModuleRegistrator();

	currentAnimation_ = "cultist_idle";
	previousAnimation_ = currentAnimation_;

	this->runAction(Animate::create(lpAnimMng.GetAnimationCache(type_, currentAnimation_)));

	direction_ = Direction::Left;

	// 攻撃矩形のサイズ設定
	attackRect_.size_ = Size(30.0f, 30.0f);

	for (auto anim : lpAnimMng.GetAnimations(type_))
	{
		// colliderBoxのLoad
		lpCol.ReadData(collider_, anim);
		for (auto col : collider_[anim])
		{
			for (int colNum = 0; colNum < col.size(); colNum++)
			{
				// colliderBoxを自身の子にする
				auto draw = col[colNum]->create();
				draw->setContentSize(Size{ (float)col[colNum]->GetData().size_.x,(float)col[colNum]->GetData().size_.y });
				draw->drawRect(Vec2(0, 0), Vec2{ (float)col[colNum]->GetData().size_.x,(float)col[colNum]->GetData().size_.y }, col[colNum]->GetColor());

				draw->setTag(col[colNum]->GetData().frame_);

				this->addChild(draw, col[colNum]->GetData().type_, anim);
			}
		}
	}
	// 初期アニメーションのセット
	lpAnimMng.InitAnimation(*this, ActorType::Cultist, "cultist_idle");
}

Cultist::~Cultist()
{
}

Cultist* Cultist::CreateCultist(Vec2 pos, Player& player, BehaviorTree* aiTree, VisionRange visionRange, int hp,Layer& myLayer)
{
	Cultist* pRet = new(std::nothrow) Cultist(pos,player, aiTree, visionRange, hp,myLayer);
	if (pRet && pRet->init())
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

void Cultist::Action(void)
{
}

void Cultist::update(float delta)
{
	if (Director::getInstance()->getRunningScene()->getName() != "GameScene")
	{
		animationFrame_ = 0.0f;
		return;
	}
	// 死んだ判定
	if (getName() == "cultist_death" || getName() == "changeFloor_death")
	{
		// 自分を親であるGameSceneから削除する
		this->removeFromParentAndCleanup(true);
	}
	else
	{
		if (!isAttacking_)
		{
			// 方向の変更
			ChangeDirection();
		}
		actCtl_.update(type_, delta, *this);

		// 現在のフレームを整数値で取得
		animationFrame_int_ = GetAnimationFrameInt() - 1;
		// 無条件に通っていたら処理が重くなるので
		// ﾌﾟﾚｲﾔｰが攻撃態勢で自分が攻撃食らっていなかったら(今のところこうしているが、後で自分がhitﾓｰｼｮﾝが終わったらに変更予定)
		if (player_.IsAttacking() && !onDamaged_)
		{
			// ﾌﾟﾚｲﾔｰとの当たり判定をとっている
			CheckHitPLAttack();
		}

		// ﾀﾞﾒｰｼﾞを食らった
		if (onDamaged_ && stateTransitioner_ != &Enemy::Hit)
		{
			// hpを削る
			hp_ -= 10;
			// hpが0になったら
			if (hp_ <= 0)
			{
				if (stateTransitioner_ != &Enemy::Death)
				{
					// 死ぬ状態にする
					ChangeAnimation("death");
					stateTransitioner_ = &Enemy::Death;
				}
			}
			else
			{
				// 0ではなかったらhit状態にする
				ChangeAnimation("hit");
				stateTransitioner_ = &Enemy::Hit;
			}
		}
		// 0以下になると0にする
		if (animationFrame_int_ < 0)
		{
			animationFrame_int_ = 0;
		}

		//if (currentAnimation_ == "attack")
		{
		}

		//previousAnimation_ = currentAnimation_;

		TRACE("pos:(%f,%f)", this->getPosition().x, this->getPosition().y);

		TRACE("attackFlag:%d\n", isAttacking_);


		for (auto animationCol = this->getChildren().rbegin();
			animationCol != this->getChildren().rend(); animationCol++)
		{
			if (currentAnimation_ == (*animationCol)->getName() &&
				animationFrame_int_ == (*animationCol)->getTag())
			{
				(*animationCol)->setVisible(true);
			}
			else
			{
				(*animationCol)->setVisible(false);
			}
		}
		// 状態の遷移
		(this->*stateTransitioner_)();
		// Mapオブジェクトに当たっているかの確認
		//CheckMapObjHit(delta);
		// 重力をかける
		//gravity_->ApplyGravityToTarget(delta);
		// アニメーションの更新
		UpdateAnimation(delta);
		// アニメーション終了時に攻撃フラグをfalse
		if (isAnimEnd_)
		{
			isAttacking_ = false;
			isFire_ = false;
			hittingToPlayer_ = false;
		}
		if (currentAnimation_ != previousAnimation_)
		{
			ChangeAnimation(currentAnimation_);
		}
		previousAnimation_ = currentAnimation_;
	}
	// ﾌﾛｱ変更の際に自身を消す
	//DeleteSelfOnFloor();
}

void Cultist::AnimRegistrator(void)
{


}

void Cultist::AddAttackObj(const float& angle)
{
	// 攻撃していたらfireballを飛ばす
	auto fireball = Fireball::CreateFireball({ getPosition().x,getPosition().y + 30.0f },direction_,angle,player_);
	// 攻撃ﾚｲﾔｰにfireballをぶら下げる
	attackLayer_->addChild(fireball,0);
	attackCnt_++;
	fireball->scheduleUpdate();
}

const float Cultist::GetPLAngle(void)
{
	auto plPos = player_.getPosition();

	auto angle = atan2(getPosition().y - plPos.y,getPosition().x - plPos.x);

	return angle;
}

void Cultist::actModuleRegistration(void)
{
	Vec2 size = { 45.0f,42.0f };

	// 右移動
	{
		ActModule act;
		act.state = nullptr;
		act.vel = Vec2{ 2,0 };
		act.actName = "cultist_walk";
		act.checkPoint1 = Vec2{ size.x / 2, size.y / 2 };	// 右上
		act.checkPoint2 = Vec2{ size.x / 2,  15 };			// 右下
		//act.blackList.emplace_back(ACTION::FALLING);	// 落下中に右移動してほしくないときの追加の仕方

		//act.whiteList.emplace_back(ACTION::JUMPING);
		act.blackList.emplace_back("cultist_attack");
		actCtl_.RunInitializeActCtl(type_, "右移動", act);
	}

	// 左移動
	{
		ActModule act;
		act.state = nullptr;
		act.vel = Vec2{ 2,0 };
		act.actName = "cultist_walk";
		act.checkPoint1 = Vec2{ -size.x / 2, size.y / 2 };	// 左上
		act.checkPoint2 = Vec2{ -size.x / 2,  15 };			// 左下

		//act.blackList.emplace_back(ACTION::FALLING);

		//act.whiteList.emplace_back(ACTION::JUMPING);
		act.blackList.emplace_back("cultist_attack");
		actCtl_.RunInitializeActCtl(type_, "左移動", act);
	}

	// 右向き反転
	{
		ActModule flipAct;
		flipAct.state = nullptr;
		flipAct.flipFlg = true;
		flipAct.actName = "cultist_idle";
		//flipAct.blackList.emplace_back(ACTION::FALLING);

		flipAct.blackList.emplace_back("cultist_attack");
		actCtl_.RunInitializeActCtl(type_, "右向き", flipAct);
	}

	// 左向き反転
	{
		ActModule flipAct;
		flipAct.state = nullptr;
		flipAct.flipFlg = false;
		flipAct.actName = "cultist_idle";

		//flipAct.blackList.emplace_back(ACTION::FALLING);

		flipAct.blackList.emplace_back("cultist_attack");
		actCtl_.RunInitializeActCtl(type_, "左向き", flipAct);
	}
	// 落下
	{
		// checkkeylistに離している間の設定もしたけど特に効果なし
		ActModule act;
		act.actName = "cultist_fall";
		act.state = nullptr;
		//act.checkPoint1 = Vec2{ 0,-10 };			// 左下
		//act.checkPoint2 = Vec2{ 0,-10 };			// 右下
		act.checkPoint1 = Vec2{ 0,0 };				// 左下
		act.checkPoint2 = Vec2{ 0,0 };				// 右下

		act.checkPoint3 = Vec2{ size.x / 2, size.y / 2 };  // 右上
		act.checkPoint4 = Vec2{ -size.x / 2, size.y / 2 }; // 左上

		act.gravity = Vec2{ 0.0f,-5.0f };
		act.blackList.emplace_back("cultist_attack");	// ジャンプ中に落下してほしくない
		//act.blackList.emplace_back(ACTION::JUMP);	// ジャンプ中に落下してほしくない

		actCtl_.RunInitializeActCtl(type_, "落下", act);
	}
	// 攻撃
	{
		ActModule act;
		act.state = nullptr;
		act.actName = "cultist_attack";
		actCtl_.RunInitializeActCtl(type_, "攻撃", act);
	}
	// 更新関数の登録
	actCtl_.InitUpdater(type_);
}

void Cultist::NormalAttack(void)
{
	if (animationFrame_int_ < 11)
	{
		currentCol_ = collider_[currentAnimation_][animationFrame_int_];
	}

	if (animationFrame_ >= 0.01f)
	{
		isFire_ = true;
	}

}

void Cultist::UseSkill(void)
{

}

void Cultist::Patrol(void)
{
	if (isMoveComplete_)
	{
		// 一定距離歩いていたら
		oldPos_ = getPosition();
		isMoveComplete_ = false;
	}
	else
	{
		// 歩いた距離がある一定距離になるまで
		if (getPosition().x >= oldPos_.x + 30.0f)
		{
			isMoveComplete_ = true;
		}
	}
	auto previousPos = getPosition().x;
	// 巡回処理
	switch (direction_)
	{
	case Direction::Right:
		speed_.x = 1;
		flipFlag_ = FlipX::create(true);
		break;
	case Direction::Left:
		speed_.x = -1;
		flipFlag_ = FlipX::create(false);
		break;
	case Direction::Up:
		break;
	case Direction::Down:
		break;
	case Direction::Max:
		break;
	default:
		break;
	}
	currentAnimation_ = "cultist_walk";
	auto move = MoveTo::create(0.0f, Vec2(previousPos + speed_.x, getPosition().y));
	//pos_.x += speed_.x;
	//this->setPosition(Vec2(pos_.x, pos_.y));
	this->runAction(move);
	this->runAction(flipFlag_);
	setPosition(Vec2(previousPos + speed_.x, getPosition().y));
}

void Cultist::Chase(void)
{
}

void Cultist::Run(void)
{
}

void Cultist::Jump(void)
{
}

void Cultist::Fall(void)
{
}
