#pragma once
#include <vector>
#include <memory>
#include <DirectXMath.h>

enum D3D_PRIMITIVE_TOPOLOGY : int;
class Window;
class List;
class Root;
class Pipe;

namespace prm {
	struct Vertex
	{
		//座標
		DirectX::XMFLOAT3 pos;
		//色
		DirectX::XMFLOAT4 color;
	};
}

class Primitive
{
public:
	// コンストラクタ
	Primitive();
	// デストラクタ
	virtual ~Primitive();

	// 頂点のセット
	virtual void SetVertex(const DirectX::XMFLOAT2& pos, const DirectX::XMFLOAT4& color) = 0;

	// 描画
	void Draw(std::weak_ptr<List>list);

protected:
	// 定数バッファの生成
	void ConstantBuffer(void);

	// 頂点バッファの生成
	void VertexBuffer(void);

	// バンドルのセット
	void Bundle(const D3D_PRIMITIVE_TOPOLOGY& type);


	// ウィンドウ
	std::weak_ptr<Window>win;

	// リスト
	std::unique_ptr<List>list;

	// ルート
	std::weak_ptr<Root>root;

	// パイプ
	std::weak_ptr<Pipe>pipe;

	// 定数ヒープ・リソース
	int constant;

	// 頂点リソース
	int vRsc;

	// 頂点
	std::vector<prm::Vertex>vertex;
};
