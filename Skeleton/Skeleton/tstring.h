#pragma once
#include <string>

namespace std
{
	// ユニコード
#ifdef _UNICODE
	typedef wchar_t TCHAR;
	// マルチバイト
#else
	typedef char TCHAR;
#endif

	// 変数化
	typedef std::basic_string<TCHAR, std::char_traits<TCHAR>, std::allocator<TCHAR> > tstring;
}