#pragma once
#include "../etc/tString.h"

class Window
{
public:
	// コンストラクタ　
	Window(const unsigned int& width = 640, const unsigned int& height = 480);
	// デストラクタ
	~Window();

	// タイトル名の変更
	void ChangeTitle(const std::tstring& title);

	// ウィンドウハンドルの取得
	void* Get(void) const {
		return handle;
	}
	// ウィンドウ横幅の取得
	unsigned int GetX(void) const {
		return width;
	}
	// ウィンドウ縦幅
	unsigned int GetY(void) const {
		return height;
	}

private:
	// ウィンドウのコールバック
#if _WIN64
	static long __stdcall WindowProc(void* hWnd, unsigned int message, unsigned __int64 wParam, __int64 lParam);
#else
	static long __stdcall WindowProc(void* hWnd, unsigned int message, long wParam, long lParam);
#endif

	// ウィンドウの生成
	void Create(void);


	// ウィンドウハンドル
	void* handle;

	// ウィンドウインスタンス
	void* instance;

	// ウィンドウ名
	const wchar_t* name;

	// ウィンドウの横幅
	unsigned int width;

	// ウィンドウの縦幅
	unsigned int height;
};
