#pragma once
#include <string>

#define PI 3.14159265f
#define RAD(X) (X) * PI / 180.0f

namespace func
{
	// ƒƒCƒh•¶š—ñ‚Ì•ÏŠ·
	std::wstring ChangeWString(const std::string& st);

	// •¶š—ñ‚ÌŒŸõ
	std::string FindString(const std::string& path, const char& find, const unsigned int& offset = 1, const bool& end = true);
}
