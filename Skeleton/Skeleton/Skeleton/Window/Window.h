#pragma once
#include "../Vector2.h"
#include "../tstring.h"

struct ID3D12DescriptorHeap;
struct ID3D12Resource;

class Window
{
public:
	// コンストラクタ
	Window(const Vec2& size, void* parent);
	// デストラクタ
	~Window();

	// ドロップされたファイルパスの取得
	std::string GetDropPass(void);

	// ウィンドウハンドルの取得
	void* Get(void) const {
		return handle;
	}
	// サイズの取得
	Vec2 GetSize(void) const {
		return size;
	}
	// ヒープの取得
	ID3D12DescriptorHeap* GetHeap(void) const {
		return cHeap;
	}
	// リソースの取得
	ID3D12Resource* GetRsc(void) const {
		return cRsc;
	}

private:
	// ウィンドウのコールバック
#ifdef _WIN64
	static long __stdcall WindowProc(void* hWnd, unsigned int message, unsigned __int64 wParam, __int64 lParam);
#else
	static long __stdcall WindowProc(void* hWnd, unsigned int message, long wParam, long lParam);
#endif

	// ウィンドウの生成
	int CreateWnd(void* parent);

	// 定数用ヒープの生成
	long CreateCHeap(void);

	// 定数リソースの生成
	long CreateCRsc(void);

	// CBVの生成
	void CBV(void);

	// マップ
	long Map(void);

	// 初期化
	void Init(void);


	// ウィンドウハンドル
	void* handle;

	// ウィンドウサイズ
	Vec2 size;

	// ウィンドウインスタンス
	void* instance;

	// ウィンドウ名
	const wchar_t* name;

	// ドロップされたファイルパス
	static std::tstring dropFilePass;

	// 定数用ヒープ
	ID3D12DescriptorHeap* cHeap;

	// リソース
	ID3D12Resource* cRsc;
};
