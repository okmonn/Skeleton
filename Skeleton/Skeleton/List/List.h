#pragma once
#include "../Vector2.h"

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
struct D3D12_INDEX_BUFFER_VIEW;

class List
{
public:
	// コンストラクタ
	List(const D3D12_COMMAND_LIST_TYPE& type);
	// デストラクタ
	~List();

	// リセット
	void Reset(ID3D12PipelineState* pipe = nullptr) const;

	// ビューポートのセット
	void Viewport(const Vec2& size) const;

	// シザーのセット
	void Scissor(const Vec2& size) const;

	// バリアのセット
	void Barrier(const D3D12_RESOURCE_STATES& befor, const D3D12_RESOURCE_STATES& affter, ID3D12Resource* rsc) const;

	// 描画用ルートのセット
	void Root(ID3D12RootSignature* root) const;

	// 計算用ルートのセット
	void  ComputeRoot(ID3D12RootSignature* root) const;

	// パイプのセット
	void Pipe(ID3D12PipelineState* pipe) const;

	// ヒープのセット
	void Heap(ID3D12DescriptorHeap** heap, const size_t& num) const;

	// ヒープと描画ルートとの関連付け
	void RootTable(const unsigned int& id, ID3D12DescriptorHeap* heap, const unsigned int& index = 0) const;

	// ヒープと計算用ルートの関連付け
	void ComputeRootTable(const unsigned int& id, ID3D12DescriptorHeap* heap, const unsigned int& index = 0) const;

	// 頂点バッファビューのセット
	void VertexView(const D3D12_VERTEX_BUFFER_VIEW& view) const;

	// インデックスバッファビューのセット
	void IndexView(const D3D12_INDEX_BUFFER_VIEW& view) const;

	// トポロジーのセット
	void Topology(const D3D_PRIMITIVE_TOPOLOGY& type) const;

	// 頂点描画
	void DrawVertex(const size_t& num, const unsigned int& instance = 1) const;

	// GPU計算の実行
	void Dispatch(const unsigned int& x, const unsigned int& y, const unsigned int& z) const;

	// バンドルの実行
	void Bundle(ID3D12GraphicsCommandList* list) const;

	// クローズ
	void Close(void);

	// アロケータの取得
	ID3D12CommandAllocator* GetAllo(void) const {
		return allo;
	}
	// リストの取得
	ID3D12GraphicsCommandList* GetList(void) const {
		return list;
	}

private:
	// アロケータも生成
	long CreateAllo(const D3D12_COMMAND_LIST_TYPE& type);

	// リストの生成
	long CreateList(const D3D12_COMMAND_LIST_TYPE& type);


	// アロケータ
	ID3D12CommandAllocator* allo;

	// リスト
	ID3D12GraphicsCommandList* list;
};
