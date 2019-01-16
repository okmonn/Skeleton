#include "FontLoader.h"
#include <Windows.h>
#include <tchar.h>

// コンストラクタ
FontLoader::FontLoader()
{
	font.clear();
}

// デストラクタ
FontLoader::~FontLoader()
{
	for (auto itr1 = font.begin(); itr1 != font.end(); ++itr1)
	{
		for (auto itr2 = itr1->second.begin(); itr2 != itr1->second.end(); ++itr2)
		{
			if (itr2->second != nullptr)
			{
				DeleteObject(itr2->second);
			}
		}
	}
}

// フォントデータの読み込み
long FontLoader::Load(const std::tstring & fontName, const unsigned int & size)
{
	if (font.find(fontName) != font.end())
	{
		OutputDebugString(_T("\nフォント読み込み済み\n"));
		return S_FALSE;
	}
	if (font[fontName].find(size) != font[fontName].end())
	{
		OutputDebugString(_T("\nフォント読み込み済み\n"));
		return S_FALSE;
	}

	LOGFONT log{};
	log.lfHeight         = static_cast<long>(size);
	log.lfCharSet        = SHIFTJIS_CHARSET;
	log.lfOutPrecision   = OUT_TT_ONLY_PRECIS;
	log.lfClipPrecision  = CLIP_DEFAULT_PRECIS;
	log.lfQuality        = PROOF_QUALITY;
	log.lfPitchAndFamily = FIXED_PITCH | FF_MODERN;
	memcpy(log.lfFaceName, &fontName[0], sizeof(fontName[0]) * fontName.size());
	//wsprintf(log.lfFaceName, fontName.c_str());

	font[fontName][size] = CreateFontIndirect(&log);
	if (font[fontName][size] == nullptr)
	{
		OutputDebugString(_T("\nフォント画像の生成：失敗\n"));
		return S_FALSE;
	}

	return S_OK;
}
