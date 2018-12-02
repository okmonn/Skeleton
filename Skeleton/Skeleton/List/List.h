#pragma once
#include <memory>

enum D3D12_COMMAND_LIST_TYPE : int;
enum D3D12_RESOURCE_STATES : int;
struct ID3D12CommandAllocator;
struct ID3D12GraphicsCommandList;
struct ID3D12PipelineState;
struct ID3D12Resource;
class Device;

class List
{
public:
	// コンストラクタ
	List(std::weak_ptr<Device>dev, const D3D12_COMMAND_LIST_TYPE& type = (D3D12_COMMAND_LIST_TYPE)0);
	// デストラクタ
	~List();

	// リセット
	void Reset(ID3D12PipelineState* pipe = nullptr);

	// ビューポートのセット
	void SetView(const unsigned int& width, const unsigned int& height);

	// シザーのセット
	void SetScissor(const unsigned int& width, const unsigned int& height);

	// バリアのセット
	void SetBarrier(const D3D12_RESOURCE_STATES& befor, const D3D12_RESOURCE_STATES& affter, ID3D12Resource* rsc);

	// アロケータの取得
	ID3D12CommandAllocator* GetAllo(void) const {
		return allo;
	}
	// リストの取得
	ID3D12GraphicsCommandList* GetList(void) const {
		return list;
	}

private:
	// アロケータの生成
	long CreateAllo(const D3D12_COMMAND_LIST_TYPE& type);

	// リストの生成
	long CreateList(const D3D12_COMMAND_LIST_TYPE& type);


	// デバイス
	std::weak_ptr<Device>dev;

	// アロケータ
	ID3D12CommandAllocator* allo;

	// リスト
	ID3D12GraphicsCommandList* list;
};
