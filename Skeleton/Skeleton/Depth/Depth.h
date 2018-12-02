#pragma once
#include <memory>

struct ID3D12DescriptorHeap;
struct ID3D12Resource;
class Device;
class List;

class Depth
{
public:
	// コンストラクタ
	Depth(std::weak_ptr<Device>dev, const unsigned int& width, const unsigned int& height);
	// デストラクタ
	~Depth();

	// 深度のクリア
	void Clear(std::shared_ptr<List>list);

	// ヒープの取得
	ID3D12DescriptorHeap* GetHeap(void) const {
		return heap;
	}
	// リソースの取得
	ID3D12Resource* GetRsc(void) const {
		return rsc;
	}

private:
	// ヒープの生成
	long CreateHeap(void);

	// リソースの生成
	long CreateRsc(const unsigned int& width, const unsigned int& height);

	// ビューの生成
	void CreateView(void);


	// デバイス
	std::weak_ptr<Device>dev;

	// ヒープ
	ID3D12DescriptorHeap* heap;

	// リソース
	ID3D12Resource* rsc;
};