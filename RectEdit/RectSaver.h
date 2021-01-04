#pragma once
#include <memory>
#include <unordered_map>
#include "ActionRect.h"

// Sceneで作成した矩形情報をﾌｧｲﾙにｾｰﾌﾞするｼﾝｸﾞﾙﾄﾝｸﾗｽ
// 1ﾌﾚｰﾑの矩形情報
using SharedActionRect = std::vector<std::shared_ptr<ActionRect>>;

class RectSaver
{
public:
	static RectSaver& GetInstance(void)
	{
		static RectSaver s_instance;
		return s_instance;
	}

	bool SaveRect(const std::vector <SharedActionRect>& actionRect,
		const std::string& animationName,
		const std::string& actorName);

	bool WriteData(const std::vector <SharedActionRect>& actionRect,
		const std::string& animationName,
		const std::string& actorName);
	bool LoadRect(std::vector <SharedActionRect>& actionRect,
		const std::string& animationName,
		const std::string& actorName);
private:
	RectSaver() = default;
	RectSaver(const RectSaver&) = delete;
	void operator=(const RectSaver&) = delete;

	// ﾍｯﾀﾞｰﾌｧｲﾙ(矩形の個数情報)のﾌｧｲﾙﾎﾟｲﾝﾀ
	FILE* headerFile_;
	// 矩形情報のﾌｧｲﾙﾎﾟｲﾝﾀ
	FILE* file_;

};

