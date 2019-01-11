#pragma once

enum D3D12_COMMAND_LIST_TYPE : int;
struct ID3D12CommandAllocator;
struct ID3D12GraphicsCommandList;

class List
{
public:
	// コンストラクタ
	List(const D3D12_COMMAND_LIST_TYPE& type);
	// デストラクタ
	~List();

	// ビューポートのセット
	void SetView(const unsigned int& width, const unsigned int& height);

	// シザーのセット
	void SetScissor(const unsigned int& width, const unsigned int& height);

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
