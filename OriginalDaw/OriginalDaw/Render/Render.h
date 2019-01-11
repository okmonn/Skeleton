#pragma once
#include <vector>
#include <memory>

struct ID3D12DescriptorHeap;
struct ID3D12Resource;
class List;
class Swap;

class Render
{
public:
	// コンストラクタ
	Render(std::weak_ptr<Swap>swap);
	// デストラクタ
	~Render();

	// クリア
	void Clear(std::weak_ptr<List>list, ID3D12DescriptorHeap* depth = nullptr);

	// ヒープの取得
	ID3D12DescriptorHeap* GetHeap(void);

	// リソースの取得
	ID3D12Resource* GetRsc(void);
	// リソースの取得
	ID3D12Resource* GetRsc(const unsigned int& i);

private:
	// レンダーの生成
	void Create(void);


	// スワップ
	std::weak_ptr<Swap>swap;

	// ヒープID
	int heap;

	// リソースID
	std::vector<int>rsc;
};
