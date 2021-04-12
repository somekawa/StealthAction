#include <cocos2d.h>
#include <CCFileUtils.h>
#include "FileLoder.h"

FileLoder::FileLoder()
{
}

FileLoder::~FileLoder()
{
}

DataTable FileLoder::Directory(std::list<std::string> path)
{
	std::string findPath = path.front();
	if (!pathList.empty())
	{
		pathList.pop_front();
	}
	const auto fileUtils = cocos2d::FileUtils::getInstance();

	directoryName = fileUtils->listFiles(findPath);

	for (auto itr = directoryName.begin(); itr != directoryName.end(); itr++)
	{
		const auto findItr = (*itr).find(findPath);
		(*itr) = (*itr).substr(findItr);
		if (fileUtils->isFileExist((*itr)))
		{
			(*itr) = (*itr).substr((*itr).find("/") + 1);
			const std::string fileName = (*itr).substr((*itr).find("/") + 1);
			mapData_[(*itr).erase((*itr).find("/"))].push_back(fileName);
		}
		else
		{
			if ((*itr).find("./") != std::string::npos)
			{
				continue;
			}
			pathList.push_back((*itr));
		}
	}
	if (!pathList.empty())
	{
		Directory(pathList);
	}
	return mapData_;
}