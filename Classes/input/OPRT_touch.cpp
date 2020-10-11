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
	touchflg(delta);
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
	// 攻撃テスト用
	//_keyData._input[static_cast<int>(BUTTON::DOWN)] = false;
}

void OPRT_touch::touchStart(cocos2d::Touch * touch)
{
	auto loc = touch->getLocation();
	touchStartPoint = Point(loc.x, loc.y);
	touchEndPoint = Point(loc.x, loc.y);
	swipeRotate = SWIPE_CENTER;
}

void OPRT_touch::touchMove(cocos2d::Touch * touch)
{
	auto startPosOffset = 20;
	auto loc = touch->getLocation();
	auto start = touchStartPoint;
	auto end = touchEndPoint;
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
		_keyData._input[static_cast<int>(BUTTON::UP)] = true;
	}
	if (swipeRotate == SWIPE_DOWN) {
		//_keyData._input[static_cast<int>(BUTTON::DOWN)] = true;
	}
	if (swipeRotate == SWIPE_LEFT) {
		_keyData._input[static_cast<int>(BUTTON::LEFT)] = true;
	}
	if (swipeRotate == SWIPE_RIGHT) {
		_keyData._input[static_cast<int>(BUTTON::RIGHT)] = true;
	}
}

void OPRT_touch::touchEnd(cocos2d::Touch * touch)
{
	touchStartPoint = touchEndPoint = Point(-9999, -9999);
	for (auto key = static_cast<int>(BUTTON::UP); key < static_cast<int>(BUTTON::MAX); key++)
	{
		_keyData._input[key] = false;
	}
}

void OPRT_touch::touchflg(cocos2d::Sprite * delta)
{
	// 攻撃テスト用
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [this](cocos2d::Touch * touch, cocos2d::Event * event)->bool {
		Point p = touch->getLocation();
		auto director = Director::getInstance();
		auto label1 = director->getRunningScene()->getChildByName("UI_LAYER")->getChildByName("test");
		//auto label1 = director->getRunningScene()->getChildByName("test") -> getChildByName("item");
		//auto visibleSize = director->getVisibleSize();
		//float x = visibleSize.width - label1->getContentSize().width / 2;
		//float y = label1->getContentSize().height / 2;
		//label1->setPosition(Vec2(x, y));
		//auto a = x;
		//auto b = y;
		auto r = label1->getBoundingBox();
		if (r.containsPoint(p)) {
			// label1がクリック/タッチされた場合の処理
			_keyData._input[static_cast<int>(BUTTON::DOWN)] = true;
		}
		else
		{
			touchStart(touch);
			return true;
		}
		return true;
	};
	//delta->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, delta);

	//auto listener = EventListenerTouchOneByOne::create();
	//listener->onTouchBegan = [this](cocos2d::Touch* touch, cocos2d::Event* event)->bool {
	//	touchStart(touch);
	//	return true;
	//};
	listener->onTouchMoved = [this](cocos2d::Touch* touch, cocos2d::Event* event)->bool {
		touchMove(touch);
		return true;
	};
	listener->onTouchEnded = [this](cocos2d::Touch* touch, cocos2d::Event* event)->bool {
		touchEnd(touch);
		return true;
	};
	delta->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, delta);
}

const std::array<bool, static_cast<int>(BUTTON::MAX)>& OPRT_touch::GetNowData(void)
{
	return _keyData._data;
}

const std::array<bool, static_cast<int>(BUTTON::MAX)>& OPRT_touch::GetOldData(void)
{
	return _keyData._oldData;
}