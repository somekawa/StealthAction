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
		/*�擾�����p�X���f�B���N�g���ł��邩�ǂ����̔���*/
		if (win32_fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (win32_fd.cFileName[0] == '.')
			{
				continue;
			}
			std::string fllPath = findPath + win32_fd.cFileName;
			if (std::find(path.begin(), path.end(), fllPath) == path.end())
			{
				// ������x���擾�������Ƃ��Ȃ��t�H���_�̃p�X�ł������Ȃ�A
				// pathList�̈�ԍŏ��̃f�[�^�Ƃ��Ēǉ�����
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
					// ������x���ǉ����Ă��Ȃ��t�@�C�����ł���΁Akey�ɒǉ�����
					key = data.first;
					break;
				}
			}
			mapData_[key].push_back(f_name);
		}
		// �t�@�C����������Ȃ��Ȃ�܂ŌJ��Ԃ�.
	} while (FindNextFileA(h_find, &win32_fd));
	// ���ݒT�����Ă���K�w�ɖ��T���t�@�C����������΁ApathList����폜����
	// ���T����path��O�ɒǉ����Ă������ߌ�납��폜���Ă���
	pathList.pop_back();
	if (pathList.empty() == false)
	{
		/* �������T���̃t�@�C��������̂ł���΂�����x�J��Ԃ�
		�A�����̍ےT�������͈̂�x�T���ς݂̊K�w�ƈ�Ԑ󂢊K�w�ȊO�ɂȂ�*/
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
