#include "OPRT_key.h"

USING_NS_CC;

OPRT_key::OPRT_key(Sprite* sprite)
{
	for (auto key = static_cast<int>(BUTTON::UP); key < static_cast<int>(BUTTON::MAX); key++)
	{
		_keyData._input[key] = false;
		_keyData._data[key] = false;
		_keyData._oldData[key] = false;
	}
	keyflg(sprite);
}

OPRT_key::~OPRT_key()
{
}

OPRT_TYPE OPRT_key::GetType(void)
{
	return OPRT_TYPE::KEY;
}

void OPRT_key::update()
{
	_keyData._oldData = _keyData._data;
	_keyData._data = _keyData._input;
}

// Playerでやること
// ボタンを押したとき・離したときの処理
// その瞬間にしか呼ばれない
// ボタン情報を作る
void OPRT_key::keyflg(Sprite* sprite)
{
   auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = [&](EventKeyboard::KeyCode keyCode, Event* event)->bool {
		// ボタンを押したらフラグをONにする
		if (keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW) 
		{
			_keyData._input[static_cast<int>(BUTTON::UP)] = true;
		}
		if (keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW) 
		{
			_keyData._input[static_cast<int>(BUTTON::DOWN)] = true;
		}
		if (keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW) 
		{
			_keyData._input[static_cast<int>(BUTTON::RIGHT)] = true;
			_keyData._input[static_cast<int>(BUTTON::LEFT)] = false;
		}
		if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
		{
			if (_keyData._input[static_cast<int>(BUTTON::RIGHT)])
			{
				_keyData._input[static_cast<int>(BUTTON::LEFT)] = false;
			}
			else
			{
				_keyData._input[static_cast<int>(BUTTON::LEFT)] = true;
			}
		}

		if (keyCode == EventKeyboard::KeyCode::KEY_A)
		{
			_keyData._input[static_cast<int>(BUTTON::Transfrom)] = true;
		}
		if (keyCode == EventKeyboard::KeyCode::KEY_D)
		{
			_keyData._input[static_cast<int>(BUTTON::Dash)] = true;
		}
		if (keyCode == EventKeyboard::KeyCode::KEY_Q)
		{
			_keyData._input[static_cast<int>(BUTTON::SkillA)] = true;
		}
		if (keyCode == EventKeyboard::KeyCode::KEY_W)
		{
			_keyData._input[static_cast<int>(BUTTON::SkillB)] = true;
		}
		return true;
	};

	listener->onKeyReleased = [&](EventKeyboard::KeyCode keyCode, Event* event)->bool {
		// ESCボタンでゲームを終了させる
		if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE) 
		{
			Director::getInstance()->end();
		}
		// ボタンを離したらフラグをOFFにする
		if (keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW) 
		{
			_keyData._input[static_cast<int>(BUTTON::UP)] = false;
		}
		if (keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW)
		{
			_keyData._input[static_cast<int>(BUTTON::DOWN)] = false;
		}
		if (keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW) 
		{
			_keyData._input[static_cast<int>(BUTTON::RIGHT)] = false;
		}
		if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW) 
		{
			_keyData._input[static_cast<int>(BUTTON::LEFT)] = false;
		}

		if (keyCode == EventKeyboard::KeyCode::KEY_A)
		{
			_keyData._input[static_cast<int>(BUTTON::Transfrom)] = false;
		}
		if (keyCode == EventKeyboard::KeyCode::KEY_D)
		{
			_keyData._input[static_cast<int>(BUTTON::Dash)] = false;
		}
		if (keyCode == EventKeyboard::KeyCode::KEY_Q)
		{
			_keyData._input[static_cast<int>(BUTTON::SkillA)] = false;
		}
		if (keyCode == EventKeyboard::KeyCode::KEY_W)
		{
			_keyData._input[static_cast<int>(BUTTON::SkillB)] = false;
		}
		return true;
	};

	sprite->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, sprite);
}

const std::array<bool, static_cast<int>(BUTTON::MAX)> &OPRT_key::GetNowData(void)
{
	return _keyData._data;
}

const std::array<bool, static_cast<int>(BUTTON::MAX)> &OPRT_key::GetOldData(void)
{
	return _keyData._oldData;
}

void OPRT_key::KeyReset(void)
{
	for (auto key = static_cast<int>(BUTTON::UP); key < static_cast<int>(BUTTON::MAX); key++)
	{
		_keyData._input[key] = false;
		_keyData._data[key] = false;
		_keyData._oldData[key] = false;
	}
}
