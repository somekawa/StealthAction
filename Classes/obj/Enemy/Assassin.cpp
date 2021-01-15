#include "Assassin.h"
#include "Gravity.h"
#include "_Debug/_DebugConOut.h"
#include "ActionRect.h"
#include "Loader/CollisionLoader.h"
#include "anim/AnimMng.h"
#include "obj/Player.h"

#include "BehaviorBaseAI/BehaviorData.h"
#include "BehaviorBaseAI/NodeBase.h"

USING_NS_CC;

Assassin::Assassin(Vec2 pos, Player& player,
	BehaviorTree* aiTree,VisionRange visionRange,int hp,Layer& myLayer):
	Enemy(pos,player,aiTree,visionRange,hp,myLayer)
{
	//pos_ = { 500,500 };
	//this->setPosition(Vec2(pos_.x, pos_.y));
	this->setScale(1.5f);
	myName_ = "assassin";
	flipFlag_ = FlipX::create(true);
	type_ = ActorType::Assassin;

	// アニメーションの登録
	AnimRegistrator();
	// ActModuleの登録
	actModuleRegistration();

	currentAnimation_ = "assassin_idle";
	previousAnimation_ = currentAnimation_;
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

				this->addChild(draw,col[colNum]->GetData().type_, anim);
			}
		}
	}
	// 初期アニメーションのセット
	lpAnimMng.InitAnimation(*this, ActorType::Assassin, "assassin_idle");
}

Assassin::~Assassin()
{

}

Assassin* Assassin::CreateAssassin(Vec2 pos, Player& player,
	BehaviorTree* aiTree,VisionRange visionRange,int hp,Layer& myLayer)
{
	Assassin* pRet = new(std::nothrow) Assassin(pos,player,aiTree,visionRange,hp,myLayer);
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

void Assassin::Action(void)
{
	//(this->*updater_)();
}

void Assassin::update(float delta)
{
	if (Director::getInstance()->getRunningScene()->getName() != "GameScene")
	{
		animationFrame_ = 0.0f;
		animationFrame_int_ = 0.0f;
		return;
	}
	// ﾌﾛｱ変更の際に自身を消す
	//DeleteSelfOnFloor();
	// 死んだ判定
	if (getName() == "assassin_death" || getName() == "changeFloor_death")
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
		// 現在のフレームを整数値で取得
		animationFrame_int_ = GetAnimationFrameInt() - 1;

		// 0以下になると0にする
		if (animationFrame_int_ < 0)
		{
			animationFrame_int_ = 0;
		}
		
		// ﾌﾟﾚｲﾔｰが攻撃状態だと当たり判定処理をする
		if (player_.IsAttacking())
		{
			// ﾌﾟﾚｲﾔｰとの当たり判定をとっている
			CheckHitPLAttack();
		}
		// ﾀﾞﾒｰｼﾞをくらっていない時と死ぬﾓｰｼｮﾝでない場合
		if (!onDamaged_ && stateTransitioner_ != &Enemy::Death)
		{
			// actCtlのｱｯﾌﾟﾃﾞｰﾄを回す
			actCtl_.update(type_, delta, *this);
		}

		if (stateTransitioner_ != &Enemy::Death)
		{
			// ﾀﾞﾒｰｼﾞを食らった
			if (onDamaged_)
			{
				if (stateTransitioner_ != &Enemy::Hit)
				{

					//onDamaged_ = false;
					ChangeAnimation("assassin_hit");
					//currentAnimation_ = "assassin_hit";
					// 0ではなかったらhit状態にする
					//ChangeAnimation("assassin_hit");
					stateTransitioner_ = &Enemy::Hit;
				}
			}
		}
		// hpが0になったら
		if (hp_ <= 0)
		{
			if (stateTransitioner_ != &Enemy::Death)
			{
				//currentAnimation_ = "assassin_death";
				ChangeAnimation("assassin_death");
				stateTransitioner_ = &Enemy::Death;
			}
		}


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

		// アニメーションの更新
		UpdateAnimation(delta);
		// 状態の遷移
		(this->*stateTransitioner_)();

		// アニメーション終了時に攻撃フラグをfalse
		if (isAnimEnd_)
		{
			isAttacking_ = false;
			hittingToPlayer_ = false;
			stateTransitioner_ = &Enemy::Idle;
			//onDamaged_ = false;
			currentAnimation_ = "assassin_idle";
		}

		if (currentAnimation_ != previousAnimation_)
		{
			ChangeAnimation(currentAnimation_);
			//onDamaged_ = false;
		}
		previousAnimation_ = currentAnimation_;

	}

}

void Assassin::AnimRegistrator(void)
{



}

void Assassin::AddAttackObj(const float& angle)
{
}

void Assassin::actModuleRegistration(void)
{
	Vec2 size = { 106.0f,76.0f };

	// 右向き反転
	{
		ActModule flipAct;
		flipAct.state = nullptr;
		flipAct.flipFlg = true;
		flipAct.actName = "assassin_idle";
		//flipAct.blackList.emplace_back(ACTION::FALLING);

		flipAct.blackList.emplace_back("assassin_attack");
		actCtl_.RunInitializeActCtl(type_, "右向き", flipAct);
	}

	// 左向き反転
	{
		ActModule flipAct;
		flipAct.state = nullptr;
		flipAct.flipFlg = false;
		flipAct.actName = "assassin_idle";

		//flipAct.blackList.emplace_back(ACTION::FALLING);

		flipAct.blackList.emplace_back("assassin_attack");
		actCtl_.RunInitializeActCtl(type_, "左向き", flipAct);
	}

	// 右移動
	{
		ActModule act;
		act.state = nullptr;
		act.vel = Vec2{ 2,0 };
		act.actName = "assassin_run";
		act.checkPoint1 = Vec2{ size.x / 2, size.y / 2 };	// 右上
		act.checkPoint2 = Vec2{ size.x / 2,  15 };			// 右下
		//act.blackList.emplace_back(ACTION::FALLING);	// 落下中に右移動してほしくないときの追加の仕方

		//act.whiteList.emplace_back(ACTION::JUMPING);
		act.blackList.emplace_back("assassin_attack");
		actCtl_.RunInitializeActCtl(type_,"右移動", act);
	}

	// 左移動
	{
		ActModule act;
		act.state = nullptr;
		act.vel = Vec2{ 2,0 };
		act.actName = "assassin_run";
		act.checkPoint1 = Vec2{ -size.x / 2, size.y / 2 };	// 左上
		act.checkPoint2 = Vec2{ -size.x / 2,  15 };			// 左下

		//act.blackList.emplace_back(ACTION::FALLING);

		//act.whiteList.emplace_back(ACTION::JUMPING);
		act.blackList.emplace_back("assassin_attack");
		actCtl_.RunInitializeActCtl(type_,"左移動", act);
	}


	// 落下
	{
		// checkkeylistに離している間の設定もしたけど特に効果なし
		ActModule act;
		act.actName = "assassin_fall";
		act.state = nullptr;
		//act.checkPoint1 = Vec2{ 0,-10 };			// 左下
		//act.checkPoint2 = Vec2{ 0,-10 };			// 右下
		act.checkPoint1 = Vec2{ -60, -60 };				// 左下
		act.checkPoint2 = Vec2{ -60, -60 };				// 右下

		act.checkPoint3 = Vec2{ size.x / 2, size.y / 2 };  // 右上
		act.checkPoint4 = Vec2{ -size.x / 2, size.y / 2 }; // 左上

		act.gravity = Vec2{ 0.0f,-5.0f };
		act.blackList.emplace_back("assassin_attack");	// ジャンプ中に落下してほしくない
		//act.blackList.emplace_back(ACTION::JUMP);	// ジャンプ中に落下してほしくない

		actCtl_.RunInitializeActCtl(type_, "落下", act);
	}
	// 攻撃
	{
		ActModule act;
		act.state = nullptr;
		act.actName = "assassin_attack";
		actCtl_.RunInitializeActCtl(type_, "攻撃", act);
	}
	// 更新関数の登録
	actCtl_.InitUpdater(type_);
}

void Assassin::Jump(void)
{
}

void Assassin::Fall(void)
{
}

void Assassin::NormalAttack(void)
{
	//isAttacking_ = true;
	if (animationFrame_int_ < 13)
	{
		currentCol_ = collider_[currentAnimation_][animationFrame_int_];
	}
	if (OnAttacked())
	{
		player_.OnDamaged();
	}
	TRACE("direction:%d", direction_);
}

void Assassin::UseSkill(void)
{
}

void Assassin::Patrol(void)
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
	currentAnimation_ = "assassin_run";
	auto move = MoveTo::create(0.0f, Vec2(previousPos + speed_.x,getPosition().y));
	//pos_.x += speed_.x;
	//this->setPosition(Vec2(pos_.x, pos_.y));
	this->runAction(move);
	this->runAction(flipFlag_);
	setPosition(Vec2(previousPos + speed_.x, getPosition().y));
}

void Assassin::Chase(void)
{
	auto previousPos = getPosition().x;

	// プレイヤーを追いかける処理
	switch (direction_)
	{
	case Direction::Right:
		speed_.x = 2;
		flipFlag_ = FlipX::create(true);
		break;
	case Direction::Left:
		speed_.x = -2;
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
	currentAnimation_ = "assassin_run";
	auto move = MoveTo::create(0.0f, Vec2(previousPos + speed_.x, getPosition().y));
	//pos_.x += speed_.x;
	//this->setPosition(Vec2(pos_.x, pos_.y));
	this->runAction(move);
	this->runAction(flipFlag_);
	setPosition(Vec2(previousPos + speed_.x, getPosition().y));
}

void Assassin::Run(void)
{
	switch (direction_)
	{
	case Direction::Right:
		speed_.x = 4;
		flipFlag_ = FlipX::create(true);
		break;
	case Direction::Left:
		speed_.x = -4;
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

	pos_.x += speed_.x;
	this->setPosition(Vec2(pos_.x, pos_.y));
	this->runAction(flipFlag_);

	// 今のとこ50.0fはマジックナンバー
	// 後で変更あり
	if (GetDistance() < 50.0f)
	{
		// アニメーションを変更して
		ChangeAnimation("assassin_attack");
		// 変更したアニメーションを走らせる
		this->runAction(Animate::create(lpAnimMng.GetAnimationCache(type_, currentAnimation_)));
		// 行動をattackにする
		updater_ = &Assassin::Attack;
	}
}

void Assassin::Attack(void)
{
	// attackモーションが終了したら
	if (isAnimEnd_)
	{
		// アニメーションを変更して
		ChangeAnimation("assassin_idle");
		// 変更したアニメーションを走らせる
		this->runAction(Animate::create(lpAnimMng.GetAnimationCache(type_, currentAnimation_)));
		// 行動をidleにする
		updater_ = &Assassin::Idle;
	}
}
