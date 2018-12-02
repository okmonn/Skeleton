#pragma once
#include <map>
#include <memory>

enum D3D12_DESCRIPTOR_HEAP_FLAGS : int;
enum D3D12_DESCRIPTOR_HEAP_TYPE : int;
enum D3D12_RESOURCE_STATES:int;
struct D3D12_HEAP_PROPERTIES;
struct D3D12_RESOURCE_DESC;
struct D3D12_CLEAR_VALUE;
struct ID3D12DescriptorHeap;
struct ID3D12Resource;
class Device;

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
	long CreateHeap(std::weak_ptr<Device>dev, int& i, const D3D12_DESCRIPTOR_HEAP_FLAGS& flag = static_cast<D3D12_DESCRIPTOR_HEAP_FLAGS>(0),
		const unsigned int& num = 1, const D3D12_DESCRIPTOR_HEAP_TYPE& type = static_cast<D3D12_DESCRIPTOR_HEAP_TYPE>(0));

	// リソースの生成
	long CreateRsc(std::weak_ptr<Device>dev, int& i, const D3D12_HEAP_PROPERTIES& prop, const D3D12_RESOURCE_DESC& desc, D3D12_CLEAR_VALUE* clear = nullptr,
		const D3D12_RESOURCE_STATES& state = static_cast<D3D12_RESOURCE_STATES>(2755));

	// リソースの削除
	void DeleteRsc(int& i);
	// ヒープの削除
	void DeleteHeap(int& i);
	// 削除
	void Delete(int& i);

	// ヒープの取得
	ID3D12DescriptorHeap* GetHeap(int& i) {
		return heap[&i];
	}
	// リソースの取得
	ID3D12Resource* GetRsc(int& i) {
		return rsc[&i];
	}

private:
	// コンストラクタ
	DescriptorMane();
	DescriptorMane(const DescriptorMane&) {
	}
	void operator=(const DescriptorMane&) {
	}


	// ヒープ
	std::map<int*, ID3D12DescriptorHeap*>heap;

	// リソース
	std::map<int*, ID3D12Resource*>rsc;
};
