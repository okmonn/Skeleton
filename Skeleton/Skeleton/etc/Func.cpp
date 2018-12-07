#include "Func.h"
#include <Windows.h>

// ƒƒCƒh•¶š—ñ‚Ì•ÏŠ·
std::wstring func::ChangeWString(const std::string & st)
{
	//•¶š”‚Ìæ“¾
	auto byteSize = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, st.c_str(), -1, nullptr, 0);

	std::wstring wstr;
	wstr.resize(byteSize);

	//•ÏŠ·
	byteSize = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, st.c_str(), -1, &wstr[0], byteSize);

	return wstr;
}

// •¶š—ñ‚ÌŒŸõ
std::string func::FindString(const std::string & path, const char & find, const unsigned int & offset, const bool & end)
{
	unsigned int pos = (end == false) ? path.find_first_of(find) : path.find_last_of(find);
	pos += offset;

	std::string tmp = path.substr(0, pos);

	return tmp;
}
