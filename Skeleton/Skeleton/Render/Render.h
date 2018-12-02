#pragma once
#include <vector>
#include <memory>

struct ID3D12DescriptorHeap;
struct ID3D12Resource;
class Device;
class List;
class Swap;

class Render
{
public:
	// コンストラクタ
	Render(std::weak_ptr<Device>dev, std::weak_ptr<Swap>swap);
	// デストラクタ
	~Render();

	// レンダーターゲットのクリア
	void Clear(std::weak_ptr<List>list, ID3D12DescriptorHeap* depth = nullptr);

	// ヒープの取得
	ID3D12DescriptorHeap* GetHeap(void) const {
		return heap;
	}
	// リソースの取得
	std::vector<ID3D12Resource*>GetRsc(void) const {
		return rsc;
	}
	// リソースの取得
	ID3D12Resource* Get(void) const;

private:
	// ヒープの生成
	long CreateHeap(const unsigned int& num);

	// リソースの生成
	long CreateRsc(void);


	// デバイス
	std::weak_ptr<Device>dev;

	// スワップ
	std::weak_ptr<Swap>swap;

	// ヒープ
	ID3D12DescriptorHeap* heap;

	// リソース
	std::vector<ID3D12Resource*>rsc;
};
