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

#include "GameScene.h"
#include "TitleScene.h"
#include "GameOverScene.h"
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
//#include "Debug/DebugDraw.h"
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
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// サウンド
	lpSoundMng.AddSound("bgm", "BGM/BGM_piano.mp3", SoundType::BGM);
	//lpSoundMng.PlayBySoundName("bgm", 0.5f);

	for (auto& layer : layer_)
	{
		layer = Layer::create();
		this->addChild(layer);
	}
	// player用レイヤー
	layer_[(int)zOlder::CHAR_PL]->setTag((int)zOlder::CHAR_PL);
	layer_[(int)zOlder::CHAR_PL]->setGlobalZOrder((int)zOlder::CHAR_PL);
	// enemy用レイヤー
	layer_[(int)zOlder::CHAR_ENEMY]->setTag((int)zOlder::CHAR_ENEMY);
	layer_[(int)zOlder::CHAR_ENEMY]->setGlobalZOrder((int)zOlder::CHAR_ENEMY);
	// ui用レイヤー
	layer_[(int)zOlder::FRONT]->setTag((int)zOlder::FRONT);
	layer_[(int)zOlder::FRONT]->setGlobalZOrder((int)zOlder::FRONT);
	// effect用ﾚｲﾔｰ
	layer_[(int)zOlder::EFFECT]->setTag((int)zOlder::EFFECT);
	layer_[(int)zOlder::EFFECT]->setGlobalZOrder((int)zOlder::EFFECT);
	// bg用レイヤー
	layer_[(int)zOlder::BG]->setTag((int)zOlder::BG);
	layer_[(int)zOlder::BG]->setGlobalZOrder((int)zOlder::BG);
	// debug用レイヤー
	layer_[(int)zOlder::DEBUG]->setTag((int)zOlder::DEBUG);
	layer_[(int)zOlder::DEBUG]->setGlobalZOrder((int)zOlder::DEBUG);
	// playerList_にplayer追加
	// 引数に接続しているプレイヤー数を記述
	AddPlayer(1);
	// enemyList_に敵追加
	//AddEnemy(ActorType::Imp);
	// assassinを追加

	//AddEnemy(ActorType::Assassin);
	//AddEnemy(ActorType::TwistedCultist);
	//AddEnemy(ActorType::Assassin);

	// 敵を生成した回数(多分いらない)
	// 敵のsetNameするために用意したもの
	generateEnemyNum_ = 0;
	// 敵が死んだらとりあえずリスポーンするフラグ
	respawnFlag_ = false;

	// fireball の各ｱﾆﾒｰｼｮﾝのｷｬｯｼｭ登録
	lpAnimMng.addAnimationCache("Fireball", "fireball_normal", 4, 0.08f, true);
	lpAnimMng.addAnimationCache("Fireball", "fireball_impact", 5, 0.08f, false);

	this->setName("GameScene");

	auto _Gbg = LayerGradient::create(Color4B::BLACK, Color4B::WHITE);
	layer_[(int)zOlder::BG]->addChild(_Gbg);

	// ボタンテスト用
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	// 攻撃ボタン
	// ボタンの押下/非押下はOPRTのほうでselected関数を呼んで管理
	auto attackBtn = MenuItemImage::create(
	"image/button1.png",
	"image/button1_select.png");
	//auto attackBtnSp = Sprite::create("image/button1.png");
	attackBtn->setName("attackBtn");
	attackBtn->setAnchorPoint(Vec2(0.5f, 0.0f));
	float x = origin.x + visibleSize.width - attackBtn->getContentSize().width / 2 - 20;
	float y = origin.y + attackBtn->getContentSize().height / 2;
	attackBtn->setPosition(Vec2(x, y));
	layer_[(int)zOlder::FRONT]->addChild(attackBtn, 0);

	// ジャンプボタン
	// 押してる最中に画像が消えるのは仕様です
	auto jumpBtn = MenuItemImage::create(
		"image/button2.png",
		"image/button2_select.png");
	//auto jumpBtnSp = Sprite::create("image/button2.png");
	jumpBtn->setName("jumpBtn");
	jumpBtn->setAnchorPoint(Vec2(0.5f, 0.0f));
	jumpBtn->setPosition(Vec2(x - jumpBtn->getContentSize().width * 2 + 50, y));
	layer_[(int)zOlder::FRONT]->addChild(jumpBtn, 0);

	// ダッシュ(回避)
	auto dashBtn = MenuItemImage::create(
		"image/button3.png",
		"image/button2_select.png");
	dashBtn->setName("dashBtn");
	dashBtn->setAnchorPoint(Vec2(0.5f, 0.0f));
	dashBtn->setPosition(Vec2(x - dashBtn->getContentSize().width * 2 + 70, y + 90));
	layer_[(int)zOlder::FRONT]->addChild(dashBtn, 0);

	// トランスフォーム(light->dark)
	auto transformBtn_toDark = MenuItemImage::create(
		"image/button4.png",
		"image/button2_select.png");
	transformBtn_toDark->setName("transformBtn_toDark");
	transformBtn_toDark->setAnchorPoint(Vec2(0.5f, 0.0f));
	transformBtn_toDark->setPosition(Vec2(x - transformBtn_toDark->getContentSize().width * 2 + 160, y + 120));
	layer_[(int)zOlder::FRONT]->addChild(transformBtn_toDark, 0);

	// トランスフォーム(dark->light)
	auto transformBtn_toLight = MenuItemImage::create(
		"image/button5.png",
		"image/button2_select.png");
	transformBtn_toLight->setName("transformBtn_toLight");
	transformBtn_toLight->setAnchorPoint(Vec2(0.5f, 0.0f));
	transformBtn_toLight->setPosition(Vec2(x - transformBtn_toLight->getContentSize().width * 2 + 160, y + 120));
	transformBtn_toLight->setVisible(false);	// 初期は見えない
	layer_[(int)zOlder::FRONT]->addChild(transformBtn_toLight, 0);

	// 移動バーチャルパッド用
	auto startSp = Sprite::create("CloseNormal.png");
	startSp->setName("startSp");
	startSp->setPosition(Vec2(150.0f, 150.0f));
	layer_[(int)zOlder::FRONT]->addChild(startSp, 1);
#endif

	// とりあえず今はくっつけた感じで表示してます
	// アイテム(左から1番目)
	auto itemBtn = MenuItemImage::create(
		"image/skilltest.png",
		"image/skilltest.png");
	itemBtn->setName("itemBtn");
	itemBtn->setPosition(Vec2(visibleSize.width / 2 - (itemBtn->getContentSize().width + itemBtn->getContentSize().width / 2), itemBtn->getContentSize().height / 2));
	layer_[(int)zOlder::FRONT]->addChild(itemBtn, 0);

	// スキルA(左から2番目)
	auto skillABtn = MenuItemImage::create(
		"image/skill_magic.png",
		"image/skilltest.png");
	skillABtn->setName("skillABtn");
	skillABtn->setPosition(Vec2(visibleSize.width / 2 - skillABtn->getContentSize().width / 2, skillABtn->getContentSize().height / 2));
	layer_[(int)zOlder::FRONT]->addChild(skillABtn, 0);

	// スキルB(左から3番目)
	auto skillBBtn = MenuItemImage::create(
		"image/skill_fire.png",
		"image/skilltest.png");
	skillBBtn->setName("skillBBtn");
	skillBBtn->setPosition(Vec2(visibleSize.width / 2 + skillBBtn->getContentSize().width / 2, skillBBtn->getContentSize().height / 2));
	layer_[(int)zOlder::FRONT]->addChild(skillBBtn, 0);

	// スキルC(左から4番目)
	auto skillCBtn = MenuItemImage::create(
		"image/skill_heal.png",
		"image/skilltest.png");
	skillCBtn->setName("skillCBtn");
	skillCBtn->setPosition(Vec2(visibleSize.width / 2 + (skillCBtn->getContentSize().width + skillCBtn->getContentSize().width / 2), skillCBtn->getContentSize().height / 2));
	layer_[(int)zOlder::FRONT]->addChild(skillCBtn, 0);

	//メニュー遷移ボタン
	auto menuBtn = MenuItemImage::create(
		"image/keep_button1.png",
		"image/keep_button1_select.png");
	menuBtn->setName("menuBtn");
	menuBtn->setPosition(Vec2(visibleSize.width-(menuBtn->getContentSize().width*0.25)/2-10, visibleSize.height-(menuBtn->getContentSize().height * 0.25)/2-10));
	menuBtn->setScale(0.25f);
		layer_[(int)zOlder::FRONT]->addChild(menuBtn, 0);

	// map読み込み
	gameMap_ = std::make_shared<GameMap>(*layer_[(int)zOlder::BG]);
	
	// カメラ作成
	cameraManager_ = std::make_shared<CameraManager>();
	
	cameraManager_->AddCamera(*this, visibleSize,CameraType::PLAYER1, CameraFlag::USER1);
	cameraManager_->AddCamera(*this, visibleSize, CameraType::UI, CameraFlag::USER2);


	playerShader_ = std::make_shared<PlayerShader>();
	resShadowShader_ = std::make_shared<ResShadowShader>();
	//auto fileUtiles = FileUtils::getInstance();
	//auto vertexSource = fileUtiles->getStringFromFile("Shader/OutLineTest.vert");
	//auto fragmentSource = fileUtiles->getStringFromFile("Shader/OutLineTest.frag");
	//program = backend::Device::getInstance()->newProgram(vertexSource.c_str(), fragmentSource.c_str());
	//programState = new backend::ProgramState(program);

	auto player = (Player*)layer_[static_cast<int>(zOlder::CHAR_PL)]->getChildByName("player1");

	auto plHPGauge = HPGauge::createHPGauge(*player, 0);
	layer_[(int)zOlder::FRONT]->addChild(plHPGauge, 1);
	plHPGauge->setName("PL_HPgauge");
	plHPGauge->setTag(100);
	plHPGauge->setPosition(visibleSize.width / 10, visibleSize.height - visibleSize.height / 10);

	plHPGauge->scheduleUpdate();

	// プレイヤーのバー背景(ゲージの減少を見やすくする為)
	auto plHpGaugeBack = Sprite::create("image/HPbar_back.png");
	plHpGaugeBack->setPosition(plHPGauge->getPosition());
	plHpGaugeBack->setAnchorPoint(Vec2(0.0f, 0.5f));
	layer_[(int)zOlder::FRONT]->addChild(plHpGaugeBack, 0);

	//// 敵の出現や消去等を管理するManagerを生成
	enemyManager_ = std::make_unique<EnemyManager>(*layer_[static_cast<int>(zOlder::CHAR_ENEMY)], *layer_[static_cast<int>(zOlder::FRONT)], *player);

	//// 敵のｱﾆﾒｰｼｮﾝ関係、ﾋﾞﾍｲﾋﾞｱの初期化
	enemyManager_->Initialize();
	//enemyManager_->CreateInitialEnemyOnFloor(3);
	//enemyManager_->CreateBoss(effectManager_);
	

	skillSprite = SkillBase::createSkillBase();
	layer_[(int)zOlder::FRONT]->addChild(skillSprite, 0);
	skillSprite->setName("skillSprite");
	skillSprite->setPosition(0,0);

	// 地図表示時の背景に設定したいやつ…
	//auto mapBgSprite = Sprite::create("image/mapBg.png");
	//layer_[(int)zOlder::FRONT]->addChild(mapBgSprite, 0);
	//mapBgSprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	//mapBgSprite->setVisible(false);
	//mapBgSprite->setName("mapBgSprite");


	// ｴﾌｪｸﾄ用ｵﾌﾞｼﾞｪｸﾄﾌﾟｰﾙ作成
	lpEffectMng.CreatePools(*layer_[static_cast<int>(zOlder::EFFECT)]);

	// マップメニュー
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	auto listener = cocos2d::EventListenerKeyboard::create();
	listener->onKeyPressed = [this](cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* keyEvent)
	{
		if (keyCode == EventKeyboard::KeyCode::KEY_F2)
		{
			Director::getInstance()->pushScene(PoseMenu::CreatePoseMenu(*gameMap_));
		}
		if (keyCode == EventKeyboard::KeyCode::KEY_F1)
		{
			debugMode = true;
		}
	};
	listener->onKeyReleased = [this](cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* keyEvent)
	{
		if (keyCode == EventKeyboard::KeyCode::KEY_F1)
		{
			debugMode = false;
		}
	};
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
#else
	/*auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [this](cocos2d::Touch* touch, cocos2d::Event* event)
	{
		return true;
	};*/
#endif // (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	debugMode = false;
	

	// レイヤーにカメラをセット(layerにsprite等をすべてぶら下げた後にマスクを設定したほうがよい)
	layer_[static_cast<int>(zOlder::CHAR_PL)]->setCameraMask(static_cast<int>(CameraFlag::USER1));
	layer_[static_cast<int>(zOlder::CHAR_ENEMY)]->setCameraMask(static_cast<int>(CameraFlag::USER1));
	layer_[static_cast<int>(zOlder::BG)]->setCameraMask(static_cast<int>(CameraFlag::USER1));
	layer_[(int)zOlder::FRONT]->setCameraMask(static_cast<int>(CameraFlag::USER2));

	debugSprite = Sprite::create();

	this->addChild(debugSprite);

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
	auto player = (Player*)layer_[static_cast<int>(zOlder::CHAR_PL)]->getChildByName("player1");
	if (player->GetGameOverFlg())
	{
		isChanged_ = true;
		Scene* scene = GameOverScene::CreateGameOverScene();
		Director::getInstance()->replaceScene(TransitionFade::create(1.0f, scene, Color3B::WHITE));
		CC_SAFE_RELEASE(this);
		return;
	}

	// ANDROID時にメニューボタン押下したらF2押下と同じ扱いをする
	auto label1 = this->getChildByTag((int)zOlder::FRONT)->getChildByName("menuBtn");
	if (((MenuItemImage*)label1)->isSelected())
	{
		Director::getInstance()->pushScene(PoseMenu::CreatePoseMenu(*gameMap_));
	}

	// SkillBase呼ぶテスト
	auto skillBaseSp = (SkillBase*)layer_[static_cast<int>(zOlder::FRONT)]->getChildByName("skillSprite");
	skillBaseSp->UpDate(sp);
	//auto player = (Player*)layer_[static_cast<int>(zOlder::CHAR_PL)]->getChildByName("player1");
	gameMap_->update(*player);


	// skillの攻撃対象選別 // 
	// 攻撃対象がﾌﾟﾚｲﾔｰに渡せる機能が欲しい
	// ﾌﾟﾚｲﾔｰが攻撃対象にする敵のﾀｰｹﾞｯﾄをﾀｯﾌﾟにより切り替える
	// 攻撃対象にする敵は一番最初はﾚｲﾔｰにいる最初の敵
	// それ以降はﾀｯﾌﾟで変えていく(今のところGameSceneで書くのが一番だと思う)
	for (auto enemy : enemyManager_->GetEnemies())
	{
		enemy->OnHit(player->GetAttackCol());
		player->OnHit(enemy->GetAttackCol());
		enemy->OnHit(skillBaseSp->GetEffectData());

		/*debugSprite->setPosition(skillBaseSp->GetEffectData().origin);
		debugSprite->setAnchorPoint({0, 0});
		debugSprite->setTextureRect(skillBaseSp->GetEffectData());*/
	}

	if (gameMap_->ChangeFloor())
	{
		// 敵の生成数のﾘｾｯﾄ(enemyNum_を0にする)
		enemyManager_->ResetEnemyNum();
		//effectManager_->Play(EffectType::EnemySpawn, Vec2(200.0f, 300.0f));
		auto enemyNum = layer_[static_cast<int>(zOlder::CHAR_ENEMY)]->getChildrenCount();
		//layer_[static_cast<int>(zOlder::CHAR_ENEMY)]->removeAllChildren();
		layer_[static_cast<int>(zOlder::CHAR_ENEMY)]->removeAllChildrenWithCleanup(true);
		for (auto hp :layer_[static_cast<int>(zOlder::FRONT)]->getChildren())
		{
			if (hp->getTag() == 101)
			{
				hp->setTag(0);
			}
		}
		enemyNum = layer_[static_cast<int>(zOlder::CHAR_ENEMY)]->getChildrenCount();

		// ﾌﾛｱ変更後1回だけ初期の敵の数だけｲﾝｽﾀﾝｽ
		enemyManager_->CreateInitialEnemyOnFloor(1);
	}

	// 敵のｽﾎﾟｰﾝを管理
	//enemyManager_->Update(effectManager_);
	
	// カメラスクロール
	cameraManager_->ScrollCamera(layer_[static_cast<int>(zOlder::CHAR_PL)]->getChildByName("player1")->getPosition(), CameraType::PLAYER1);
	
	// シェーダセット
	resShadowShader_->SetShader(*player);
	playerShader_->SetShader(*player, Vec3(0, 0, 0));
	
	auto enemies = layer_[static_cast<int>(zOlder::CHAR_ENEMY)]->getChildren();
	for (auto& enemy : enemies)
	{
		playerShader_->SetShader(*enemy, Vec3(1, 0, 0));
	}

	gameMap_->ColisionDebugDraw(debugMode);


	// スキルのCTに合わせて明度の調整を行うようにする
	// 該当スキルのCTが0でないときは明度を暗くする
	auto director = Director::getInstance();
	// スキルA
	if (!((SkillBase*)director)->GetSkillCT("magic"))
	{
		this->getChildByTag((int)zOlder::FRONT)->getChildByName("skillABtn")->setColor(Color3B(150, 150, 150));
	}
	else
	{
		this->getChildByTag((int)zOlder::FRONT)->getChildByName("skillABtn")->setColor(Color3B(255, 255, 255));
	}
	// スキルB
	if (!((SkillBase*)director)->GetSkillCT("enemySpawn"))
	{
		this->getChildByTag((int)zOlder::FRONT)->getChildByName("skillBBtn")->setColor(Color3B(150, 150, 150));
	}
	else
	{
		this->getChildByTag((int)zOlder::FRONT)->getChildByName("skillBBtn")->setColor(Color3B(255, 255, 255));
	}
	// スキルC
	if (!((SkillBase*)director)->GetSkillCT("heal"))
	{
		this->getChildByTag((int)zOlder::FRONT)->getChildByName("skillCBtn")->setColor(Color3B(150, 150, 150));
	}
	else
	{
		this->getChildByTag((int)zOlder::FRONT)->getChildByName("skillCBtn")->setColor(Color3B(255, 255, 255));
	}
}

void Game::AddPlayer(int playerNum)
{
	auto skillBasePtr = (SkillBase*)layer_[static_cast<int>(zOlder::FRONT)]->getChildByName("skillSprite");
	auto enemies = layer_[static_cast<int>(zOlder::CHAR_ENEMY)]->getChildren();

	// player数resize
	for (int p = 0; p < playerNum; p++)
	{
		auto plSprite = Player::CreatePlayer(100, *layer_[static_cast<int>(zOlder::CHAR_PL)],*layer_[static_cast<int>(zOlder::CHAR_ENEMY)],skillBasePtr);
		plSprite->setName("player" + std::to_string(p + 1));
		plSprite->setScale(3.0f);
		plSprite->setAnchorPoint(Vec2(0.5f, 0.0f));

		// プレイヤーをキャラクター用のレイヤーの子供にする
		layer_[static_cast<int>(zOlder::CHAR_PL)]
			->addChild(plSprite, playerNum);
		plSprite->scheduleUpdate();
	}
}