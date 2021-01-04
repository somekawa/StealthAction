#pragma once
#include <memory>
#include <unordered_map>
#include "ActionRect.h"

// Scene�ō쐬������`����̧�قɾ��ނ���ݸ���ݸ׽
// 1�ڰт̋�`���
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

	// ͯ�ް̧��(��`�̌����)��̧���߲��
	FILE* headerFile_;
	// ��`����̧���߲��
	FILE* file_;

};

