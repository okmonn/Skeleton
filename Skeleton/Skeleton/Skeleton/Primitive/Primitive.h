#pragma once
#include <memory>

struct ID3D12Resource;
class Window;
class List;
class Root;
class Pipe;

class Primitive
{
public:
	// コンストラクタ
	Primitive(std::weak_ptr<Window>win, std::weak_ptr<Root>root, std::weak_ptr<Pipe>pipe, const size_t& num);
	// デストラクタ
	~Primitive();

private:
	// 頂点リソースの生成
	long CreateVRsc(const size_t& num);


	// ウィンドウ
	std::weak_ptr<Window>win;

	// ルート
	std::weak_ptr<Root>root;

	// パイプ
	std::weak_ptr<Pipe>pipe;

	// 頂点リソース
	ID3D12Resource* rsc;
};
