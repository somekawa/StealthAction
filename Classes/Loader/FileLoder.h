#pragma once
#include <vector>
#include <list>
#include <string>
#include <map>
#define lpFileLoder FileLoder::GetInstance()
using DataTable = std::map<std::string, std::vector<std::string>>;
class FileLoder
{
public:
	static FileLoder& GetInstance()
	{
		static FileLoder s_Instance;
		return s_Instance;
	}
	// File�T���p�֐�
	// �Ԃ�l�Ƃ��Ĉ�Ԑ[��directory�������̊K�w��key�Ƃ���std::map��vector�ŕԂ�
	DataTable Directory(std::list<std::string>path/*�T�����J�n����directory��*/);
private:
	FileLoder();
	~FileLoder();

	std::list<std::string> pathList;
	std::vector<std::string> directoryName;
	std::vector<std::string> f_names;
	DataTable mapData_;
};