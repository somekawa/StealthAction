// 全て担当
#include "OPRT_key.h"

USING_NS_CC;

OPRT_key::OPRT_key(Sprite* sprite)
{
	for (auto key = static_cast<int>(BUTTON::RIGHT); key < static_cast<int>(BUTTON::MAX); key++)
	{
		keyData_.input_[key]   = false;
		keyData_.data_[key]    = false;
		keyData_.oldData_[key] = false;
	}

	keyMap_.emplace(EventKeyboard::KeyCode::KEY_RIGHT_ARROW, BUTTON::RIGHT);
	keyMap_.emplace(EventKeyboard::KeyCode::KEY_LEFT_ARROW , BUTTON::LEFT);
	keyMap_.emplace(EventKeyboard::KeyCode::KEY_SPACE	   , BUTTON::Transfrom);
	keyMap_.emplace(EventKeyboard::KeyCode::KEY_UP_ARROW   , BUTTON::UP);
	keyMap_.emplace(EventKeyboard::KeyCode::KEY_S, BUTTON::Attack);
	keyMap_.emplace(EventKeyboard::KeyCode::KEY_D, BUTTON::Dash);
	keyMap_.emplace(EventKeyboard::KeyCode::KEY_Q, BUTTON::SkillA);
	keyMap_.emplace(EventKeyboard::KeyCode::KEY_W, BUTTON::SkillB);
	keyMap_.emplace(EventKeyboard::KeyCode::KEY_E, BUTTON::SkillC);
	SetUpKey(sprite);
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
	keyData_.oldData_ = keyData_.data_;
	keyData_.data_    = keyData_.input_;
}

void OPRT_key::SetUpKey(Sprite* sprite)
{
	auto listener = EventListenerKeyboard::create();

	listener->onKeyPressed = [&](EventKeyboard::KeyCode keyCode, Event* event)->bool {
		// ボタン押下時、フラグをONにする
		for (auto& key : keyMap_)
		{
			if (keyCode == key.first)
			{
				keyData_.input_[static_cast<int>(key.second)] = true;
			}

			// 左右移動キーの優先順位
			if (keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
			{
				keyData_.input_[static_cast<int>(BUTTON::LEFT)] = false;
				continue;
			}

			if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
			{
				if (keyData_.input_[static_cast<int>(BUTTON::RIGHT)])
				{
					keyData_.input_[static_cast<int>(BUTTON::LEFT)] = false;
					break;
				}
				else
				{
					keyData_.input_[static_cast<int>(BUTTON::LEFT)] = true;
					break;
				}
			}
		}
		return true;
	};

	listener->onKeyReleased = [&](EventKeyboard::KeyCode keyCode, Event* event)->bool {
		// ESCボタンでゲームを終了させる
		if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE) 
		{
			Director::getInstance()->end();
		}

		// ボタン非押下時、フラグをOFFにする
		for (auto& key : keyMap_)
		{
			if (keyCode == key.first)
			{
				keyData_.input_[static_cast<int>(key.second)] = false;
			}
		}
		return true;
	};

	sprite->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, sprite);
}

const std::array<bool, static_cast<int>(BUTTON::MAX)> &OPRT_key::GetNowData(void)
{
	return keyData_.data_;
}

const std::array<bool, static_cast<int>(BUTTON::MAX)> &OPRT_key::GetOldData(void)
{
	return keyData_.oldData_;
}

void OPRT_key::ResetKey(void)
{
	for (auto key = static_cast<int>(BUTTON::RIGHT); key < static_cast<int>(BUTTON::MAX); key++)
	{
		keyData_.input_[key]   = false;
		keyData_.data_[key]    = false;
		keyData_.oldData_[key] = false;
	}
}