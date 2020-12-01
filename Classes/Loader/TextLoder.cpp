#include <fstream>
#include <sstream>
#include "TextLoder.h"

Param TextLoder::TextReader(std::string path)
{
    std::ifstream file(path);  // �ǂݍ��ރt�@�C���̃p�X���w��
    std::string line;          // �ǂݍ��񂾕�����f�[�^�̈ꎞ�ۑ��p�i1�s�j
    Param fileData;            // �e�L�X�g�t�@�C�����̃f�[�^��ԋp���邽�߂̕ϐ�
    while (std::getline(file, line))
    { 
        // 1�s���ǂݍ���
        fileData.textData.push_back(line + "\n");
    }
    
    // ���i�f�[�^�ƉἨf�[�^��int�^�Ƃ��ĕۑ�����
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
