#include <fstream>
#include <sstream>
#include "TextLoder.h"

Param TextLoder::TextReader(std::string path)
{
    std::ifstream file(path);  // 読み込むファイルのパスを指定
    std::string line;          // 読み込んだ文字列データの一時保存用（1行）
    Param fileData;            // テキストファイル内のデータを返却するための変数
    while (std::getline(file, line))
    { 
        // 1行ずつ読み込む
        fileData.textData.push_back(line + "\n");
    }
    
    // 価格データと火力データをint型として保存する
    fileData.power = atoi((fileData.textData.back()).c_str());
    fileData.textData.pop_back();
    fileData.price = atoi((fileData.textData.back()).c_str());
    fileData.textData.pop_back();

	return fileData;
}

TextLoder::TextLoder()
{
}

TextLoder::~TextLoder()
{
}
