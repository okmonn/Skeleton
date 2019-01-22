#pragma once
#include "../etc/Vector2.h"
#include "../etc/tstring.h"

struct ID3D12DescriptorHeap;
struct ID3D12Resource;

class Window
{
public:
	// コンストラクタ
	Window(const Vec2& size, void* parent);
	// デストラクタ
	~Window();

	// タイトル名の変更
	void ChangeTitle(const std::string& title);

	// ウィンドウハンドルの取得
	void* Get(void) const {
		return handle;
	}
	// ウィンドウサイズの取得
	Vec2 GetSize(void) const {
		return size;
	}
	//ドロップしたファイルパスの取得
	std::string GetDropFilePath(void);
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
	void Create(void* parent);

	// 定数バッファの生成
	void ConstantBuffer(void);


	// ウィンドウサイズ
	Vec2 size;

	// ウィンドウハンドル
	void* handle;

	// ウィンドウインスタンス
	void* instance;

	// ウィンドウ名
	const wchar_t* name;

	// 定数バッファID
	int constant;

	// ドロップされたファイルパス
	static std::tstring dropFilePath;
};
