#include "GameMap.h"
#include "_Debug/_DebugConOut.h"
#include "Scene/LoadScene.h"
#include "obj/Player.h"
#include "Gate.h"
#include "ClearObj.h"
#include "Generator/MapGenerator.h"
#include "Generator/MST.h"
#include "anim/AnimMng.h"

USING_NS_CC;
#pragma execution_character_set("utf-8")

GameMap::GameMap(cocos2d::Layer& layer)
{
	// ���[�v�̃A�j���[�V�����o�^
	lpAnimMng.addAnimationCache("image/Sprites/map", "warp", 5, 0.25f, true);
	std::random_device engine;
	mapGenerator_ = std::make_shared<MapGenerator>(engine());
	mapGenerator_->Call();
	const auto nodeData = mapGenerator_->GetMSTNode();
	mapLayer_ = &layer;
	objLayer_ = Layer::create();
	
	isChangeFloor_ = false;
	isClear_ = false;
	
	layer.addChild(objLayer_,layer.getLocalZOrder() + 1);	

	// �p�X�����X�g��
	pathList_.push_back("image/Environment/tutorialMap.tmx");
	pathList_.push_back("image/Environment/trueMap.tmx");
	
	nextPosTbl = {
		Vec2(100.0f, 100.0f),	// E_UP
		Vec2(300.0f, 100.0f),	// N_RIGHT
		Vec2(700.0f, 100.0f),	// N_LEFT
		Vec2(900.0f, 100.0f),	// W_UP
		Vec2(900.0f, 500.0f),	// W_DOWN
		Vec2(700.0f, 500.0f),	// S_LEFT
		Vec2(300.0f, 500.0f),	// S_RIGHT
		Vec2(700.0f, 500.0f),	// E_DOWN 
	};
	
	// ���������}�b�v�f�[�^�쐬(�f�[�^��)
	AddMap(pathList_);
	MapChild mapChild;
	for (auto& node : nodeData)
	{
		MapParentState mapParent;
		mapParent.mapID = 1;
		mapParent.enemyNum = RandomHelper::random_int(3, 7);
		mapParent.isArrival = false;
		for (auto& child : node.childData)
		{
			mapChild.mapID = 1;
			mapChild.nextParentID = child.childId;

			mapChild.gateDir = child.dir;
			mapChild.nextPos = nextPosTbl[static_cast<int>(mapChild.gateDir)];

			mapParent.child.push_back(mapChild);
		}
		mapParentsList_.mapParents.push_back(mapParent);
	}

	// �N���A������̂��镔��
	mapParentsList_.clearMapID = random(0, 49);

	// �`���[�g���A���}�b�v�f�[�^�쐬
	// �C���f�b�N�X���������������}�b�v�̌��ɂ��Ă���
	MapParentState mapParent;
	mapParent.mapID = 0;
	mapParent.enemyNum = 0;
	mapParent.isArrival = false;
	
	mapChild.mapID = 1;
	mapChild.nextParentID = 0;

	mapChild.gateDir = MapDirection::E_Down;
	const auto visibleSize = Director::getInstance()->getVisibleSize();
	const Vec2 origin = Director::getInstance()->getVisibleOrigin();
	mapChild.nextPos  = { visibleSize.width / 2 + origin.x - 0,visibleSize.height / 2 + origin.y + 200 };
	mapParent.child.push_back(mapChild);
	mapParentsList_.mapParents.push_back(mapParent);

	// ID���`���[�g���A����p�̃}�b�v�ԍ��ɂ��Ă���
	mapParentsList_.nowID = mapParentsList_.mapParents.size() - 1;

	// �`���[�g���A���}�b�v�̃I�u�W�F�N�g����鏈��
	CreateObject();
	// �`���[�g���A���}�b�v�̈ȊO�������Ȃ����鏈��
	for (auto data : mapDatas_)
	{
		if (mapDatas_[mapParentsList_.mapParents[mapParentsList_.nowID].mapID] == data)
		{
			continue;
		}
		data->setVisible(false);
	}

	frame_ = 0;
	
}

GameMap::~GameMap()
{
	mapParentsList_.mapParents.clear();
	for (auto& mapData : mapDatas_)
	{
		mapData->release();
	}
}

void GameMap::AddMap(std::vector<std::string>& mapPaths)
{
	for (auto& mapPath : mapPaths)
	{
		auto mapArray = createMapFromPath(mapPath);
		mapDatas_.push_back(mapArray);
	}
}

void GameMap::LoadMap(Player& player)
{
	const auto fade = TransitionFade::create(1.0f, LoadScene::CreateLoadScene(player, *this), Color3B::BLACK);
	Director::getInstance()->pushScene(fade);
}

void GameMap::ReplaceMap(Player& player)
{
	const int childId = nextId_;
	const auto mapState_ = mapParentsList_.mapParents[mapParentsList_.nowID];
	const auto selectNextMap = mapDatas_[static_cast<int>(mapState_.child[childId].mapID)];
	const auto nowMap = GetNowMap();

	if (nowMap != selectNextMap)
	{
		selectNextMap->setVisible(true);
		selectNextMap->setName("MapData");
		nowMap->setVisible(false);
		nowMap->setName("");
	}

	mapParentsList_.nowID = static_cast<int>(mapState_.child[childId].nextParentID);
	mapParentsList_.mapParents[mapParentsList_.nowID].isArrival = true;

	// �I�u�W�F�N�g�쐬
	CreateObject();

	// �v���C���[�|�W�V�����Z�b�g
	player.setPosition(mapState_.child[childId].nextPos);
}

void GameMap::CreateObject()
{
	const auto objGroups = GetNowMap()->getObjectGroups();
	// �I�u�W�F�N�g�����ɂ�������폜����
	if (objs_.size() > 0)
	{
		for (auto obj : objs_)
		{
			obj->removeFromParent();
		}
		objs_.clear();
	}

	for (auto objGroup : objGroups)
	{
		if (objGroup == nullptr)
		{
			return;
		}
		const auto nowMapParent = mapParentsList_.mapParents[mapParentsList_.nowID];
		// TMXMap�̃I�u�W�F�N�g���C���[������擾
		const auto& objs = objGroup->getObjects();

		for (auto& obj : objs)
		{
			ValueMap prop = obj.asValueMap();
			const auto name = prop["name"].asString();
			const auto x = prop["x"].asFloat();
			const auto y = prop["y"].asFloat();
			const auto w = prop["width"].asInt();
			const auto h = prop["height"].asInt();

			// �Q�[�g��������
			if (name == "gate")
			{
				const auto gateNum = prop["gateNum"].asInt();
				for (unsigned int i = 0; i < nowMapParent.child.size(); ++i)
				{
					if (static_cast<int>(nowMapParent.child[i].gateDir) != gateNum)
					{
						continue;
					}
					const auto gate = Gate::CreateGate({ x, y }, i);
					gate->setCameraMask(static_cast<int>(CameraFlag::USER1));
					objs_.push_back(gate);
				}
			}
			// �N���A�I�u�W�F�N�g��������
			if (name == "clear")
			{
				if (mapParentsList_.clearMapID == mapParentsList_.nowID)
				{
					const auto clearObj = ClearObj::CreateClearObj({ x, y });
					clearObj->setCameraMask(static_cast<int>(CameraFlag::USER1));
					objs_.push_back(clearObj);
				}
			}
		}
	}

	// �I�u�W�F�N�g���Z�b�g
	for (auto obj : objs_)
	{
		objLayer_->addChild(obj);
	}
}

cocos2d::TMXTiledMap* GameMap::GetNowMap()
{
	const auto nowMapParent = mapParentsList_.mapParents[mapParentsList_.nowID];
	return mapDatas_[static_cast<int>(nowMapParent.mapID)];
}

void GameMap::Update(Player& player,int nowEnemyNum)
{
	// ��Ƀt���A�ύX�̃t���O��false��
	isChangeFloor_ = false;

	for (auto obj : objs_)
	{
		obj->Update(nowEnemyNum);
		
		if(obj->IsHit(player))
		{
			if (obj->getName() == "gate")
			{
				nextId_ = dynamic_cast<Gate*>(obj)->GetGateNum();
				// �v���C���[���Q�[�g�������������ɂ̂�true�ɂ���
				isChangeFloor_ = true;
				LoadMap(player);
			}
			if (obj->getName() == "clear")
			{
				isClear_ = true;
			}
		}
	}

	frame_++;
}

MapParentList& GameMap::GetMapParentList()
{
	return mapParentsList_;
}

const int GameMap::GetNowID()
{
	return mapParentsList_.nowID;
}

int GameMap::GetEnemyNum()
{
	return mapParentsList_.mapParents[mapParentsList_.nowID].enemyNum;
}

int GameMap::IsClear()
{
	return isClear_;
}

MapGenerator& GameMap::GetMapGenerator()
{
	return *mapGenerator_;
}

cocos2d::TMXTiledMap* GameMap::createMapFromPath(std::string& mapPath)
{
	TMXTiledMap* map = TMXTiledMap::create(mapPath);

	const auto colLayer = map->getLayer("Collision");
	const auto wallSlideLayer = map->getLayer("WallSlide");
	colLayer->setName("col");
	wallSlideLayer->setName("slide");
	mapLayer_->addChild(map);
	map->setName("MapData");

	cocos2d::TMXTiledMap* mapArray = map;
	return mapArray;
}

const bool GameMap::ChangeFloor()
{
	return isChangeFloor_;
}

void GameMap::ColisionDebugDraw(bool debug)
{
//#ifdef _DEBUG
//	if (mapParentsList_.mapParents.size() == 0)
//	{
//		return;
//	}
//	if (tex == nullptr)
//	{
//		auto director = Director::getInstance();
//		auto size = director->getVisibleSize();
//		tex = RenderTexture::create(size.width, size.height);
//		tex->setPosition(size.width / 2, size.height / 2);
//		mapLayer_->addChild(tex);
//		tex->begin();
//		auto nowMap = GetNowMap();
//		auto colLayer = nowMap->getLayer("Collision");
//		auto mapIdx = nowMap->getMapSize();
//		auto mapTileSize = colLayer->getMapTileSize();
//		for (int y = 0; y < mapIdx.height; ++y)
//		{
//			for (int x = 0; x < mapIdx.width; ++x)
//			{
//				if (colLayer->getTileGIDAt({ static_cast<float>(x),mapIdx.height - static_cast<float>(y + 1) }) != 0)
//				{
//					auto rect = cocos2d::Rect({ 0,0 }, mapTileSize);
//					Sprite* sprite = Sprite::create();
//					sprite->setTextureRect(rect);
//					sprite->setPosition(x * mapTileSize.width, y * mapTileSize.height);
//					sprite->setColor(Color3B(0.0f, 255.0f, 0.0f));
//					sprite->setOpacity(122.0f);
//					sprite->setAnchorPoint({ 0.0f,0.0f });
//					sprite->retain();
//					sprite->visit();
//				}
//			}
//		}
//		tex->end();
//	}
//	else
//	{
//		tex->setVisible(debug);
//	}
//#endif
}