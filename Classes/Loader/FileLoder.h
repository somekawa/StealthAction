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
	// File探索用関数
	// 返り値としてdirectory名をkeyとしたstd::mapのvectorで返す
	DataTable Directory(std::list<std::string>);
private:
	FileLoder();
	~FileLoder();

	std::list<std::string> pathList;
	std::vector<std::string> directoryName;
	std::vector<std::string> f_names;
	DataTable mapData_;
};

