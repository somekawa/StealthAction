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
#include "_Debug/_DebugDispOutCC.h"
#include "_Debug/_DebugConOut.h"
#include "CameraManager.h"
#include "obj/Player.h"
#include "obj/Enemy/Imp.h"
#include "obj/Enemy/Assassin.h"
#include "PL_HPgauge.h"
#include "Loader/CollisionLoader.h"
#include "GameMap.h"
#include "Gate.h"
USING_NS_CC;

Scene* Game::createScene()
{
	return Game::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in GameScene.cpp\n");
}

#if CK_PLATFORM_ANDROID
#ifdef __cplusplus
extern "C" {
#endif
	JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_initCricket(JNIEnv* env, jclass activity, jobject context) {
		CkConfig config(env, context);
		CkInit(&config);
		//CkBank * _bank = CkBank::newBank("SE/se.ckb");
		//CkSound * _music = CkSound::newBankSound(_bank, 0);
		CkSound* _music = CkSound::newStreamSound("BGM/pianoSound.cks");
		_music->setLoopCount(-1);
		_music->play();
	}
#ifdef __cplusplus
}
#endif
#endif

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

	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.
	// add a "close" icon to exit the progress. it's an autorelease object
	//auto closeItem = MenuItemImage::create(
	//	"CloseNormal.png",
	//	"CloseSelected.png",
	//	CC_CALLBACK_1(Game::menuCloseCallback, this));
	//if (closeItem == nullptr ||
	//	closeItem->getContentSize().width <= 0 ||
	//	closeItem->getContentSize().height <= 0)
	//{
	//	problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
	//}
	//else
	//{
	//	float x = origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
	//	float y = origin.y + closeItem->getContentSize().height / 2;
	//	closeItem->setPosition(Vec2(x, y));
	//}
	//// create menu, it's an autorelease object
	//auto menu = Menu::create(closeItem, NULL);
	//menu->setPosition(Vec2::ZERO);
	//this->addChild(menu, 1);
	/////////////////////////////
	// 3. add your codes below...
	// add a label shows "Hello World"
	// create and initialize a label

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
	// bg用レイヤー
	layer_[(int)zOlder::BG]->setTag((int)zOlder::BG);
	layer_[(int)zOlder::BG]->setGlobalZOrder((int)zOlder::BG);


	// colliderBoxのロード(一括で全てのcolliderをロードしておいた方がいいのでここに書く)
	//lpCol.Load(colliderBox_[static_cast<int>(ActorType::Player)], "Look_Intro", "player");
	//lpCol.Load(colliderBox_[static_cast<int>(ActorType::Player)], "Run", "player");
	//lpCol.Load(colliderBox_[static_cast<int>(ActorType::Player)], "Fall", "player");
	//lpCol.Load(colliderBox_[static_cast<int>(ActorType::Player)], "Jump", "player");
	//lpCol.Load(colliderBox_[static_cast<int>(ActorType::Player)], "AttackA", "player");

	// playerList_にplayer追加
	// 引数に接続しているプレイヤー数を記述
	AddPlayer(1);
	// enemyList_に敵追加
	//AddEnemy(ActorType::Imp);
	// assassinを追加
	AddEnemy(ActorType::Assassin);
	// 敵を生成した回数(多分いらない)
	// 敵のsetNameするために用意したもの
	generateEnemyNum_ = 0;
	// 敵が死んだらとりあえずリスポーンするフラグ
	respawnFlag_ = false;

	auto label = Label::createWithTTF("Action", "fonts/Marker Felt.ttf", 24);
	if (label == nullptr)
	{
		problemLoading("'fonts/Marker Felt.ttf'");
	}
	else
	{
		label->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y + 250));
		this->addChild(label, 1);
	}

	this->setName("GameScene");

	// Scene - Layer - Sprite	// bgLayerはGameSceneに直接ぶら下がる
	//this->addChild(bgLayer, (int)zOlder::BG);

	// 背景の登録(bgLayerにぶら下がるbgSprite)
	auto bgSprite1 = Sprite::create("image/Environment/background-big.png");
	bgSprite1->setPosition(Vec2(0, visibleSize.height / 2));
	layer_[(int)zOlder::BG]->addChild(bgSprite1, -2);

	auto bgSprite2 = Sprite::create("image/Environment/background-big.png");
	bgSprite2->setPosition(Vec2(720, visibleSize.height / 2));
	layer_[(int)zOlder::BG]->addChild(bgSprite2, -1);

	auto bgMiddle = Sprite::create("image/Environment/middleground-big.png");
	bgMiddle->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	layer_[(int)zOlder::BG]->addChild(bgMiddle, 0);

	// ボタンテスト用
	// 攻撃ボタン
	auto attackBtnSp = Sprite::create("image/button1.png");
	attackBtnSp->setName("attackBtn");
	attackBtnSp->setAnchorPoint(Vec2(0.5f, 0.0f));
	float x = origin.x + visibleSize.width - attackBtnSp->getContentSize().width / 2;
	float y = origin.y + attackBtnSp->getContentSize().height / 2;
	attackBtnSp->setPosition(Vec2(x, y));
	layer_[(int)zOlder::FRONT]->addChild(attackBtnSp, 0);
	// ジャンプボタン
	auto jumpBtnSp = Sprite::create("image/button2.png");
	jumpBtnSp->setName("jumpBtn");
	jumpBtnSp->setAnchorPoint(Vec2(0.5f, 0.0f));
	jumpBtnSp->setPosition(Vec2(x - jumpBtnSp->getContentSize().width * 2, y));
	layer_[(int)zOlder::FRONT]->addChild(jumpBtnSp, 0);

	// HPゲージ描画用
	auto PL_HPgaugeSp = PL_HPgauge::createPL_HPgauge();
	layer_[(int)zOlder::FRONT]->addChild(PL_HPgaugeSp, 0);
	PL_HPgaugeSp->setName("PL_HPgauge");
	PL_HPgaugeSp->setPosition(visibleSize.width / 10, visibleSize.height - visibleSize.height / 10);

	// 移動バーチャルパッド用
	auto startSp = Sprite::create("CloseNormal.png");
	startSp->setName("startSp");
	startSp->setPosition(Vec2(150.0f,150.0f ));
	layer_[(int)zOlder::FRONT]->addChild(startSp, 1);

	// map読み込み
	// つながっているマップも読む
	// collisionLayerの取得
	gameMap_ = std::make_shared<GameMap>(*layer_[(int)zOlder::BG]);
	/*gameMap_->CreateMap("image/Environment/test.tmx", "image/Environment/uratest.tmx");
	gameMap_->AddNextMap("image/Environment/map2.tmx", "image/Environment/uratest.tmx");*/
	//gameMap_->CreateMap(*layer_[(int)zOlder::BG], "image/Environment/map2.tmx", "image/Environment/uratest.tmx");

	// キャラの登録(charLayerはGameSceneに直接ぶら下がり、plSpriteはcharLayerにぶら下がる)

	//auto plSprite = Player::createPlayer();
	//plSprite = Player::createPlayer();
	//charLayer->addChild((Node*)plSprite, (int)zOlder::CHAR_PL);
	//plSprite->setName("player");
	//// キャラサイズ3倍
	//plSprite->setScale(3.0f);
	//// アンカーポイント下中央
	//plSprite->setAnchorPoint(Vec2(0.5f, 0.0f));

	//layer_[static_cast<int>(zOlder::CHAR_PL)]->getChildByName("player1")->setScale(3.0f);
	//layer_[static_cast<int>(zOlder::CHAR_PL)]->getChildByName("player1")->setAnchorPoint(Vec2(0.5f, 0.0f));
	/*前面に出したいレイヤーの登録*/
	//auto frLayer = Layer::create();
	// auto frSprite = Sprite::create(" ");
	// plSprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	// frLayer->addChild(frSprite,0);
	//this->addChild(frLayer, (int)zOlder::FRONT);	

	//Sprite* sp;
	//sp->setName("aa");
	//_DebugDispOutCC::GetInstance().DrawRect("aa", Vec2(0, 0), Vec2(100, 100), Vec2(200, 200), Color4F::BLUE);
	
	// カメラ作成
	cameraManager_ = std::make_shared<CameraManager>();
	auto size = Director::getInstance()->getWinSize();
	cameraManager_->AddCamera(*this, size,CameraType::PLAYER1, CameraFlag::USER1);
	cameraManager_->AddCamera(*this, size, CameraType::UI, CameraFlag::USER2);

	layer_[static_cast<int>(zOlder::CHAR_PL)]->setCameraMask(static_cast<int>(CameraFlag::USER1));
	layer_[static_cast<int>(zOlder::CHAR_ENEMY)]->setCameraMask(static_cast<int>(CameraFlag::USER1));

	//charLayer->setCameraMask(static_cast<int>(CameraFlag::USER1));
	layer_[static_cast<int>(zOlder::BG)]->setCameraMask(static_cast<int>(CameraFlag::USER1));
	layer_[(int)zOlder::FRONT]->setCameraMask(static_cast<int>(CameraFlag::USER2));

	//plSprite->scheduleUpdate();
	// プレイヤーの更新
	layer_[static_cast<int>(zOlder::CHAR_PL)]->getChildByName("player1")->scheduleUpdate();

	PL_HPgaugeSp->scheduleUpdate();

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

	/*if (++frame  == 60)
	{
		gameMap_->SetMapInfo(MapType::URA);
	}
	if (frame == 120)
	{
		gameMap_->SetMapInfo(MapType::OMOTE);
		frame = 0;
	}*/
	auto player = (Player*)layer_[static_cast<int>(zOlder::CHAR_PL)]->getChildByName("player1");
	gameMap_->update(*player);
	
	// プレイヤーのカメラがうまくいかない
	//cameraManager_->ScrollCamera(plSprite->getPosition(), CameraType::PLAYER1);
	cameraManager_->ScrollCamera(layer_[static_cast<int>(zOlder::CHAR_PL)]->getChildByName("player1")->getPosition(), CameraType::PLAYER1);


	//// 当たり判定用の枠を出してみる
	//auto ppos = plSprite->getPosition();
	//auto psize = plSprite->getContentSize();
	// //右の時はoffset+  左はoffset-
	// //右向きの時
	//auto offset = Vec2(psize.width * 3.0f, 0.0f);
	//auto leftPoint = Vec2(ppos.x - (psize.width + psize.width / 2), ppos.y + psize.width) + offset;
	//auto rightPoint = Vec2(ppos.x - psize.width / 2, ppos.y) + offset;
	// //左向きの時
	////auto rightPoint = Vec2(ppos.x + psize.width + psize.width / 2, ppos.y + psize.width);
	////auto leftPoint = Vec2(ppos.x + psize.width / 2, ppos.y);

	////TRACE("%f\n", leftPoint.x + offset.x);

	//auto rect = DrawNode::create();
	// //四角消す
	//this->removeChildByName("box");
	//rect->setName("box");
	// //drawRect(開始点の座標、終了点の座標, 枠線の色)
	//rect->drawRect(leftPoint /*+ offset*/, rightPoint /*+ offset*/, Color4F::BLUE);
	//this->addChild(rect);
}

void Game::menuCloseCallback(Ref* pSender)
{
	//Close the cocos2d-x game scene and quit the application

	//Director::getInstance()->end();
	int a = 0;

	/*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

	//EventCustom customEndEvent("game_scene_close_event");
	//_eventDispatcher->dispatchEvent(&customEndEvent);
}

void Game::AddPlayer(int playerNum)
{
	// player数resize
	for (int p = 0; p < playerNum; p++)
	{
		auto plSprite = Player::CreatePlayer();
		plSprite->setName("player" + std::to_string(p + 1));
		plSprite->setScale(3.0f);
		plSprite->setAnchorPoint(Vec2(0.5f, 0.0f));
		// プレイヤーをキャラクター用のレイヤーの子供にする
		layer_[static_cast<int>(zOlder::CHAR_PL)]
			->addChild(plSprite, playerNum);
	}
}

void Game::AddEnemy(const ActorType& type)
{
	//enSprites_.emplace_front(std::make_shared<Enemy>(plSprites_));
	Enemy* sprite = nullptr;
	switch (type)
	{
		// Impを生成する場合
	case ActorType::Imp:
		// 敵の生成
		sprite = Imp::CreateImp(layer_[static_cast<int>(zOlder::CHAR_PL)]->getChildren());
		break;
	case ActorType::Assassin:
		sprite = Assassin::CreateAssassin(layer_[static_cast<int>(zOlder::CHAR_PL)]->getChildren());
		break;
	default:
		break;
	}

	// 敵に名前を付ける
	// 死んだら"death"となる
	sprite->setName("enemy");

	// 敵をActor用レイヤーの子供にする
	layer_[static_cast<int>(zOlder::CHAR_ENEMY)]
		->addChild(sprite, (int)zOlder::CHAR_ENEMY);
	generateEnemyNum_++;
	sprite->scheduleUpdate();
}
