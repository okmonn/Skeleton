#include "List.h"
#include "../Release.h"

// コンストラクタ
List::List(const D3D12_COMMAND_LIST_TYPE & type) : 
	allo(nullptr), list(nullptr)
{
	CreateAllo(type);
	CreateList(type);
}

// デストラクタ
List::~List()
{
	Release(list);
	Release(allo);
}

// アロケータも生成
long List::CreateAllo(const D3D12_COMMAND_LIST_TYPE & type)
{
	auto hr = Dev->CreateCommandAllocator(type, IID_PPV_ARGS(&allo));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nアロケータの生成：失敗\n"));
	}

	return hr;
}

// リストの生成
long List::CreateList(const D3D12_COMMAND_LIST_TYPE & type)
{
	auto hr = Dev->CreateCommandList(0, type, allo, nullptr, IID_PPV_ARGS(&list));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nリストの生成：失敗\n"));
		return hr;
	}

	Close();

	return hr;
}

// リセット
void List::Reset(ID3D12PipelineState * pipe) const
{
	allo->Reset();
	list->Reset(allo, pipe);
}

// ビューポートのセット
void List::Viewport(const Vec2 & size) const
{
	D3D12_VIEWPORT view{};
	view.Height   = static_cast<float>(size.y);
	view.MaxDepth = 1.0f;
	view.Width    = static_cast<float>(size.x);

	list->RSSetViewports(1, &view);
}

// シザーのセット
void List::Scissor(const Vec2 & size) const
{
	RECT scissor{};
	scissor.bottom = static_cast<long>(size.y);
	scissor.right  = static_cast<long>(size.x);

	list->RSSetScissorRects(1, &scissor);
}

// バリア
void List::Barrier(const D3D12_RESOURCE_STATES & befor, const D3D12_RESOURCE_STATES & affter, ID3D12Resource * rsc) const
{
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type                   = D3D12_RESOURCE_BARRIER_TYPE::D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags                  = D3D12_RESOURCE_BARRIER_FLAGS::D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource   = rsc;
	barrier.Transition.StateBefore = befor;
	barrier.Transition.StateAfter  = affter;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_FLAGS::D3D12_RESOURCE_BARRIER_FLAG_NONE;

	list->ResourceBarrier(1, &barrier);
}

// 描画用ルートのセット
void List::Root(ID3D12RootSignature * root) const
{
	list->SetGraphicsRootSignature(root);
}

// 計算用ルートのセット
void List::ComputeRoot(ID3D12RootSignature * root) const
{
	list->SetComputeRootSignature(root);
}

// パイプラインのセット
void List::Pipe(ID3D12PipelineState * pipe) const
{
	list->SetPipelineState(pipe);
}

// ヒープのセット
void List::Heap(ID3D12DescriptorHeap ** heap, const size_t & num) const
{
	list->SetDescriptorHeaps(static_cast<unsigned int>(num), heap);
}

// ヒープと描画ルートとの関連付け
void List::RootTable(const unsigned int & id, ID3D12DescriptorHeap * heap, const unsigned int & index) const
{
	auto handle = heap->GetGPUDescriptorHandleForHeapStart();
	handle.ptr += Dev->GetDescriptorHandleIncrementSize(heap->GetDesc().Type) * index;

	list->SetGraphicsRootDescriptorTable(id, handle);
}

// ヒープと計算用ルートの関連付け
void List::ComputeRootTable(const unsigned int & id, ID3D12DescriptorHeap * heap, const unsigned int & index) const
{
	auto handle = heap->GetGPUDescriptorHandleForHeapStart();
	handle.ptr += Dev->GetDescriptorHandleIncrementSize(heap->GetDesc().Type) * index;

	list->SetComputeRootDescriptorTable(id, handle);
}

// 頂点バッファビューのセット
void List::VertexView(const D3D12_VERTEX_BUFFER_VIEW & view) const
{
	list->IASetVertexBuffers(0, 1, &view);
}

// インデックスバッファビューのセット
void List::IndexView(const D3D12_INDEX_BUFFER_VIEW & view) const
{
	list->IASetIndexBuffer(&view);
}

// トポロジーのセット
void List::Topology(const D3D_PRIMITIVE_TOPOLOGY & type) const
{
	list->IASetPrimitiveTopology(type);
}

// 頂点描画
void List::DrawVertex(const size_t & num, const unsigned int & instance) const
{
	list->DrawInstanced(static_cast<unsigned int>(num), instance, 0, 0);
}

// GPU計算の実行
void List::Dispatch(const unsigned int & x, const unsigned int & y, const unsigned int & z) const
{
	list->Dispatch(x, y, z);
}

// バンドルの実行
void List::Bundle(ID3D12GraphicsCommandList * list) const
{
	this->list->ExecuteBundle(list);
}

// クローズ
void List::Close(void)
{
	list->Close();
}
