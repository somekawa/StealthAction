#pragma once
#include <list>
#include <string>
#define lpTextLoder TextLoder::GetInstance()
struct Param
{
	std::list<std::string> textData;	// テキストとして表示する文字列
	unsigned int price;					// 価格データを保存する
	unsigned int power;					// 火力データを保存する
};
class TextLoder
{
public:
	static TextLoder& GetInstance()
	{
		static TextLoder s_Instance;
		return s_Instance;
	}
	//テキストの読み込み
	Param TextReader(std::string path);
private:
	TextLoder();
	~TextLoder();
};

