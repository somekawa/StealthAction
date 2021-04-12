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
	// File’Tõ—pŠÖ”
	// •Ô‚è’l‚Æ‚µ‚Äˆê”Ô[‚¢directory–¼‚àˆê‚Âã‚ÌŠK‘w‚ğkey‚Æ‚µ‚½std::map‚Ìvector‚Å•Ô‚·
	DataTable Directory(std::list<std::string>path/*’Tõ‚ğŠJn‚·‚édirectory–¼*/);
private:
	FileLoder();
	~FileLoder();

	std::list<std::string> pathList;
	std::vector<std::string> directoryName;
	std::vector<std::string> f_names;
	DataTable mapData_;
};