#pragma once
#include "PrimitiveInfo.h"
#include <vector>
#include <memory>

enum D3D_PRIMITIVE_TOPOLOGY : int;
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

	// 描画
	void Draw(std::weak_ptr<List>list, const std::vector<prm::Vertex>& vertex, const D3D_PRIMITIVE_TOPOLOGY& type);

private:
	// 頂点リソースの生成
	long CreateVRsc(const size_t& num);

	// マップ
	long Map(const std::vector<prm::Vertex>& vertex);


	// ウィンドウ
	std::weak_ptr<Window>win;

	// ルート
	std::weak_ptr<Root>root;

	// パイプ
	std::weak_ptr<Pipe>pipe;

	// 頂点リソース
	ID3D12Resource* rsc;
};
