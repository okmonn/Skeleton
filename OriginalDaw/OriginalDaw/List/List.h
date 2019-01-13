#pragma once
#include "../etc/Vector2.h"

enum D3D12_COMMAND_LIST_TYPE : int;
enum D3D12_RESOURCE_STATES : int;
enum D3D_PRIMITIVE_TOPOLOGY : int;
struct ID3D12CommandAllocator;
struct ID3D12GraphicsCommandList;
struct ID3D12DescriptorHeap;
struct ID3D12Resource;
struct ID3D12RootSignature;
struct ID3D12PipelineState;
struct D3D12_VERTEX_BUFFER_VIEW;

class List
{
public:
	// コンストラクタ
	List(const D3D12_COMMAND_LIST_TYPE& type);
	// デストラクタ
	~List();

	// リセット
	void Reset(ID3D12PipelineState* pipe = nullptr);

	// ビューポートのセット
	void SetView(const Vec2& size);

	// シザーのセット
	void SetScissor(const Vec2& size);

	// バリア
	void Barrier(const D3D12_RESOURCE_STATES & befor, const D3D12_RESOURCE_STATES & affter, ID3D12Resource * rsc);

	// 描画用ルートシグネチャのセット
	void SetRoot(ID3D12RootSignature* root);

	// コンピュート用ルートシグネチャのセット
	void SetComputeRoot(ID3D12RootSignature* root);

	// パイプラインのセット
	void SetPipe(ID3D12PipelineState* pipe);

	// ヒープのセット
	void SetHeap(ID3D12DescriptorHeap** heap, const size_t& num);

	// ヒープと描画用ルートシグネチャの関連付け
	void SetRootTable(const unsigned int& id, ID3D12DescriptorHeap* heap, const unsigned int& index = 0);

	// ヒープとコンピュート用ルートシグネチャの関連付け
	void SetComputeRootTable(const unsigned int& id, ID3D12DescriptorHeap* heap, const unsigned int& index = 0);

	// 頂点バッファのセット
	void SetVertexBufferView(const D3D12_VERTEX_BUFFER_VIEW& view);

	// トポロジータイプのセット
	void SetTopology(const D3D_PRIMITIVE_TOPOLOGY& type);

	// 頂点描画
	void DrawVertex(const size_t& vertexNum, const unsigned int& instance = 1);

	// コンピュートの実行
	void Dispatch(const unsigned int& x, const unsigned int& y, const unsigned int& z);

	// リストのクローズ
	void Close(void);

	// バンドルの実行
	void ExecuteBundle(ID3D12GraphicsCommandList* list);

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



	// アロケータ
	ID3D12CommandAllocator* allo;

	// リスト
	ID3D12GraphicsCommandList* list;
};
