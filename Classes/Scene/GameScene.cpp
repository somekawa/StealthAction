/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

 // UI関連(スキルクールタイム表示やその他UIの表示)処理とゲームオーバーシーンへの遷移処理担当
#include "TitleScene.h"
#include "GameOverScene.h"
#include "ClearScene.h"
#include "_Debug/_DebugDispOutCC.h"
#include "_Debug/_DebugConOut.h"
#include "CameraManager.h"
#include "obj/Player.h"
#include "obj/Enemy/Enemy.h"
#include "Loader/CollisionLoader.h"
#include "Map/GameMap.h"
#include "SoundMng.h"
#include "Skill/SkillBase.h"
#include "Map/MapMenu.h"
#include "PoseMenu.h"
#include "Shader/PlayerShader.h"
#include "Shader/ResShadowShader.h"
#include "HPGauge.h"

USING_NS_CC;

Scene* Game::createScene()
{
	return Game::create();
}

Game::~Game()
{
	lpEffectMng.ClearPool();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in GameScene.cpp\n");
}

// on "init" you need to initialize your instance
bool Game::init()
{
	if (!Scene::init())
	{
		return false;
	}
	const auto visibleSize = Director::getInstance()->getVisibleSize();
	const Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// サウンド
	lpSoundMng.AddSound("BattleBGM", "BGM/sento.mp3", SoundType::BGM);
	lpSoundMng.PlayBySoundName("BattleBGM", 0.5f);

	for (auto& layer : layer_)
	{
		layer = Layer::create();
		this->addChild(layer);
	}
	// player用レイヤー
	layer_[(int)zOrder::CHAR_PL]->setTag((int)zOrder::CHAR_PL);
	layer_[(int)zOrder::CHAR_PL]->setGlobalZOrder((int)zOrder::CHAR_PL);
	// enemy用レイヤー
	layer_[(int)zOrder::CHAR_ENEMY]->setTag((int)zOrder::CHAR_ENEMY);
	layer_[(int)zOrder::CHAR_ENEMY]->setGlobalZOrder((int)zOrder::CHAR_ENEMY);
	// ui用レイヤー
	layer_[(int)zOrder::FRONT]->setTag((int)zOrder::FRONT);
	layer_[(int)zOrder::FRONT]->setGlobalZOrder((int)zOrder::FRONT);
	// effect用ﾚｲﾔｰ
	layer_[(int)zOrder::EFFECT]->setTag((int)zOrder::EFFECT);
	layer_[(int)zOrder::EFFECT]->setGlobalZOrder((int)zOrder::EFFECT);
	// bg用レイヤー
	layer_[(int)zOrder::BG]->setTag((int)zOrder::BG);
	layer_[(int)zOrder::BG]->setGlobalZOrder((int)zOrder::BG);
	// debug用レイヤー
	layer_[(int)zOrder::DEBUG]->setTag((int)zOrder::DEBUG);
	layer_[(int)zOrder::DEBUG]->setGlobalZOrder((int)zOrder::DEBUG);
	// playerList_にplayer追加
	// 引数に接続しているプレイヤー数を記述
	AddPlayer(1);
	generateEnemyNum_ = 0;
	respawnFlag_ = false;
	isChanged_ = false;

	// fireball の各アニメーションのキャッシュ登録
	lpAnimMng.FireBallClear();
	lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/Fireball", "fireball_normal", 4, 0.08f, true);
	lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/Fireball", "fireball_impact", 5, 0.08f, false);

	this->setName("GameScene");

	const auto _Gbg = LayerGradient::create(Color4B::BLACK, Color4B::WHITE);
	layer_[(int)zOrder::BG]->addChild(_Gbg);

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	// 攻撃ボタン
	// ボタンの押下/非押下はOPRTのほうでselected関数を呼んで管理
	auto attackBtn = MenuItemImage::create(
		"image/button1.png",
		"image/button1_select.png");
	//auto attackBtnSp = Sprite::create("image/button1.png");
	attackBtn->setName("attackBtn");
	attackBtn->setAnchorPoint(Vec2(0.5f, 0.0f));
	float x = origin.x + visibleSize.width - attackBtn->getContentSize().width / 2.0f - 20.0f;
	float y = origin.y;
	attackBtn->setPosition(Vec2(x, y));
	layer_[(int)zOrder::FRONT]->addChild(attackBtn, 0);

	// ジャンプボタン
	auto jumpBtn = MenuItemImage::create(
		"image/button2.png",
		"image/button2_select.png");
	jumpBtn->setName("jumpBtn");
	jumpBtn->setAnchorPoint(Vec2(0.5f, 0.0f));
	jumpBtn->setPosition(Vec2(x - jumpBtn->getContentSize().width * 2.0f + 50.0f, y));
	layer_[(int)zOrder::FRONT]->addChild(jumpBtn, 0);

	// ダッシュ(回避)
	auto dashBtn = MenuItemImage::create(
		"image/button3.png",
		"image/button2_select.png");
	dashBtn->setName("dashBtn");
	dashBtn->setAnchorPoint(Vec2(0.5f, 0.0f));
	dashBtn->setPosition(Vec2(x - dashBtn->getContentSize().width * 2.0f + 70.0f, y + 90.0f));
	layer_[(int)zOrder::FRONT]->addChild(dashBtn, 0);

	// トランスフォーム(light->dark)
	auto transformBtn_toDark = MenuItemImage::create(
		"image/button4.png",
		"image/button2_select.png");
	transformBtn_toDark->setName("transformBtn_toDark");
	transformBtn_toDark->setAnchorPoint(Vec2(0.5f, 0.0f));
	transformBtn_toDark->setPosition(Vec2(x - transformBtn_toDark->getContentSize().width * 2.0f + 160.0f, y + 120.0f));
	layer_[(int)zOrder::FRONT]->addChild(transformBtn_toDark, 0);

	// トランスフォーム(dark->light)
	auto transformBtn_toLight = MenuItemImage::create(
		"image/button5.png",
		"image/button2_select.png");
	transformBtn_toLight->setName("transformBtn_toLight");
	transformBtn_toLight->setAnchorPoint(Vec2(0.5f, 0.0f));
	transformBtn_toLight->setPosition(Vec2(x - transformBtn_toLight->getContentSize().width * 2.0f + 160.0f, y + 120.0f));
	transformBtn_toLight->setVisible(false);	// 初期は見えない
	layer_[(int)zOrder::FRONT]->addChild(transformBtn_toLight, 0);

	// 移動バーチャルパッド用
	auto startSp = Sprite::create("image/virtualPad.png");
	startSp->setName("startSp");
	startSp->setPosition(Vec2(150.0f, 150.0f));
	layer_[(int)zOrder::FRONT]->addChild(startSp, 1);
#endif

	const auto itemBtn = MenuItemImage::create(
		"image/skilltest.png",
		"image/skilltest.png");
	itemBtn->setName("itemBtn");
	itemBtn->setPosition(Vec2(visibleSize.width / 2.0f - (itemBtn->getContentSize().width + itemBtn->getContentSize().width / 2.0f), itemBtn->getContentSize().height / 2.0f));
	layer_[(int)zOrder::FRONT]->addChild(itemBtn, 0);

	// スキルA(左から2番目)
	const auto skillABtn = MenuItemImage::create(
		"image/skill_magic.png",
		"image/skilltest.png");
	skillABtn->setName("skillABtn");
	skillABtn->setPosition(Vec2(visibleSize.width / 2.0f - skillABtn->getContentSize().width / 2, skillABtn->getContentSize().height / 2.0f));
	layer_[(int)zOrder::FRONT]->addChild(skillABtn, 0);

	// スキルB(左から3番目)
	const auto skillBBtn = MenuItemImage::create(
		"image/skill_fire.png",
		"image/skilltest.png");
	skillBBtn->setName("skillBBtn");
	skillBBtn->setPosition(Vec2(visibleSize.width / 2.0f + skillBBtn->getContentSize().width / 2.0f, skillBBtn->getContentSize().height / 2.0f));
	layer_[(int)zOrder::FRONT]->addChild(skillBBtn, 0);

	// スキルC(左から4番目)
	const auto skillCBtn = MenuItemImage::create(
		"image/skill_heal.png",
		"image/skilltest.png");
	skillCBtn->setName("skillCBtn");
	skillCBtn->setPosition(Vec2(visibleSize.width / 2.0f + (skillCBtn->getContentSize().width + skillCBtn->getContentSize().width / 2.0f), skillCBtn->getContentSize().height / 2.0f));
	layer_[(int)zOrder::FRONT]->addChild(skillCBtn, 0);

	float BtnSize = 0.0f;
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	//メニュー遷移ボタン
	auto menuBtn = MenuItemImage::create(
		"image/keep_button1.png",
		"image/keep_button1_select.png");
	menuBtn->setName("menuBtn");
	menuBtn->setPosition(Vec2(visibleSize.width - (menuBtn->getContentSize().width * 0.25f) / 2.0f - 10.0f, visibleSize.height - (menuBtn->getContentSize().height * 0.25f) / 2.0f - 10.0f));
	menuBtn->setScale(0.25f);
	layer_[(int)zOrder::FRONT]->addChild(menuBtn, 0);
	BtnSize = menuBtn->getContentSize().width;
#endif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	const auto menuGuide = Sprite::create("image/menuGuide.png");
	const auto sprite_size = menuGuide->getContentSize();
	menuGuide->setScale(0.3f);
	const auto menuPosX = visibleSize.width - menuGuide->getContentSize().width / 2.0f * menuGuide->getScale() - BtnSize;
	menuGuide->setPosition(Vec2(menuPosX, visibleSize.height - (menuGuide->getContentSize().height / 2.0f) * menuGuide->getScale()));
	this->addChild(menuGuide);

	// map読み込み
	gameMap_ = std::make_shared<GameMap>(*layer_[(int)zOrder::BG]);

	// カメラ作成
	cameraManager_ = std::make_shared<CameraManager>();

	cameraManager_->AddCamera(*this, visibleSize, CameraType::PLAYER1, CameraFlag::USER1);
	cameraManager_->AddCamera(*this, visibleSize, CameraType::UI, CameraFlag::USER2);


	playerShader_ = std::make_shared<PlayerShader>();
	resShadowShader_ = std::make_shared<ResShadowShader>();

	const auto player = (Player*)layer_[static_cast<int>(zOrder::CHAR_PL)]->getChildByName("player1");

	const auto plHPGauge = HPGauge::createHPGauge(*player, 0);
	layer_[(int)zOrder::FRONT]->addChild(plHPGauge, 1);
	plHPGauge->setName("PL_HPgauge");
	plHPGauge->setTag(100);
	plHPGauge->setPosition(visibleSize.width / 10, visibleSize.height - (plHPGauge->getContentSize().height + plHPGauge->getContentSize().height / 4));
	plHPGauge->scheduleUpdate();

	// プレイヤーのバー背景(ゲージの減少を見やすくする為)
	const auto plHpGaugeBack = Sprite::create("image/HPbar_back.png");
	plHpGaugeBack->setPosition(plHPGauge->getPosition());
	plHpGaugeBack->setAnchorPoint(Vec2(0.0f, 0.5f));
	layer_[(int)zOrder::FRONT]->addChild(plHpGaugeBack, 0);

	// プレイヤーHPゲージカバー
	const auto plHpGaugeCover = Sprite::create("image/HPbar_cover.png");
	plHpGaugeCover->setPosition(Vec2(plHPGauge->getPosition().x - 40.0f, plHPGauge->getPosition().y));
	plHpGaugeCover->setAnchorPoint(Vec2(0.0f, 0.5f));
	layer_[(int)zOrder::FRONT]->addChild(plHpGaugeCover, 0);

	// 敵の出現や消去等を管理するManagerを生成
	enemyManager_ = std::make_unique<EnemyManager>(*layer_[static_cast<int>(zOrder::CHAR_ENEMY)], *layer_[static_cast<int>(zOrder::FRONT)], *player);

	// 敵のｱﾆﾒｰｼｮﾝ関係、ﾋﾞﾍｲﾋﾞｱの初期化
	enemyManager_->Initialize();

	const auto skillSprite_ = SkillBase::createSkillBase();
	layer_[(int)zOrder::FRONT]->addChild(skillSprite_, 0);
	skillSprite_->setName("skillSprite");
	skillSprite_->setPosition(0, 0);

	// エフェクト用オブジェクトプール作成
	lpEffectMng.CreatePools(*layer_[static_cast<int>(zOrder::EFFECT)]);

	// スキルCT表示用の変数を初期化
	skillTimeCnt_.resize(3);
	for (auto& data : skillTimeCnt_)
	{
		data = std::make_pair(0, 0);
	}

	// マップメニュー
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	auto listener = cocos2d::EventListenerKeyboard::create();
	listener->onKeyPressed = [this](cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* keyEvent)
	{
		if (keyCode == EventKeyboard::KeyCode::KEY_TAB)
		{
			// ここで一度削除しておかないとエラーにつながる
			if (this->getChildByTag((int)zOrder::FRONT)->getChildByName("magicCT") != nullptr)
			{
				this->getChildByTag((int)zOrder::FRONT)->getChildByName("skillABtn")->setColor(Color3B(255, 255, 255));
				this->getChildByTag((int)zOrder::FRONT)->removeChildByName("magicCT");
			}
			if (this->getChildByTag((int)zOrder::FRONT)->getChildByName("fireBallCT") != nullptr)
			{
				this->getChildByTag((int)zOrder::FRONT)->getChildByName("skillBBtn")->setColor(Color3B(255, 255, 255));
				this->getChildByTag((int)zOrder::FRONT)->removeChildByName("fireBallCT");
			}
			if (this->getChildByTag((int)zOrder::FRONT)->getChildByName("healCT") != nullptr)
			{
				this->getChildByTag((int)zOrder::FRONT)->getChildByName("skillCBtn")->setColor(Color3B(255, 255, 255));
				this->getChildByTag((int)zOrder::FRONT)->removeChildByName("healCT");
			}
			Director::getInstance()->pushScene(PoseMenu::CreatePoseMenu(*gameMap_));
		}
	};
	listener->onKeyReleased = [this](cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* keyEvent)
	{
		// 何も処理を行わない
	};
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
#endif // (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	
	// レイヤーにカメラをセット(layerにsprite等をすべてぶら下げた後にマスクを設定したほうがよい)
	layer_[static_cast<int>(zOrder::CHAR_PL)]->setCameraMask(static_cast<int>(CameraFlag::USER1));
	layer_[static_cast<int>(zOrder::CHAR_ENEMY)]->setCameraMask(static_cast<int>(CameraFlag::USER1));
	layer_[static_cast<int>(zOrder::BG)]->setCameraMask(static_cast<int>(CameraFlag::USER1));
	layer_[(int)zOrder::FRONT]->setCameraMask(static_cast<int>(CameraFlag::USER2));

	this->scheduleUpdate();
	return true;
}

void Game::update(float sp)
{
	// 今のSceneがGameScene以外だった場合、returnさせる
	if (Director::getInstance()->getRunningScene()->getName() != "GameScene")
	{
		return;
	}

	// トランジション中のupdate更新を防ぐ
	if (isChanged_)
	{
		return;
	}

	// gameoversceneへのフラグがtrueになっていたら、画面遷移を行う
	const auto player = (Player*)layer_[static_cast<int>(zOrder::CHAR_PL)]->getChildByName("player1");
	if (gameMap_->IsClear())
	{
		lpEffectMng.ClearPool();
		lpSoundMng.SetPauseAll(true);
		isChanged_ = true;
		Scene* scene = ClearScene::CreateClearScene();
		Director::getInstance()->replaceScene(TransitionFade::create(1.0f, scene, Color3B::WHITE));
		CC_SAFE_RELEASE(this);
		return;
	}

	if (player->GetGameOverFlg())
	{
		lpEffectMng.ClearPool();
		lpSoundMng.SetPauseAll(true);
		isChanged_ = true;
		Scene* scene = GameOverScene::CreateGameOverScene();
		Director::getInstance()->replaceScene(TransitionFade::create(1.0f, scene, Color3B::WHITE));
		CC_SAFE_RELEASE(this);
		return;
	}
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	// ANDROID時にメニューボタン押下したらTAB押下と同じ扱いをする
	auto label1 = this->getChildByTag((int)zOrder::FRONT)->getChildByName("menuBtn");
	if (((MenuItemImage*)label1)->isSelected())
	{
		// メニュー表示時には選択状態を解除する(これを書かないと、continueを押した瞬間にまたメニューに切り替わってしまう)
		((MenuItemImage*)label1)->unselected();

		// ここで一度削除しておかないとエラーにつながる
		if (this->getChildByTag((int)zOrder::FRONT)->getChildByName("magicCT") != nullptr)
		{
			this->getChildByTag((int)zOrder::FRONT)->getChildByName("skillABtn")->setColor(Color3B(255, 255, 255));
			this->getChildByTag((int)zOrder::FRONT)->removeChildByName("magicCT");
		}
		if (this->getChildByTag((int)zOrder::FRONT)->getChildByName("fireBallCT") != nullptr)
		{
			this->getChildByTag((int)zOrder::FRONT)->getChildByName("skillBBtn")->setColor(Color3B(255, 255, 255));
			this->getChildByTag((int)zOrder::FRONT)->removeChildByName("fireBallCT");
		}
		if (this->getChildByTag((int)zOrder::FRONT)->getChildByName("healCT") != nullptr)
		{
			this->getChildByTag((int)zOrder::FRONT)->getChildByName("skillCBtn")->setColor(Color3B(255, 255, 255));
			this->getChildByTag((int)zOrder::FRONT)->removeChildByName("healCT");
		}
		Director::getInstance()->pushScene(PoseMenu::CreatePoseMenu(*gameMap_));
	}

#endif // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

	const auto skillBaseSp = (SkillBase*)layer_[static_cast<int>(zOrder::FRONT)]->getChildByName("skillSprite");
	skillBaseSp->UpDate(sp);
	
	for (auto enemy : enemyManager_->GetEnemies())
	{
		enemy->OnHit(player->GetAttackCol());
		player->OnHit(enemy->GetAttackCol());
		enemy->OnHit(skillBaseSp->GetEffectData());
	}

	if (gameMap_->ChangeFloor())
	{
		// 敵の生成数リセット
		enemyManager_->ResetEnemyNum();
		auto enemyNum = layer_[static_cast<int>(zOrder::CHAR_ENEMY)]->getChildrenCount();
		layer_[static_cast<int>(zOrder::CHAR_ENEMY)]->removeAllChildrenWithCleanup(true);
		for (auto hp :layer_[static_cast<int>(zOrder::FRONT)]->getChildren())
		{
			if (hp->getTag() == 101)
			{
				hp->setTag(0);
			}
		}
		enemyNum = layer_[static_cast<int>(zOrder::CHAR_ENEMY)]->getChildrenCount();

		// ﾌﾛｱ変更後1回だけ初期の敵の数だけｲﾝｽﾀﾝｽ
		enemyManager_->CreateInitialEnemyOnFloor(1);
	}
	const auto enemyNum = layer_[static_cast<int>(zOrder::CHAR_ENEMY)]->getChildrenCount();
	gameMap_->Update(*player, enemyNum);
	// 敵のスポーン管理
	enemyManager_->Update();
	
	// カメラスクロール
	cameraManager_->ScrollCamera(layer_[static_cast<int>(zOrder::CHAR_PL)]->getChildByName("player1")->getPosition(), CameraType::PLAYER1);
	
	// シェーダセット
	if (player->GetPlayerColor() == "player_Light_")
	{
		playerShader_->SetShader(*player, Vec3(0, 0, 0));
	}
	
	const auto enemies = layer_[static_cast<int>(zOrder::CHAR_ENEMY)]->getChildren();
	for (auto& enemy : enemies)
	{
		playerShader_->SetShader(*enemy, Vec3(1, 0, 0));
	}

	// スキルCT更新
	SkillCntUpdate("magic"		, "skillABtn", 0);
	SkillCntUpdate("fireBall"	, "skillBBtn", 1);
	SkillCntUpdate("heal"		, "skillCBtn", 2);
}

void Game::AddPlayer(int playerNum)
{
	const auto skillBasePtr = (SkillBase*)layer_[static_cast<int>(zOrder::FRONT)]->getChildByName("skillSprite");
	const auto enemies = layer_[static_cast<int>(zOrder::CHAR_ENEMY)]->getChildren();

	// player数resize
	for (int p = 0; p < playerNum; p++)
	{
		const auto plSprite = Player::CreatePlayer(100, *layer_[static_cast<int>(zOrder::CHAR_PL)],*layer_[static_cast<int>(zOrder::CHAR_ENEMY)],skillBasePtr);
		plSprite->setName("player" + std::to_string(p + 1));
		plSprite->setScale(3.0f);
		plSprite->setAnchorPoint(Vec2(0.5f, 0.0f));

		// プレイヤーをキャラクター用のレイヤーの子供にする
		layer_[static_cast<int>(zOrder::CHAR_PL)]
			->addChild(plSprite, playerNum);
		plSprite->scheduleUpdate();
	}
}

void Game::SkillCntUpdate(std::string skillName,std::string buttonName,int skillNum)
{
	// スキルのCTに合わせて明度の調整を行うようにする
	// 該当スキルのCTが0でないときは明度を暗くする
	const auto director = Director::getInstance();
	const std::string nameCT = skillName + "CT";

	auto createLambda = [&]() {
		std::string number = StringUtils::format("%d", ((SkillBase*)director)->GetSkillCTTime(skillName));
		auto label = Label::createWithTTF(number.c_str(), "fonts/PixelMplus12-Regular.ttf", 48);
		label->setPosition(this->getChildByTag((int)zOrder::FRONT)->getChildByName(buttonName)->getPosition());
		label->setName(nameCT);
		layer_[(int)zOrder::FRONT]->addChild(label, 3);
		label->setCameraMask(static_cast<int>(CameraFlag::USER2));	// カメラマスクを設定しないと、アイコンの後ろに隠れてしまう
	};

	if (((SkillBase*)director)->GetSkillCT(skillName))
	{
		// CTの表示が不必要な為、ラベルを削除する
		skillTimeCnt_[skillNum].second = 0;
		if (this->getChildByTag((int)zOrder::FRONT)->getChildByName(nameCT) != nullptr)
		{
			this->getChildByTag((int)zOrder::FRONT)->removeChildByName(nameCT);
		}
		this->getChildByTag((int)zOrder::FRONT)->getChildByName(buttonName)->setColor(Color3B(255, 255, 255));
	}
	else
	{
		// CTの表示が必要かを確認する
		if (((SkillBase*)director)->GetSkillCTTime(skillName) >= 0)
		{
			if (this->getChildByTag((int)zOrder::FRONT)->getChildByName(buttonName)->getColor() == Color3B(255,255,255))
			{
				this->getChildByTag((int)zOrder::FRONT)->getChildByName(buttonName)->setColor(Color3B(150, 150, 150));
				skillTimeCnt_[skillNum].second = ((SkillBase*)director)->GetSkillCTTime(skillName);
				skillTimeCnt_[skillNum].first = skillTimeCnt_[skillNum].second;
				createLambda();
			}

			// CTの更新
			skillTimeCnt_[skillNum].first = skillTimeCnt_[skillNum].second;
			skillTimeCnt_[skillNum].second = ((SkillBase*)director)->GetSkillCTTime(skillName);

			if (this->getChildByTag((int)zOrder::FRONT)->getChildByName(nameCT) != nullptr)
			{
				// ラベルの削除処理
				// 毎フレーム削除処理をすると重くなるので、数値が変わったときにだけ削除するようにする
				if (skillTimeCnt_[skillNum].first != skillTimeCnt_[skillNum].second)
				{
					this->getChildByTag((int)zOrder::FRONT)->removeChildByName(nameCT);
					createLambda();
				}
			}
		}
	}
}