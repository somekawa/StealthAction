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
#include "obj/Enemy/Enemy.h"
#include "PL_HPgauge.h"
#include "Loader/CollisionLoader.h"
#include "GameMap.h"
#include "renderer/backend/Device.h"
#include "SoundMng.h"
#include "ENemyHPGauge.h"
#include "Effect/EffectManager.h"
#include "Skill/SkillBase.h"
#include "MiniMap.h"

USING_NS_CC;

namespace
{
	backend::ProgramState* programState;
	backend::Program* program;
}

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

	// �T�E���h
	lpSoundMng.AddSound("bgm", "BGM/BGM_piano.mp3", SoundType::BGM);
	lpSoundMng.PlayBySoundName("bgm");

	// �e�G�L������AINode�̐���----------------------------------------------------------
	
	//CharaInfoRegistrator(ActorType::Cultist);
	//CharaInfoRegistrator(ActorType::Fireball);
	//-------------------------------------------------------------------------------
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
	// player�p���C���[
	layer_[(int)zOlder::CHAR_PL]->setTag((int)zOlder::CHAR_PL);
	layer_[(int)zOlder::CHAR_PL]->setGlobalZOrder((int)zOlder::CHAR_PL);
	// enemy�p���C���[
	layer_[(int)zOlder::CHAR_ENEMY]->setTag((int)zOlder::CHAR_ENEMY);
	layer_[(int)zOlder::CHAR_ENEMY]->setGlobalZOrder((int)zOlder::CHAR_ENEMY);
	// ui�p���C���[
	layer_[(int)zOlder::FRONT]->setTag((int)zOlder::FRONT);
	layer_[(int)zOlder::FRONT]->setGlobalZOrder((int)zOlder::FRONT);
	// effect�pڲ԰
	layer_[(int)zOlder::EFFECT]->setTag((int)zOlder::EFFECT);
	layer_[(int)zOlder::EFFECT]->setGlobalZOrder((int)zOlder::EFFECT);
	// bg�p���C���[
	layer_[(int)zOlder::BG]->setTag((int)zOlder::BG);
	layer_[(int)zOlder::BG]->setGlobalZOrder((int)zOlder::BG);

	// playerList_��player�ǉ�
	// �����ɐڑ����Ă���v���C���[�����L�q
	AddPlayer(1);
	// enemyList_�ɓG�ǉ�
	//AddEnemy(ActorType::Imp);
	// assassin��ǉ�

	//AddEnemy(ActorType::Assassin);
	//AddEnemy(ActorType::TwistedCultist);
	//AddEnemy(ActorType::Assassin);

	// �G�𐶐�������(��������Ȃ�)
	// �G��setName���邽�߂ɗp�ӂ�������
	generateEnemyNum_ = 0;
	// �G�����񂾂�Ƃ肠�������X�|�[������t���O
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

	// Scene - Layer - Sprite	// bgLayer��GameScene�ɒ��ڂԂ牺����
	//this->addChild(bgLayer, (int)zOlder::BG);

	// �w�i�̓o�^(bgLayer�ɂԂ牺����bgSprite)
	auto bgSprite1 = Sprite::create("image/Environment/background-big.png");
	bgSprite1->setPosition(Vec2(0, visibleSize.height / 2));
	layer_[(int)zOlder::BG]->addChild(bgSprite1, -2);

	auto bgSprite2 = Sprite::create("image/Environment/background-big.png");
	bgSprite2->setPosition(Vec2(720, visibleSize.height / 2));
	layer_[(int)zOlder::BG]->addChild(bgSprite2, -1);

	/*auto bgMiddle = Sprite::create("image/Environment/middleground-big.png");
	bgMiddle->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	layer_[(int)zOlder::BG]->addChild(bgMiddle, 0);*/

	// �{�^���e�X�g�p
	// �U���{�^��
	
	auto attackBtnSp = Sprite::create("image/button1.png");
	attackBtnSp->setName("attackBtn");
	attackBtnSp->setAnchorPoint(Vec2(0.5f, 0.0f));
	float x = origin.x + visibleSize.width - attackBtnSp->getContentSize().width / 2;
	float y = origin.y + attackBtnSp->getContentSize().height / 2;
	attackBtnSp->setPosition(Vec2(x, y));
	layer_[(int)zOlder::FRONT]->addChild(attackBtnSp, 0);
	// �W�����v�{�^��
	auto jumpBtnSp = Sprite::create("image/button2.png");
	jumpBtnSp->setName("jumpBtn");
	jumpBtnSp->setAnchorPoint(Vec2(0.5f, 0.0f));
	jumpBtnSp->setPosition(Vec2(x - jumpBtnSp->getContentSize().width * 2, y));
	layer_[(int)zOlder::FRONT]->addChild(jumpBtnSp, 0);

	// HP�Q�[�W�`��p
	auto PL_HPgaugeSp = PL_HPgauge::createPL_HPgauge();
	layer_[(int)zOlder::FRONT]->addChild(PL_HPgaugeSp, 0);
	PL_HPgaugeSp->setName("PL_HPgauge");
	PL_HPgaugeSp->setPosition(visibleSize.width / 10, visibleSize.height - visibleSize.height / 10);

	// �ړ��o�[�`�����p�b�h�p
	auto startSp = Sprite::create("CloseNormal.png");
	startSp->setName("startSp");
	startSp->setPosition(Vec2(150.0f,150.0f ));
	layer_[(int)zOlder::FRONT]->addChild(startSp, 1);

	// map�ǂݍ���
	gameMap_ = std::make_shared<GameMap>(*layer_[(int)zOlder::BG]);

	// �L�����̓o�^(charLayer��GameScene�ɒ��ڂԂ牺����AplSprite��charLayer�ɂԂ牺����)

	//auto plSprite = Player::createPlayer();
	//plSprite = Player::createPlayer();
	//charLayer->addChild((Node*)plSprite, (int)zOlder::CHAR_PL);
	//plSprite->setName("player");
	//// �L�����T�C�Y3�{
	//plSprite->setScale(3.0f);
	//// �A���J�[�|�C���g������
	//plSprite->setAnchorPoint(Vec2(0.5f, 0.0f));

	//layer_[static_cast<int>(zOlder::CHAR_PL)]->getChildByName("player1")->setScale(3.0f);
	//layer_[static_cast<int>(zOlder::CHAR_PL)]->getChildByName("player1")->setAnchorPoint(Vec2(0.5f, 0.0f));
	/*�O�ʂɏo���������C���[�̓o�^*/
	//auto frLayer = Layer::create();
	// auto frSprite = Sprite::create(" ");
	// plSprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	// frLayer->addChild(frSprite,0);
	//this->addChild(frLayer, (int)zOlder::FRONT);	

	//Sprite* sp;
	//sp->setName("aa");
	//_DebugDispOutCC::GetInstance().DrawRect("aa", Vec2(0, 0), Vec2(100, 100), Vec2(200, 200), Color4F::BLUE);
	
	// �J�����쐬
	cameraManager_ = std::make_shared<CameraManager>();
	
	cameraManager_->AddCamera(*this, visibleSize,CameraType::PLAYER1, CameraFlag::USER1);
	cameraManager_->AddCamera(*this, visibleSize, CameraType::UI, CameraFlag::USER2);

	layer_[static_cast<int>(zOlder::CHAR_PL)]->setCameraMask(static_cast<int>(CameraFlag::USER1));
	layer_[static_cast<int>(zOlder::CHAR_ENEMY)]->setCameraMask(static_cast<int>(CameraFlag::USER1));

	//charLayer->setCameraMask(static_cast<int>(CameraFlag::USER1));
	layer_[static_cast<int>(zOlder::BG)]->setCameraMask(static_cast<int>(CameraFlag::USER1));
	layer_[(int)zOlder::FRONT]->setCameraMask(static_cast<int>(CameraFlag::USER2));

	//plSprite->scheduleUpdate();
	// �v���C���[�̍X�V
	//layer_[static_cast<int>(zOlder::CHAR_PL)]->getChildByName("player1")->scheduleUpdate();

	PL_HPgaugeSp->scheduleUpdate();

	auto fileUtiles = FileUtils::getInstance();
	auto vertexSource = fileUtiles->getStringFromFile("OutLineTest.vert");
	auto fragmentSource = fileUtiles->getStringFromFile("OutLineTest.frag");
	program = backend::Device::getInstance()->newProgram(vertexSource.c_str(), fragmentSource.c_str());
	programState = new backend::ProgramState(program);

	auto player = (Player*)layer_[static_cast<int>(zOlder::CHAR_PL)]->getChildByName("player1");
	// �G�̏o������������Ǘ�����Manager�𐶐�
	enemyManager_ = std::make_unique<EnemyManager>(*layer_[static_cast<int>(zOlder::CHAR_ENEMY)], *layer_[static_cast<int>(zOlder::FRONT)], *player);

	// �G�̱�Ұ��݊֌W�A��Ͳ�ޱ�̏�����
	enemyManager_->Initialize();
	enemyManager_->CreateInitialEnemyOnFloor(3);
	//enemyManager_->CreateBoss(effectManager_);

	skillSprite = SkillBase::createSkillBase();
	layer_[(int)zOlder::FRONT]->addChild(skillSprite, 0);
	skillSprite->setName("skillSprite");
	skillSprite->setPosition(0,0);

	// �̪�ėp��޼ު���ٍ߰쐬
	lpEffectMng.CreatePools(*layer_[static_cast<int>(zOlder::EFFECT)]);

	// �~�j�}�b�v
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	auto listener = cocos2d::EventListenerKeyboard::create();
	listener->onKeyPressed = [this](cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* keyEvent)
	{
		if (keyCode == EventKeyboard::KeyCode::KEY_M)
		{
			Director::getInstance()->pushScene(MiniMap::CreateMiniMap(gameMap_->GetMapGenerator(), gameMap_->GetNowID()));
		}
	};
#else
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [this](cocos2d::Touch* touch, cocos2d::Event* event)
	{
		return true;
	};
#endif // (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
	this->scheduleUpdate();
	return true;
}

void Game::update(float sp)
{
	// ����Scene��GameScene�ȊO�������ꍇ�Areturn������
	if (Director::getInstance()->getRunningScene()->getName() != "GameScene")
	{
		return;
	}

	// SkillBase�Ăԃe�X�g
	auto skillBaseSp = (SkillBase*)layer_[static_cast<int>(zOlder::FRONT)]->getChildByName("skillSprite");
	skillBaseSp->UpDate();

	auto player = (Player*)layer_[static_cast<int>(zOlder::CHAR_PL)]->getChildByName("player1");
	gameMap_->update(*player);

	if (gameMap_->ChangeFloor())
	{
		
		// �G�̐�������ؾ��(enemyNum_��0�ɂ���)
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

		// �۱�ύX��1�񂾂������̓G�̐������ݽ�ݽ
		enemyManager_->CreateInitialEnemyOnFloor(1);
	}
	auto enemyNum = layer_[static_cast<int>(zOlder::CHAR_ENEMY)]->getChildrenCount();

	// �G�̽�݂߰��Ǘ�
	//enemyManager_->Update(effectManager_);
	
	// �v���C���[�̃J���������܂������Ȃ�
	//cameraManager_->ScrollCamera(plSprite->getPosition(), CameraType::PLAYER1);
	cameraManager_->ScrollCamera(layer_[static_cast<int>(zOlder::CHAR_PL)]->getChildByName("player1")->getPosition(), CameraType::PLAYER1);
	
	auto c = layer_[static_cast<int>(zOlder::CHAR_ENEMY)]->getChildren();
	for (auto e : c)
	{
		
		//auto program2 = backend::Device::getInstance()->newProgram(vertexSource.c_str(), fragmentSource.c_str());
		auto programState2 = new backend::ProgramState(program);
		e->setProgramState(programState2);
		programState2->release();
	}
	player->setProgramState(programState);
	//// �����蔻��p�̘g���o���Ă݂�
	//auto ppos = plSprite->getPosition();
	//auto psize = plSprite->getContentSize();
	// //�E�̎���offset+  ����offset-
	// //�E�����̎�
	//auto offset = Vec2(psize.width * 3.0f, 0.0f);
	//auto leftPoint = Vec2(ppos.x - (psize.width + psize.width / 2), ppos.y + psize.width) + offset;
	//auto rightPoint = Vec2(ppos.x - psize.width / 2, ppos.y) + offset;
	// //�������̎�
	////auto rightPoint = Vec2(ppos.x + psize.width + psize.width / 2, ppos.y + psize.width);
	////auto leftPoint = Vec2(ppos.x + psize.width / 2, ppos.y);

	////TRACE("%f\n", leftPoint.x + offset.x);

	//auto rect = DrawNode::create();
	// //�l�p����
	//this->removeChildByName("box");
	//rect->setName("box");
	// //drawRect(�J�n�_�̍��W�A�I���_�̍��W, �g���̐F)
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
	auto skillBasePtr = (SkillBase*)layer_[static_cast<int>(zOlder::FRONT)]->getChildByName("skillSprite");

	// player��resize
	for (int p = 0; p < playerNum; p++)
	{
		auto plSprite = Player::CreatePlayer(100, *layer_[static_cast<int>(zOlder::CHAR_PL)], skillBasePtr);
		plSprite->setName("player" + std::to_string(p + 1));
		plSprite->setScale(3.0f);
		plSprite->setAnchorPoint(Vec2(0.5f, 0.0f));
		// �v���C���[���L�����N�^�[�p�̃��C���[�̎q���ɂ���
		layer_[static_cast<int>(zOlder::CHAR_PL)]
			->addChild(plSprite, playerNum);
		plSprite->scheduleUpdate();
	}
}
