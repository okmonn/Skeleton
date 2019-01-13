#pragma once
#include <memory>
#include <unordered_map>

enum D3D12_DESCRIPTOR_HEAP_FLAGS : int;
enum D3D12_DESCRIPTOR_HEAP_TYPE : int;
enum D3D12_RESOURCE_STATES :int;
struct D3D12_HEAP_PROPERTIES;
struct D3D12_RESOURCE_DESC;
struct D3D12_CLEAR_VALUE;
struct ID3D12DescriptorHeap;
struct ID3D12Resource;

class Swap;

enum RSC_TYPE {
	RSC_TYPE_NON, 
	RSC_TYPE_RTV,
	RSC_TYPE_DSV,
	RSC_TYPE_CBV,
	RSC_TYPE_SRV
};

class DescriptorMane
{
public:
	// デストラクタ
	~DescriptorMane();

	// インスタンス変数の取得
	static DescriptorMane& Get(void) {
		static DescriptorMane instance;
		return instance;
	}

	// ヒープの生成
	long CreateHeap(int& addr, const D3D12_DESCRIPTOR_HEAP_FLAGS& flag, const size_t& num, const D3D12_DESCRIPTOR_HEAP_TYPE& type);

	// リソースの生成
	long CreateRsc(int& addr, const D3D12_HEAP_PROPERTIES& prop, const D3D12_RESOURCE_DESC& desc, const D3D12_RESOURCE_STATES& state, const D3D12_CLEAR_VALUE* clear = nullptr);
	// リソースの生成
	long CreateRsc(int& addr, std::weak_ptr<Swap>swap, const unsigned int& index);

	// RTVの生成
	void RTV(int& heapAddr, int& rscAddr, const unsigned int& index = 0);

	// DSVの生成
	void DSV(int& heapAddr, int& rscAddr, const unsigned int& index = 0);

	// CBVの生成
	void CBV(int& heapAddr, int& rscAddr, const size_t& size, const unsigned int& index = 0);

	// SRVの生成
	void SRV(int& heapAddr, int& rscAddr, const unsigned int& index = 0);

	// UAVの生成
	void UAV(int& heapAddr, int& rscAddr, const unsigned int& stride, const unsigned int& num, const unsigned int& index = 0);

	// ヒープの削除
	void DeleteHeap(int& addr);

	// リソースの削除
	void DeleteRsc(int& addr);

	// ヒープの取得
	ID3D12DescriptorHeap* GetHeap(int& addr) {
		return heap[&addr];
	}
	// リソースの取得
	ID3D12Resource* GetRsc(int& addr) {
		return rsc[&addr];
	}

private:
	// コンストラクタ
	DescriptorMane();
	DescriptorMane(const DescriptorMane&) = delete;
	void operator=(const DescriptorMane&) = delete;


	// ヒープ
	std::unordered_map<int*, ID3D12DescriptorHeap*>heap;

	// リソース
	std::unordered_map<int*, ID3D12Resource*>rsc;
};
