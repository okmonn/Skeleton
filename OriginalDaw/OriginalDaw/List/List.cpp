#include "List.h"
#include "../Device/Device.h"
#include "../etc/Release.h"

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

// アロケータの生成
long List::CreateAllo(const D3D12_COMMAND_LIST_TYPE & type)
{
	auto hr = Device::Get().GetDev()->CreateCommandAllocator(type, IID_PPV_ARGS(&allo));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nアロケータの生成：失敗\n"));
	}

	return hr;
}

// リストの生成
long List::CreateList(const D3D12_COMMAND_LIST_TYPE & type)
{
	auto hr = Device::Get().GetDev()->CreateCommandList(0, type, allo, nullptr, IID_PPV_ARGS(&list));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nリストの生成：失敗\n"));
		return hr;
	}

	list->Close();

	return hr;
}

// リセット
void List::Reset(ID3D12PipelineState * pipe)
{
	allo->Reset();
	list->Reset(allo, pipe);
}

// ビューポートのセット
void List::SetView(const Vec2 & size)
{
	D3D12_VIEWPORT view{};
	view.Height   = static_cast<float>(size.y);
	view.MaxDepth = 1.0f;
	view.MinDepth = 0.0f;
	view.TopLeftX = 0.0f;
	view.TopLeftY = 0.0f;
	view.Width    = static_cast<float>(size.x);

	list->RSSetViewports(1, &view);
}

// シザーのセット
void List::SetScissor(const Vec2 & size)
{
	RECT scissor{};
	scissor.bottom = static_cast<long>(size.y);
	scissor.left   = 0;
	scissor.right  = static_cast<long>(size.x);
	scissor.top    = 0;

	list->RSSetScissorRects(1, &scissor);
}

// バリア
void List::Barrier(const D3D12_RESOURCE_STATES & befor, const D3D12_RESOURCE_STATES & affter, ID3D12Resource * rsc)
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

// 描画用ルートシグネチャのセット
void List::SetRoot(ID3D12RootSignature * root)
{
	list->SetGraphicsRootSignature(root);
}

// コンピュート用ルートシグネチャのセット
void List::SetComputeRoot(ID3D12RootSignature * root)
{
	list->SetComputeRootSignature(root);
}

// パイプラインのセット
void List::SetPipe(ID3D12PipelineState * pipe)
{
	list->SetPipelineState(pipe);
}

// ヒープのセット
void List::SetHeap(ID3D12DescriptorHeap ** heap, const size_t & num)
{
	list->SetDescriptorHeaps(num, heap);
}

// ヒープと描画用ルートシグネチャの関連付け
void List::SetRootTable(const unsigned int & id, ID3D12DescriptorHeap * heap, const unsigned int & index)
{
	auto handle = heap->GetGPUDescriptorHandleForHeapStart();
	handle.ptr += Device::Get().GetDev()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * index;

	list->SetGraphicsRootDescriptorTable(id, handle);
}

// ヒープとコンピュート用ルートシグネチャの関連付け
void List::SetComputeRootTable(const unsigned int & id, ID3D12DescriptorHeap * heap, const unsigned int & index)
{
	auto handle = heap->GetGPUDescriptorHandleForHeapStart();
	handle.ptr += Device::Get().GetDev()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * index;

	list->SetComputeRootDescriptorTable(id, handle);
}

// 頂点バッファのセット
void List::SetVertexBufferView(const D3D12_VERTEX_BUFFER_VIEW & view)
{
	list->IASetVertexBuffers(0, 1, &view);
}

// トポロジータイプのセット
void List::SetTopology(const D3D_PRIMITIVE_TOPOLOGY & type)
{
	list->IASetPrimitiveTopology(type);
}

// 頂点描画
void List::DrawVertex(const size_t & vertexNum, const unsigned int & instance)
{
	list->DrawInstanced(static_cast<unsigned int>(vertexNum), instance, 0, 0);
}

// コンピュートの実行
void List::Dispatch(const unsigned int & x, const unsigned int & y, const unsigned int & z)
{
	list->Dispatch(x, y, z);
}

// リストのクローズ
void List::Close(void)
{
	list->Close();
}

// バンドルの実行
void List::ExecuteBundle(ID3D12GraphicsCommandList * list)
{
	this->list->ExecuteBundle(list);
}
