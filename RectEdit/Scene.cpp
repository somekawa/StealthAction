#include <DxLib.h>
#include "Scene.h"
#include "KeyInput.h"
#include "Mouse.h"
#include "ImageManager.h"
#include "ActorType.h"

Scene::Scene() :spritePos_({Screen_x/2,Screen_y/2})
{
	Init();
}

Scene::~Scene()
{
}

void Scene::UpDate(const KeyInput& keyInput, const Mouse& mouse)
{
	mousePos_ = mouse.GetPos();

	(this->*upDater_)(keyInput,mouse);

	Draw();

}

void Scene::Draw(void)
{
	ClearDrawScreen();
	(this->*drawExecuter_)();
	ScreenFlip();
}

void Scene::Init(void)
{
	upDater_ = &Scene::SelectActor;
	actorType_ = ActorType::Player;
	actor_ = Actor::Max;
	rectType_ = 0;
	turn_ = EditTurn::Max;
	actorName_ =
	{
		"Player",
		"Enemy",
		"Numpad1:player,Numpad2:enemy"
	};

	actorTypeName_ =
	{
		"player",
		"imp",
		"assassin",
		"twistedCultist",
		"cultist"
	};

	isModeChange_ = false;
	rectLoadFlag_ = true;
	animationCount_ = 0;
	currentFrame_ = 0;
	maxFrame_ = 0;

	auto& imagemanager = ImageManager::GetInstance();
	// size count
	imagemanager.Load(ActorType::Player, "attack1", Vector2I(54, 29), Vector2I(10, 1));
	imagemanager.Load(ActorType::Player, "attack2", Vector2I(54, 29), Vector2I(10, 1));
	imagemanager.Load(ActorType::Player, "attack3", Vector2I(54, 29), Vector2I(11, 1));
	imagemanager.Load(ActorType::Player, "run", Vector2I(21, 23), Vector2I(10, 1));
	imagemanager.Load(ActorType::Player, "jump", Vector2I(13, 25), Vector2I(1, 4));
	imagemanager.Load(ActorType::Player, "fall", Vector2I(18, 32), Vector2I(4, 1));
	imagemanager.Load(ActorType::Player, "idle", Vector2I(15, 18), Vector2I(1, 3));

	imagemanager.Load(ActorType::Imp, "attackFirst", Vector2I(100, 80), Vector2I(6, 1));
	imagemanager.Load(ActorType::Imp, "death", Vector2I(50, 48), Vector2I(5, 1));
	imagemanager.Load(ActorType::Imp, "walk", Vector2I(50, 48), Vector2I(6, 1));

	imagemanager.Load(ActorType::Assassin, "ambush", Vector2I(106, 76), Vector2I(13, 1));
	imagemanager.Load(ActorType::Assassin, "arise", Vector2I(106, 76), Vector2I(8, 1));
	imagemanager.Load(ActorType::Assassin, "attack", Vector2I(106, 76), Vector2I(13, 1));
	imagemanager.Load(ActorType::Assassin, "death", Vector2I(106, 76), Vector2I(16, 1));
	imagemanager.Load(ActorType::Assassin, "death_noEffect", Vector2I(106, 76), Vector2I(16, 1));
	imagemanager.Load(ActorType::Assassin, "fall", Vector2I(106, 76), Vector2I(3, 1));
	imagemanager.Load(ActorType::Assassin, "hit", Vector2I(106, 76), Vector2I(3, 1));
	imagemanager.Load(ActorType::Assassin, "hit_noEffect", Vector2I(106, 76), Vector2I(3, 1));
	imagemanager.Load(ActorType::Assassin, "idle", Vector2I(106, 76), Vector2I(8, 1));
	imagemanager.Load(ActorType::Assassin, "jump", Vector2I(106, 76), Vector2I(3, 1));
	imagemanager.Load(ActorType::Assassin, "run", Vector2I(106, 76), Vector2I(8, 1));
	imagemanager.Load(ActorType::Assassin, "run_blink", Vector2I(106, 76), Vector2I(8, 1));
	imagemanager.Load(ActorType::Assassin, "uptoFall", Vector2I(106, 76), Vector2I(2, 1));
	imagemanager.Load(ActorType::Assassin, "vanish", Vector2I(106, 76), Vector2I(8, 1));

	imagemanager.Load(ActorType::TwistedCultist, "attack", Vector2I(91, 42), Vector2I(7, 1));
	imagemanager.Load(ActorType::TwistedCultist, "death", Vector2I(45, 42), Vector2I(12, 1));
	imagemanager.Load(ActorType::TwistedCultist, "death_noEffect", Vector2I(45, 42), Vector2I(3, 1));
	imagemanager.Load(ActorType::TwistedCultist, "fall", Vector2I(45, 42), Vector2I(3, 1));
	imagemanager.Load(ActorType::TwistedCultist, "hit", Vector2I(45, 42), Vector2I(3, 1));
	imagemanager.Load(ActorType::TwistedCultist, "hit_noEffect", Vector2I(45, 42), Vector2I(3, 1));
	imagemanager.Load(ActorType::TwistedCultist, "idle", Vector2I(45, 42), Vector2I(6, 1));
	imagemanager.Load(ActorType::TwistedCultist, "idle_closeEye", Vector2I(45, 42), Vector2I(6, 1));
	imagemanager.Load(ActorType::TwistedCultist, "jump", Vector2I(45, 42), Vector2I(3, 1));
	imagemanager.Load(ActorType::TwistedCultist, "twist", Vector2I(45, 42), Vector2I(12, 1));
	imagemanager.Load(ActorType::TwistedCultist, "uptoFall", Vector2I(45, 42), Vector2I(2, 1));
	imagemanager.Load(ActorType::TwistedCultist, "walk", Vector2I(45, 42), Vector2I(8, 1));
	imagemanager.Load(ActorType::TwistedCultist, "walk_closeEye", Vector2I(45, 42), Vector2I(8, 1));

	imagemanager.Load(ActorType::Cultist, "attack", Vector2I(45, 42), Vector2I(10, 1));
	imagemanager.Load(ActorType::Cultist, "death", Vector2I(45, 42), Vector2I(12, 1));
	imagemanager.Load(ActorType::Cultist, "death_noEffect", Vector2I(45, 42), Vector2I(12, 1));
	imagemanager.Load(ActorType::Cultist, "fall", Vector2I(45, 42), Vector2I(3, 1));
	imagemanager.Load(ActorType::Cultist, "hit", Vector2I(45, 42), Vector2I(3, 1));
	imagemanager.Load(ActorType::Cultist, "hit_noEffect", Vector2I(45, 42), Vector2I(3, 1));
	imagemanager.Load(ActorType::Cultist, "idle", Vector2I(45, 42), Vector2I(6, 1));
	imagemanager.Load(ActorType::Cultist, "jump", Vector2I(45, 42), Vector2I(3, 1));
	imagemanager.Load(ActorType::Cultist, "uptoFall", Vector2I(45, 42), Vector2I(2, 1));
	imagemanager.Load(ActorType::Cultist, "walk", Vector2I(45, 42), Vector2I(8, 1));
}

void Scene::InputAnimation(void)
{
	auto& imageManage = ImageManager::GetInstance();

	if (KeyInputSingleCharString(300, 200, 30, inputAnimationName_, false) == 1)
	{
		if (!imageManage.GetID(actorType_, inputAnimationName_).empty())
		{
			auto it = imageManage.GetActionSet(actorType_).find(inputAnimationName_);
			maxFrame_ = it->second;
			actionRect_.emplace_back();
			upDater_ = &Scene::Edit;
			/*drawExecuter_ = &Scene::LoadRectDraw;
			upDater_ = &Scene::LoadRect;*/
		}
	}
}

void Scene::SelectActor(const KeyInput& keyInput, const Mouse& mouse)
{
	auto inputNow = keyInput.GetInput(InputFlag::Now);
	auto inputOld = keyInput.GetInput(InputFlag::Old);

	auto mouseDataNow = mouse.GetMouseFlag(InputFlag::Now);
	auto mouseDataOld = mouse.GetMouseFlag(InputFlag::Old);

	if (inputNow[KEY_INPUT_NUMPAD1] && !inputOld[KEY_INPUT_NUMPAD1])
	{
		actor_ = Actor::Player;
	}
	if (inputNow[KEY_INPUT_NUMPAD2] && !inputOld[KEY_INPUT_NUMPAD2])
	{
		actor_ = Actor::Enemy;
	}

	if (actor_ != Actor::Max)
	{
		if (inputNow[KEY_INPUT_RETURN] && !inputOld[KEY_INPUT_RETURN])
		{
			upDater_ = &Scene::SelectActorType;
		}
	}

	drawExecuter_ = &Scene::SelectActorDraw;
}

void Scene::SelectActorType(const KeyInput& keyInput, const Mouse& mouse)
{
	auto inputNow = keyInput.GetInput(InputFlag::Now);
	auto inputOld = keyInput.GetInput(InputFlag::Old);

	auto mouseDataNow = mouse.GetMouseFlag(InputFlag::Now);
	auto mouseDataOld = mouse.GetMouseFlag(InputFlag::Old);

	if (!isModeChange_)
	{
		if (actor_ == Actor::Enemy)
		{
			actorType_ = ActorType::Imp;
			isModeChange_ = true;
		}
		else
		{
			actorType_ = ActorType::Player;
			isModeChange_ = false;
			turn_ = EditTurn::Start;
			upDater_ = &Scene::SelectAnimation;
		}
	}

	if (actor_ == Actor::Enemy)
	{
		if (inputNow[KEY_INPUT_RIGHT] && !inputOld[KEY_INPUT_RIGHT])
		{
			actorType_ = (actorType_ < ActorType::Cultist?actorType_+1:ActorType::Imp);
		}
		if (inputNow[KEY_INPUT_LEFT] && !inputOld[KEY_INPUT_LEFT])
		{
			actorType_ = (actorType_ >= ActorType::Imp?actorType_-1:ActorType::Cultist);
		}
		if (inputNow[KEY_INPUT_RETURN] && !inputOld[KEY_INPUT_RETURN])
		{
			isModeChange_ = false;
			turn_ = EditTurn::Start;
			upDater_ = &Scene::SelectAnimation;
		}
	}

	drawExecuter_ = &Scene::SelectActorTypeDraw;
}

void Scene::SelectAnimation(const KeyInput& keyInput, const Mouse& mouse)
{
	auto inputNow = keyInput.GetInput(InputFlag::Now);
	auto inputOld = keyInput.GetInput(InputFlag::Old);

	drawExecuter_ = &Scene::SelectAnimationDraw;

}

void Scene::Edit(const KeyInput& keyInput, const Mouse& mouse)
{
	auto inputNow = keyInput.GetInput(InputFlag::Now);
	auto inputOld = keyInput.GetInput(InputFlag::Old);

	auto clickNow = mouse.GetMouseFlag(InputFlag::Now);
	auto clickOld = mouse.GetMouseFlag(InputFlag::Old);

	// ｷｰﾎﾞｰﾄﾞのAを押すとattack矩形ﾓｰﾄﾞに
	// ｷｰﾎﾞｰﾄﾞのDを押すとdamage矩形ﾓｰﾄﾞに
	if (inputNow[KEY_INPUT_A] && !inputOld[KEY_INPUT_A])
	{
		rectType_ = 0;
	}
	if (inputNow[KEY_INPUT_D] && !inputOld[KEY_INPUT_D])
	{
		rectType_ = 1;
	}

	for (auto click : ClickDir())
	{
		// 左クリック時
		if (click == ClickDir::Left)
		{
			if (turn_ == EditTurn::Start)
			{
				if (clickNow[click] && !clickOld[click])
				{
					// 始点
					begin_ = mousePos_;
					turn_ = EditTurn::End;
				}
			}
		}
		// 右クリック時
		else
		{
			if (turn_ == EditTurn::End)
			{
				if (clickNow[click] && !clickOld[click])
				{
					// 終点の設定
					end_ = mousePos_;
					turn_ = EditTurn::Start;
					actionRect_[currentFrame_].emplace_back(std::make_shared<ActionRect>(begin_,end_,spritePos_,rectType_,currentFrame_));
				}
			}
		}
	}
	if (inputNow[KEY_INPUT_LCONTROL])
	{

		if (inputNow[KEY_INPUT_S] && !inputOld[KEY_INPUT_S])
		{
			currentFrame_++;
			if (currentFrame_ <= maxFrame_ - 1)
			{
				actionRect_.emplace_back();
			}
			else
			{
				auto& saver = RectSaver::GetInstance();
				for (auto rects : actionRect_)
				{
					rectNum_.emplace_back(rects.size());
				}
				currentFrame_ = 0;
				saver.WriteData(actionRect_,
					inputAnimationName_,
					actorTypeName_[static_cast<int>(actorType_)]);

				ResetData();
				upDater_ = &Scene::SelectActor;
			}
		}
	}
	if (upDater_ != &Scene::SelectActor)
	{
		drawExecuter_ = &Scene::EditDraw;
	}

}

void Scene::SelectEditorLoad(const KeyInput& keyInput, const Mouse& mouse)
{
	auto inputNow = keyInput.GetInput(InputFlag::Now);
	auto inputOld = keyInput.GetInput(InputFlag::Old);

	if (inputNow[KEY_INPUT_1] && !inputOld[KEY_INPUT_1])
	{
		upDater_ = &Scene::LoadRect;
	}
	if (inputNow[KEY_INPUT_2] && !inputOld[KEY_INPUT_2])
	{
		upDater_ = &Scene::Edit;
	}
}

void Scene::LoadRect(const KeyInput& keyInput, const Mouse& mouse)
{
	auto& saver = RectSaver::GetInstance();

	auto inputNow = keyInput.GetInput(InputFlag::Now);
	auto inputOld = keyInput.GetInput(InputFlag::Old);

	if (inputNow[KEY_INPUT_RIGHT] && !inputOld[KEY_INPUT_RIGHT])
	{
		currentFrame_++;
		if (currentFrame_ >= 10)
		{
			currentFrame_ = 0;
		}
	}

	if (inputNow[KEY_INPUT_LEFT] && !inputOld[KEY_INPUT_LEFT])
	{
		currentFrame_--;
		if (currentFrame_ < 0)
		{
			currentFrame_ = 9;
		}
	}

	if (rectLoadFlag_)
	{
		saver.LoadRect(actionRect_, inputAnimationName_, "player");
		rectLoadFlag_ = false;
	}

	drawExecuter_ = &Scene::LoadRectDraw;
}

void Scene::End(const KeyInput& keyInput, const Mouse& mouse)
{
	auto inputNow = keyInput.GetInput(InputFlag::Now);
	auto inputOld = keyInput.GetInput(InputFlag::Old);

	drawExecuter_ = &Scene::EndDraw;
}

void Scene::ResetData(void)
{
	actionRect_.clear();
	rectNum_.clear();
}

void Scene::SelectActorDraw(void)
{
	DrawFormatString(0, 300, 0xffffff, "NumPad1:Player,NumPad2:Enemy");
	DrawFormatString(0, 30, 0xffffff, actorName_[static_cast<int>(actor_)].c_str());
	DrawFormatString(0, 330, 0xffffff, "決定後Enterｷｰで次へ");

	auto& imageManage = ImageManager::GetInstance();

	//DrawGraph(0, 0, imageManage.GetID(actorType_, "attack")[0], true);
	DrawCircle(mousePos_.x, mousePos_.y, 5, 0xff0000, true);
}

void Scene::SelectActorTypeDraw(void)
{
	DrawFormatString(0, 300, 0xfffff, "←:一つ前のｱｸﾀｰﾀｲﾌﾟ,→:次のｱｸﾀｰﾀｲﾌﾟ");
	DrawFormatString(0, 30, 0xffffff, actorTypeName_[static_cast<int>(actorType_)].c_str());
	DrawFormatString(0, 330, 0xffffff, "決定後Enterｷｰで次へ");

}

void Scene::SelectAnimationDraw(void)
{
	auto& imageManager = ImageManager::GetInstance();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawBox(300, 200, 450, 216, 0xffff00, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawFormatString(50, 200, 0xffffff, "ｱﾆﾒｰｼｮﾝ名を入力してください:");

	DrawFormatString(500, 30, 0xff00ff, "ｱﾆﾒｰｼｮﾝ一覧");
	DrawFormatString(0, 330, 0xffffff, "決定後Enterｷｰで次へ");


	auto animName = imageManager.GetActionSet(actorType_);
	int i = 0;
	for (auto set : animName)
	{
		DrawFormatString(500, 50 + (i * 30), 0xffffff, set.first.c_str());
		i++;
	}
	// ｱﾆﾒｰｼｮﾝ名入力
	InputAnimation();

}

void Scene::EditDraw(void)
{
	DrawBox(0, 0, Screen_x, Screen_y, 0x0000ff, true);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
	DrawBox((Screen_x / 2) - (editScreenX_ / 2),
		(Screen_y / 2) - (editScreenY_ / 2),
		(Screen_x / 2) + (editScreenX_ / 2),
		(Screen_y / 2) + (editScreenY_ / 2),
		0xffffff, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	auto& imagemanager = ImageManager::GetInstance();

	//DrawFormatString(0, 0, 0xffffff, inputAnimationName_);
	DrawRotaGraph(spritePos_.x, spritePos_.y,
		1.0f,0.0f,
		imagemanager.GetID(actorType_, inputAnimationName_)[currentFrame_],
		true, false);

	if (actionRect_.size() > 0)
	{
		if (!actionRect_[currentFrame_].empty())
		{
			for (auto rects : actionRect_[currentFrame_])
			{
				rects->Draw();
			}
		}
	}

	DrawFormatString(180, 100, 0xffffff, "mPos.x:%d\nmPos.y:%d", mousePos_.x, mousePos_.y);
	DrawFormatString(0, 50, 0xffffff, "ctrl+Sで矩形ﾃﾞｰﾀをｾｰﾌﾞし、次ﾌﾚｰﾑに移行");

	DrawFormatString(0, 80, 0xffffff, "ﾌﾚｰﾑ");
	DrawFormatString(20, 100 + (currentFrame_ * 20), 0xffffff, "←現在ﾌﾚｰﾑ");
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawBox(0, 100 + (currentFrame_ * 20), 20, 120 + (currentFrame_ * 20), 0xffff00, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	for (int f = 0; f < maxFrame_; f++)
	{
		DrawFormatString(0, 100 + (f * 20), 0xffffff, "%d", f);
	}


	switch (turn_)
	{
	case EditTurn::Start:
		DrawFormatString(300, 0, 0x00ff00, "左クリックで始点を決定");
		break;
	case EditTurn::End:
		DrawFormatString(300, 0, 0x00ff00, "右クリックで終点を決定");
		break;
	case Max:
		DrawFormatString(300, 0, 0xff0000, "error");
		break;
	default:
		break;
	}

	switch (rectType_)
	{
	case 0:
		DrawFormatString(300, Screen_y/2 - 100, 0xff0000, "AttckRect");
		DrawCircle(mousePos_.x, mousePos_.y, 3.0f, 0xff0000, true);
		break;
	case 1:
		DrawFormatString(300, Screen_y / 2 - 100, 0x0000ff, "DamageRect");
		DrawCircle(mousePos_.x, mousePos_.y, 3.0f, 0x0000ff, true);
		break;
	default:
		DrawFormatString(300, Screen_y / 2 - 100, 0xff0000, "error");
		break;
	}
}

void Scene::LoadRectDraw(void)
{
	if (!actionRect_[currentFrame_].empty())
	{
		for (auto rects : actionRect_[currentFrame_])
		{
			rects->Draw();
		}
	}
}

void Scene::EndDraw(void)
{
}
