#pragma once
#include "../etc/tstring.h"

struct ID3D12DescriptorHeap;
struct ID3D12Resource;

class Window
{
public:
	// コンストラクタ
	Window(const unsigned int& = 640, const unsigned int& = 480);
	// デストラクタ
	~Window();

	// タイトル名の変更
	void ChangeTitle(const std::tstring& title);

	// ウィンドウハンドルの取得
	void* Get(void) const {
		return handle;
	}
	// ウィンドウの横幅の取得
	unsigned int GetWidth(void) const {
		return width;
	}
	// ウィンドウの縦幅の取得
	unsigned int GetHeight(void) const {
		return height;
	}
	// ヒープの取得
	ID3D12DescriptorHeap* GetHeap(void);
	// リソースの取得
	ID3D12Resource* GetRsc(void);

private:
	// ウィンドウのコールバック
#if _WIN64
	static long __stdcall WindowProc(void* hWnd, unsigned int message, unsigned __int64 wParam, __int64 lParam);
#else
	static long __stdcall WindowProc(void* hWnd, unsigned int message, long wParam, long lParam);
#endif

	// ウィンドウの生成
	void Create(void);

	// 定数バッファの生成
	void ConstantBuffer(void);


	// ウィンドウの横幅
	unsigned int width;

	// ウィンドウの縦幅
	unsigned int height;

	// ウィンドウハンドル
	void* handle;

	// ウィンドウインスタンス
	void* instance;

	// ウィンドウ名
	const wchar_t* name;

	// 定数バッファID
	int constant;
};
