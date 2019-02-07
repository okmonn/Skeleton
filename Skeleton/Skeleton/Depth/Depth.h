#pragma once
#include <memory>

struct ID3D12DescriptorHeap;
struct ID3D12Resource;
class Window;
class List;

class Depth
{
public:
	// コンストラクタ
	Depth(std::weak_ptr<Window>win);
	// デストラクタ
	~Depth();

	// クリア
	void Clear(std::weak_ptr<List>list);

	// ヒープの取得
	ID3D12DescriptorHeap* GetHeap(void) const {
		return dHeap;
	}
	// リソースの取得
	ID3D12Resource* GetRsc(void) const {
		return dRsc;
	}

private:
	// デプス用ヒープの生成
	long CreateDHeap(void);

	// デプス用リソースの生成
	long CreateDRsc(void);

	// DSVの生成
	void DSV(void);

	// 初期化
	void Init(void);


	// ウィンドウ
	std::weak_ptr<Window>win;

	// デプス用ヒープ
	ID3D12DescriptorHeap* dHeap;

	// デプス用リソース
	ID3D12Resource* dRsc;
};
