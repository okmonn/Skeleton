#pragma once

enum D3D12_COMMAND_LIST_TYPE : int;
enum D3D12_RESOURCE_STATES;
struct ID3D12CommandAllocator;
struct ID3D12GraphicsCommandList;
struct ID3D12Resource;
struct ID3D12RootSignature;
struct ID3D12PipelineState;

class List
{
public:
	// コンストラクタ
	List(const D3D12_COMMAND_LIST_TYPE& type);
	// デストラクタ
	~List();

	// リセット
	void Reset(ID3D12PipelineState* pipe = nullptr);

	// 描画用ルートシグネチャのセット
	void SetRoot(ID3D12RootSignature* root);

	// コンピュート用ルートシグネチャのセット
	void SetComputeRoot(ID3D12RootSignature* root);

	// パイプラインのセット
	void SetPipe(ID3D12PipelineState* pipe);

	// ビューポートのセット
	void SetView(const unsigned int& width, const unsigned int& height);

	// シザーのセット
	void SetScissor(const unsigned int& width, const unsigned int& height);

	// バリア
	void Barrier(const D3D12_RESOURCE_STATES & befor, const D3D12_RESOURCE_STATES & affter, ID3D12Resource * rsc);

	// リストのクローズ
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
	// アロケータの生成
	long CreateAllo(const D3D12_COMMAND_LIST_TYPE& type);

	// リストの生成
	long CreateList(const D3D12_COMMAND_LIST_TYPE& type);



	// アロケータ
	ID3D12CommandAllocator* allo;

	// リスト
	ID3D12GraphicsCommandList* list;
};
