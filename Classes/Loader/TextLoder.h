#pragma once
#include <list>
#include <string>
#define lpTextLoder TextLoder::GetInstance()
struct Param
{
	std::list<std::string> textData;	// �e�L�X�g�Ƃ��ĕ\�����镶����
	unsigned int price;					// ���i�f�[�^��ۑ�����
	unsigned int power;					// �Ἠf�[�^��ۑ�����
};
class TextLoder
{
public:
	static TextLoder& GetInstance()
	{
		static TextLoder s_Instance;
		return s_Instance;
	}
	//�e�L�X�g�̓ǂݍ���
	Param TextReader(std::string path);
private:
	TextLoder();
	~TextLoder();
};

