#include <cocos2d.h>
#include <CCFileUtils.h>
#include "FileLoder.h"

DataTable FileLoder::Directory(std::list<std::string> path)
{
	std::string findPath = path.front();
	auto fileUtils = cocos2d::FileUtils::getInstance();

	directoryName = fileUtils->listFiles(findPath);

	if (pathList.empty() == false)
	{
		pathList.pop_front();
	}
	for (auto itr = directoryName.begin();itr != directoryName.end();itr++)
	{
		(*itr) = (*itr).substr((*itr).find(findPath));
		if (fileUtils->isFileExist((*itr)))
		{
			(*itr) = (*itr).substr((*itr).find("/")+1);
			auto fileName = (*itr).substr((*itr).find("/") + 1);
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
	if (pathList.empty() == false)
	{
		Directory(pathList);
	}
	return mapData_;
}

FileLoder::FileLoder()
{
}

FileLoder::~FileLoder()
{
}
