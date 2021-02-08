#include "scene/GameScene.h"
#include "OPRT_touch.h"
#include "_Debug/_DebugConOut.h"

USING_NS_CC;

OPRT_touch::OPRT_touch(Sprite* delta)
{
	for (auto key = static_cast<int>(BUTTON::UP); key < static_cast<int>(BUTTON::MAX); key++)
	{
		_keyData._input[key] = false;
		_keyData._data[key] = false;
		_keyData._oldData[key] = false;
	}
	touchesflg(delta);
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
	_keyData._oldData = _keyData._data;
	_keyData._data = _keyData._input;
}

//void OPRT_touch::touchStart(cocos2d::Touch * touch)
//{
//	auto loc = touch->getLocation();
//	touchStartPoint = Point(loc.x, loc.y);
//	touchEndPoint = Point(loc.x, loc.y);
//	swipeRotate = SWIPE_CENTER;
//}
//
//void OPRT_touch::touchMove(cocos2d::Touch * touch)
//{
//	auto startPosOffset = 20;
//	auto loc = touch->getLocation();
//	auto start = touchStartPoint;
//	auto end = touchEndPoint;
//	if (loc.x < start.x - startPosOffset) {
//		swipeRotate = SWIPE_LEFT;		// left
//	}
//	else {
//		if (loc.x > start.x + startPosOffset) {
//			swipeRotate = SWIPE_RIGHT;  // right
//		}
//	}
//	if (swipeRotate == SWIPE_CENTER || (swipeRotate != SWIPE_CENTER && std::abs(loc.x - start.x) < std::abs(loc.y - start.y))) {
//		if (loc.y < start.y - startPosOffset) {
//			swipeRotate = SWIPE_DOWN;	// down
//		}
//		if (loc.y > start.y + startPosOffset) {
//			swipeRotate = SWIPE_UP;		// up
//		}
//	}
//
//	if (swipeRotate == SWIPE_UP) {
//		//_keyData._input[static_cast<int>(BUTTON::UP)] = true;
//	}
//	if (swipeRotate == SWIPE_DOWN) {
//		//_keyData._input[static_cast<int>(BUTTON::DOWN)] = true;
//	}
//	if (swipeRotate == SWIPE_LEFT) {
//		_keyData._input[static_cast<int>(BUTTON::LEFT)] = true;
//	}
//	if (swipeRotate == SWIPE_RIGHT) {
//		_keyData._input[static_cast<int>(BUTTON::RIGHT)] = true;
//	}
//}
//
//void OPRT_touch::touchEnd(cocos2d::Touch * touch)
//{
//	touchStartPoint = touchEndPoint = Point(-9999, -9999);
//	for (auto key = static_cast<int>(BUTTON::UP); key < static_cast<int>(BUTTON::MAX); key++)
//	{
//		_keyData._input[key] = false;
//	}
//}

void OPRT_touch::touchesStart(cocos2d::Touch* touch)
{
	touchStartPoint = Point(touchVectors[touch->getID()].pos.x, touchVectors[touch->getID()].pos.y);
	touchEndPoint = Point(touchVectors[touch->getID()].pos.x, touchVectors[touch->getID()].pos.y);
	swipeRotate = SWIPE_CENTER;	
}

void OPRT_touch::touchesMove(cocos2d::Touch* touch)
{
	if (!touchVectors[touch->getID()].isMoveTouch)
	{
		return;
	}
	auto startPosOffset = 20;
	auto loc = touch->getLocation();
	auto start = touchVectors[touch->getID()].pos;
	if (loc.x < start.x - startPosOffset) {
		swipeRotate = SWIPE_LEFT;		// left
	}
	else {
		if (loc.x > start.x + startPosOffset) {
			swipeRotate = SWIPE_RIGHT;  // right
		}
	}
	if (swipeRotate == SWIPE_CENTER || (swipeRotate != SWIPE_CENTER && std::abs(loc.x - start.x) < std::abs(loc.y - start.y))) {
		if (loc.y < start.y - startPosOffset) {
			swipeRotate = SWIPE_DOWN;	// down
		}
		if (loc.y > start.y + startPosOffset) {
			swipeRotate = SWIPE_UP;		// up
		}
	}

	if (swipeRotate == SWIPE_UP) {
		//_keyData._input[static_cast<int>(BUTTON::UP)] = true;
	}
	if (swipeRotate == SWIPE_DOWN) {
		//_keyData._input[static_cast<int>(BUTTON::DOWN)] = true;
		_keyData._input[static_cast<int>(BUTTON::UP)] = false;
	}
	if (swipeRotate == SWIPE_LEFT) {
		_keyData._input[static_cast<int>(BUTTON::LEFT)] = true;
		_keyData._input[static_cast<int>(BUTTON::RIGHT)] = false;
	}
	if (swipeRotate == SWIPE_RIGHT) {
		_keyData._input[static_cast<int>(BUTTON::RIGHT)] = true;
		_keyData._input[static_cast<int>(BUTTON::LEFT)] = false;
	}
}

void OPRT_touch::touchesEnd(cocos2d::Touch* touch)
{
	// ボタン非押下状態の画像に戻すために使用
	auto buttonLambda = [&](std::string str, BUTTON button) {
		auto director = Director::getInstance();
		auto label1 = director->getRunningScene()->getChildByTag((int)zOlder::FRONT)->getChildByName(str);
		_keyData._input[static_cast<int>(button)] = false;
		((MenuItemImage*)label1)->unselected();
	};

	for (auto key = static_cast<int>(BUTTON::UP); key < static_cast<int>(BUTTON::MAX); key++)
	{
		if (touchVectors[touch->getID()].isMoveTouch)
		{
			if (key != static_cast<int>(BUTTON::DOWN) && key != static_cast<int>(BUTTON::UP))
			{
				_keyData._input[key] = false;
			}
		}
		else
		{
			buttonLambda("attackBtn", BUTTON::DOWN);
			buttonLambda("jumpBtn", BUTTON::UP);
			buttonLambda("dashBtn", BUTTON::Dash);
			buttonLambda("transformBtn_toDark", BUTTON::Transfrom);
			buttonLambda("transformBtn_toLight", BUTTON::Transfrom);

			//_keyData._input[static_cast<int>(BUTTON::DOWN)] = false;
			//_keyData._input[static_cast<int>(BUTTON::UP)] = false;
		}
	}
}

void OPRT_touch::touchesflg(cocos2d::Sprite* delta)
{
	Touches touches = Touches();
	touchVectors.clear();
	// とりあえず5本
	for (int i = 0; i < 5; i++)
	{
		touchVectors.emplace_back(touches);
	}
	moveFlag = false;
	auto listener = EventListenerTouchAllAtOnce::create();
	listener->onTouchesBegan = [&](std::vector<Touch*> touches, Event* event)
	{
		for (auto touch : touches)
		{
			touchVectors[touch->getID()].pos = touch->getLocation();
			auto director = Director::getInstance();

			auto buttonLambda = [&](std::string str,BUTTON button) {
				auto label1 = director->getRunningScene()->getChildByTag((int)zOlder::FRONT)->getChildByName(str);
				auto r = label1->getBoundingBox();
				if (r.containsPoint(touchVectors[touch->getID()].pos)) {
					// label1がクリック/タッチされた場合の処理
					_keyData._input[static_cast<int>(button)] = true;
					// ボタン押下状態の画像に変更
					((MenuItemImage*)label1)->selected();

					// strがtransfrom関係だったら、visibleを入れ替える
					if (str == "transformBtn" + trStr)
					{
						trStr == "_toDark" ? trStr = "_toLight" : trStr = "_toDark";
						((MenuItemImage*)label1)->setVisible(false);
						auto toLight = director->getRunningScene()->getChildByTag((int)zOlder::FRONT)->getChildByName("transformBtn"+trStr);
						((MenuItemImage*)toLight)->setVisible(true);
					}

					return true;
				}
				return false;
			};

			if (!buttonLambda("attackBtn", BUTTON::DOWN) && !buttonLambda("jumpBtn", BUTTON::UP) &&
				!buttonLambda("dashBtn", BUTTON::Dash)   && !buttonLambda("transformBtn"+ trStr, BUTTON::Transfrom))
			{
				touchesStart(touch);
				if (!moveFlag)
				{
					auto startSp = director->getRunningScene()->getChildByTag((int)zOlder::FRONT)->getChildByName("startSp");
					startSp->setPosition(touchVectors[touch->getID()].pos);
					moveFlag = true;
					touchVectors[touch->getID()].isMoveTouch = true;
				}
			}
		}
		return true;
	};
	listener->onTouchesMoved = [&](std::vector<Touch*> touches, Event* event)
	{
		auto gameScene = cocos2d::Director::getInstance()->getRunningScene();
		if (gameScene->getName() != "GameScene")
		{
			return false;
		}

		for (auto touch : touches) {
			touchesMove(touch);
		}
		return true;
	};
	listener->onTouchesEnded = [this](std::vector<Touch*> touches, Event* event)
	{
		for (auto touch : touches)
		{
			touchesEnd(touch);
			if (touchVectors[touch->getID()].isMoveTouch)
			{
				touchVectors[touch->getID()].isMoveTouch = false;
				moveFlag = false;
				auto startSp = Director::getInstance()->getRunningScene()->getChildByTag((int)zOlder::FRONT)->getChildByName("startSp");
				startSp->setPosition(150, 150);
			}
		}


		return true;
	};
	delta->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, delta);
}

void OPRT_touch::KeyReset(void)
{
	for (auto key = static_cast<int>(BUTTON::UP); key < static_cast<int>(BUTTON::MAX); key++)
	{
		_keyData._input[key] = false;
		_keyData._data[key] = false;
		_keyData._oldData[key] = false;
	}
}

const std::array<bool, static_cast<int>(BUTTON::MAX)>& OPRT_touch::GetNowData(void)
{
	return _keyData._data;
}

const std::array<bool, static_cast<int>(BUTTON::MAX)>& OPRT_touch::GetOldData(void)
{
	return _keyData._oldData;
}