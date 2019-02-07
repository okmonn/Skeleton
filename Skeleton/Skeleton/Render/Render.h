#pragma once
#include <vector>
#include <memory>

struct ID3D12DescriptorHeap;
struct ID3D12Resource;
class List;
class Swap;
class Depth;

class Render
{
public:
	// コンストラクタ
	Render(std::weak_ptr<Swap>swap);
	// デストラクタ
	~Render();

	// クリア
	void Clear(std::weak_ptr<List>list, std::weak_ptr<Depth>depth);

	// リソースの取得
	ID3D12Resource* GetRsc(void) const;

	// ヒープの取得
	ID3D12DescriptorHeap* GetHeap(void) const {
		return rHeap;
	}

private:
	// レンダー用ヒープの生成
	long CreateRHeap(void);

	// レンダー用リソースの生成
	long CreateRRsc(void);

	// RTVの生成
	void RTV(void);

	// 初期化
	void Init(void);


	// スワップ
	std::weak_ptr<Swap>swap;

	// ヒープ
	ID3D12DescriptorHeap* rHeap;

	// リソース
	std::vector<ID3D12Resource*>rRsc;
};
