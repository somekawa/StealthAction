#include <iostream>
#include <Windows.h>
#include "FileLoder.h"

DataTable FileLoder::Directory(std::list<std::string> path)
{
	HANDLE h_find;
	WIN32_FIND_DATAA win32_fd;
	pathList = path;
	std::string findPath = pathList.back();
	std::string Search_name = findPath + "/*";
	h_find = FindFirstFileA(Search_name.c_str(), &win32_fd);
	do
	{
		/*取得したパスがディレクトリであるかどうかの判定*/
		if (win32_fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (win32_fd.cFileName[0] == '.')
			{
				continue;
			}
			std::string fllPath = findPath + win32_fd.cFileName;
			if (std::find(path.begin(), path.end(), fllPath) == path.end())
			{
				// もし一度も取得したことがないフォルダのパスであったなら、
				// pathListの一番最初のデータとして追加する
				fllPath = fllPath;
				mapData_[win32_fd.cFileName];
				pathList.push_front(fllPath);
			}
		}
		else
		{
			std::string f_name = win32_fd.cFileName;
			f_name = f_name.erase(f_name.find("."));
			std::string key;
			for (auto data : mapData_)
			{
				if (findPath.find(data.first) != std::string::npos)
				{
					// もし一度も追加していないファイル名であれば、keyに追加する
					key = data.first;
					break;
				}
			}
			mapData_[key].push_back(f_name);
		}
		// ファイルが見つからなくなるまで繰り返す.
	} while (FindNextFileA(h_find, &win32_fd));
	// 現在探索している階層に未探索ファイルが無ければ、pathListから削除する
	// 未探索のpathを前に追加していくため後ろから削除していく
	pathList.pop_back();
	if (pathList.empty() == false)
	{
		/* もし未探査のファイルがあるのであればもう一度繰り返す
		但しその際探索されるのは一度探索済みの階層と一番浅い階層以外になる*/
		Directory(pathList);
	}
	FindClose(h_find);
	return mapData_;
}

FileLoder::FileLoder()
{
}

FileLoder::~FileLoder()
{
}
