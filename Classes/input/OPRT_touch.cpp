#include "_Debug/_DebugConOut.h"
#include "scene/GameScene.h"
#include "OPRT_touch.h"

USING_NS_CC;

OPRT_touch::OPRT_touch(Sprite* sp)
{
	ResetKey();

	// UPからGameExitまでを登録(ボタン名のテーブル)
	const std::array<std::string, static_cast<int>(BUTTON::MAX)> nameTable{
		"jumpBtn"    ,"attackBtn","dashBtn",
		"skillABtn"  ,"skillBBtn","skillCBtn","menuBtn",
		"continueBtn","mapBtn"   ,"guideBtn" ,"exitBtn","cancelBtn"
	};

	for (auto key = static_cast<int>(BUTTON::UP); key < static_cast<int>(BUTTON::Non); key++)
	{
		map_.emplace(nameTable[key - static_cast<int>(BUTTON::UP)], static_cast<BUTTON>(key));
	}

	// Scene名とボタンを一致させる
	for (int num = 0; num < static_cast<int>(BUTTON::Non); num++)
	{
		if (num <= static_cast<int>(BUTTON::Menu))
		{
			std::vector<std::string> vec{ "GameScene" };
			sceneBtn_.emplace(static_cast<BUTTON>(num), vec);
		}
		else if (num <= static_cast<int>(BUTTON::GameExit))
		{
			std::vector<std::string> vec{ "PoseMenu" };
			sceneBtn_.emplace(static_cast<BUTTON>(num), vec);
		}
		else
		{
			std::vector<std::string> vec{ "MapMenu","Guide" };
			sceneBtn_.emplace(static_cast<BUTTON>(num), vec);
		}
	}

	trStr_ = "_toDark";

	SetupTouch(sp);
}

OPRT_touch::~OPRT_touch()
{
}

OPRT_TYPE OPRT_touch::GetType(void)
{
	return OPRT_TYPE::TOUCH;
}

void OPRT_touch::update()
{
	keyData_.oldData_ = keyData_.data_;
	keyData_.data_ = keyData_.input_;
}

void OPRT_touch::StartSwipe(cocos2d::Touch * touch)
{
	swipeStartPoint = Point(touchVectors[touch->getID()].pos.x, touchVectors[touch->getID()].pos.y);
	swipeEndPoint   = Point(touchVectors[touch->getID()].pos.x, touchVectors[touch->getID()].pos.y);
	swipeRotate = SWIPE_CENTER;
}

void OPRT_touch::MoveSwipe(cocos2d::Touch * touch)
{
	if (!touchVectors[touch->getID()].isMoveTouch)
	{
		return;
	}
	const auto startOffset = 20.0f;
	const auto loc = touch->getLocation();
	const auto start = touchVectors[touch->getID()].pos;

	if (loc.x < start.x - startOffset) 
	{
		swipeRotate = SWIPE_LEFT;		// left
	}
	else 
	{
		if (loc.x > start.x + startOffset)
		{
			swipeRotate = SWIPE_RIGHT;  // right
		}
	}
	if (swipeRotate == SWIPE_CENTER ||
	   (swipeRotate != SWIPE_CENTER && std::abs(loc.x - start.x) < std::abs(loc.y - start.y)))
	{
		if (loc.y < start.y - startOffset) 
		{
			swipeRotate = SWIPE_DOWN;	// down
		}
		if (loc.y > start.y + startOffset)
		{
			swipeRotate = SWIPE_UP;		// up
		}
	}

	switch (swipeRotate)
	{
	case SWIPE_UP:
		// 何も処理を行わない
		break;
	case SWIPE_DOWN:
		keyData_.input_[static_cast<int>(BUTTON::UP)] = false;
		break;
	case SWIPE_LEFT:
		keyData_.input_[static_cast<int>(BUTTON::LEFT)]  = true;
		keyData_.input_[static_cast<int>(BUTTON::RIGHT)] = false;
		break;
	case SWIPE_RIGHT:
		keyData_.input_[static_cast<int>(BUTTON::RIGHT)] = true;
		keyData_.input_[static_cast<int>(BUTTON::LEFT)]  = false;
		break;
	default:
		break;
	}
}

void OPRT_touch::EndSwipe(cocos2d::Touch * touch)
{
	// ボタン非押下状態の画像に戻すために使用
	auto buttonLambda = [&](std::string str, BUTTON button) {
		cocos2d::Node* buttonName = SetBtnNameInfo(str, button);

		if (buttonName != nullptr)
		{
			keyData_.input_[static_cast<int>(button)] = false;
			dynamic_cast<MenuItemImage*>(buttonName)->unselected();
		}
	};

	for (auto key = static_cast<int>(BUTTON::RIGHT); key < static_cast<int>(BUTTON::MAX); key++)
	{
		if (touchVectors[touch->getID()].isMoveTouch)
		{
			keyData_.input_[key] = false;
		}
		else
		{
			for (auto button : map_)
			{
				buttonLambda(button.first, button.second);
			}
			buttonLambda("transformBtn" + trStr_, BUTTON::Transfrom);
		}
	}
}

void OPRT_touch::SetupTouch(cocos2d::Sprite * delta)
{
	Touches touches = Touches();
	touchVectors.clear();
	for (int i = 0; i < MULTI_TOUCH_MAX; i++)
	{
		touchVectors.emplace_back(touches);
	}
	moveFlag = false;
	auto listener = EventListenerTouchAllAtOnce::create();

	listener->onTouchesBegan = [&](std::vector<Touch*> touches, Event * event)
	{
		for (auto touch : touches)
		{
			touchVectors[touch->getID()].pos = touch->getLocation();
			auto runningScene = Director::getInstance()->getRunningScene();

			auto buttonLambda = [&](std::string str, BUTTON button) {
				cocos2d::Node* buttonName = SetBtnNameInfo(str, button);

				if (buttonName == nullptr)
				{
					return false;
				}

				if (buttonName->getBoundingBox().containsPoint(touchVectors[touch->getID()].pos))
				{
					// buttonNameがクリック/タッチされた場合の処理
					keyData_.input_[static_cast<int>(button)] = true;
					// ボタン押下状態の画像に変更
					dynamic_cast<MenuItemImage*>(buttonName)->selected();

					// strがtransfrom関係だったら、visibleを入れ替える
					if (str == "transformBtn" + trStr_)
					{
						trStr_ = (trStr_ == "_toDark" ? "_toLight" : "_toDark");
						dynamic_cast<MenuItemImage*>(buttonName)->setVisible(false);
						auto toLight = runningScene->getChildByTag(static_cast<int>(zOrder::FRONT))->getChildByName("transformBtn" + trStr_);
						dynamic_cast<MenuItemImage*>(toLight)->setVisible(true);
					}
					return true;
				}
			};

			// バーチャルパッドが反応しても良いかを確認する処理
			bool buttonFlg = false;
			for (auto button : map_)
			{
				buttonFlg |= buttonLambda(button.first, button.second);
			}

			if (runningScene->getName() != "GameScene")
			{
				buttonFlg = true;
			}

			if (!buttonFlg && !buttonLambda("transformBtn" + trStr_, BUTTON::Transfrom))
			{
				StartSwipe(touch);
				if (!moveFlag)
				{
					auto startSp = runningScene->getChildByTag(static_cast<int>(zOrder::FRONT))->getChildByName("startSp");
					startSp->setPosition(touchVectors[touch->getID()].pos);
					moveFlag = true;
					touchVectors[touch->getID()].isMoveTouch = true;
				}
			}
		}
		return true;
	};

	listener->onTouchesMoved = [&](std::vector<Touch*> touches, Event * event)
	{
		if (cocos2d::Director::getInstance()->getRunningScene()->getName() != "GameScene")
		{
			return false;
		}

		for (auto touch : touches) 
		{
			MoveSwipe(touch);
		}
		return true;
	};

	listener->onTouchesEnded = [this](std::vector<Touch*> touches, Event * event)
	{
		for (auto touch : touches)
		{
			EndSwipe(touch);
			if (touchVectors[touch->getID()].isMoveTouch)
			{
				touchVectors[touch->getID()].isMoveTouch = false;
				moveFlag = false;
				auto startSp = Director::getInstance()->getRunningScene()->getChildByTag(static_cast<int>(zOrder::FRONT))->getChildByName("startSp");
				startSp->setPosition(150.0f, 150.0f);
			}
		}
		return true;
	};

	delta->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, delta);
}

void OPRT_touch::ResetKey(void)
{
	for (auto key = static_cast<int>(BUTTON::RIGHT); key < static_cast<int>(BUTTON::MAX); key++)
	{
		keyData_.input_[key] = false;
		keyData_.data_[key]  = false;
		keyData_.oldData_[key] = false;
	}
}

cocos2d::Node* OPRT_touch::SetBtnNameInfo(std::string str, BUTTON button)
{
	auto runningScene = Director::getInstance()->getRunningScene();
	cocos2d::Node* buttonName = nullptr;

	for (auto data : sceneBtn_[button])
	{
		if (runningScene->getName() != data)
		{
			continue;
		}
		buttonName = runningScene->getChildByName(str);
		if (buttonName == nullptr)
		{
			buttonName = runningScene->getChildByTag(static_cast<int>(zOrder::FRONT))->getChildByName(str);
		}
	}
	return buttonName;
}

const std::array<bool, static_cast<int>(BUTTON::MAX)>& OPRT_touch::GetNowData(void)
{
	return keyData_.data_;
}

const std::array<bool, static_cast<int>(BUTTON::MAX)>& OPRT_touch::GetOldData(void)
{
	return keyData_.oldData_;
}

// Vec2とかの( )と{ }の差について
// 基本的な初期化方法は( )を利用している
// 複数の初期化とかは{ }の初期化リストを利用する
// また、Vec2 test = Vec2(xxx,yyy)と書かないといけないぐらいなら、Vec2 test(xxx,yyy)のほうが冗長にならない
// 参考サイト→https://qiita.com/h2suzuki/items/d033679afde821d04af8