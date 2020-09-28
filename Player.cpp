#include <DxLib.h>
#include "Player.h"

Player::Player()
{
	Init();
}

Player::~Player()
{
}

bool Player::Init(void)
{
	controller_ = std::make_unique<KeyBoard>();
	controller_->Setup();

	LoadDivGraph("image/idle.png", 4, 4, 1, 240, 240, playerIdleImg);
	LoadDivGraph("image/run.png", 10, 2, 5, 240, 240, playerRunImg);
	playerDown = LoadGraph("image/down.png");

	moveMode_ = PL_MOVE::IDLE;
	dir_ = PL_DIR::RIGHT;
	ImgMax_ = 3;
	ImgNowCnt_ = 0;
	animCnt_ = 0;
	keyActFlg_ = false;

	/*キーごとの処理*/
	keyAct_.try_emplace(InputID::DOWN, [&](CtlData & ctlData) {
		if (ctlData[InputID::DOWN][static_cast<int>(TRG::NOW)])
		{
			keyActFlg_ = true;
			moveMode_ = PL_MOVE::DOWN;
		}
	});

	keyAct_.try_emplace(InputID::LEFT, [&](CtlData & ctlData) {
		if (ctlData[InputID::LEFT][static_cast<int>(TRG::NOW)])
		{
			keyActFlg_ = true;
			moveMode_ = PL_MOVE::LEFT;
			dir_ = PL_DIR::LEFT;
		}
	});

	keyAct_.try_emplace(InputID::RIGHT, [&](CtlData & ctlData) {
		if (ctlData[InputID::RIGHT][static_cast<int>(TRG::NOW)])
		{
			keyActFlg_ = true;
			moveMode_ = PL_MOVE::RIGHT;
			dir_ = PL_DIR::RIGHT;
		}
	});

	return true;
}

void Player::Update(void)
{
	// コントローラー状態更新
	(*controller_)();

	// キー処理
	auto CtlData = controller_->GetCtlData();
	keyActFlg_ = false;
	for (auto id : InputID())
	{
		keyAct_[id](CtlData);
	}

	// 待機状態
	if (!keyActFlg_)
	{
		moveMode_ = PL_MOVE::IDLE;
	}

	// カウント値を0に戻す
	if (animCnt_ >= 50)
	{
		animCnt_ = 0;
	}
	animCnt_++;

}

void Player::Draw(void)
{
	// 待機
	if (moveMode_ == PL_MOVE::IDLE)
	{
		ImgMax_ = 4;

		// アニメーション
		if (animCnt_ % 10 == 0)
		{
			if (ImgNowCnt_ < ImgMax_ - 1)
			{
				ImgNowCnt_++;
			}
			else if (ImgNowCnt_ >= ImgMax_ - 1)
			{
				ImgNowCnt_ = 0;
			}
		}

		// 画像を反転すべきか判断しての描画
		if (dir_ == PL_DIR::RIGHT)
		{
			DrawRotaGraph(100, 100, 1.0f, 0.0f, playerIdleImg[ImgNowCnt_], true);
		}
		else if (dir_ == PL_DIR::LEFT)
		{
			DrawRotaGraph(100, 100, 1.0f, 0.0f, playerIdleImg[ImgNowCnt_], true, true);
		}
	}

	// 走る
	if (moveMode_ == PL_MOVE::RIGHT || moveMode_ == PL_MOVE::LEFT)
	{
		ImgMax_ = 10;

		// アニメーション
		if (animCnt_ % 5 == 0)
		{
			if (ImgNowCnt_ < ImgMax_ - 1)
			{
				ImgNowCnt_++;
			}
			else if (ImgNowCnt_ >= ImgMax_ - 1)
			{
				ImgNowCnt_ = 0;
			}
		}

		// 画像を反転すべきか判断しての描画
		if (dir_ == PL_DIR::RIGHT)
		{
			DrawRotaGraph(100, 100, 1.0f, 0.0f, playerRunImg[ImgNowCnt_], true);
		}
		else if (dir_ == PL_DIR::LEFT)
		{
			DrawRotaGraph(100, 100, 1.0f, 0.0f, playerRunImg[ImgNowCnt_], true, true);
		}
	}

	// しゃがむ
	if (moveMode_ == PL_MOVE::DOWN)
	{
		// 画像を反転すべきか判断しての描画
		if (dir_ == PL_DIR::RIGHT)
		{
			DrawRotaGraph(100, 100, 1.0f, 0.0f, playerDown, true);
		}
		else if (dir_ == PL_DIR::LEFT)
		{
			DrawRotaGraph(100, 100, 1.0f, 0.0f, playerDown, true, true);
		}
	}
}