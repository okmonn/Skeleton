#pragma once
#include <memory>

struct ID3D12DescriptorHeap;
struct ID3D12Resource;
class Window;
class List;

class Depth
{
public:
	// コンストラクタ
	Depth(std::weak_ptr<Window>win);
	// デストラクタ
	~Depth();

	// クリア
	void Clear(std::weak_ptr<List>list);

	// ヒープの取得
	ID3D12DescriptorHeap* GetHeap(void);
	// リソースの取得
	ID3D12Resource* GetRsc(void);

private:
	// デプスの生成
	void Create(void);


	// ウィンドウ
	std::weak_ptr<Window>win; 

	// ヒープID
	int heap;

	// リソースのID
	int rsc;
};
