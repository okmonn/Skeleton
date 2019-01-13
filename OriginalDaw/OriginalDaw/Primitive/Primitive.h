#pragma once
#include "PrimitiveInfo.h"
#include <vector>
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
	Primitive(std::weak_ptr<Window>win, std::weak_ptr<Root>root, std::weak_ptr<Pipe>pipe, const prm::PrimitiveType& type);
	// デストラクタ
	~Primitive();

	// 描画
	void Draw(std::weak_ptr<List>list, prm::Vertex* vertex, const size_t& num);

protected:
	// 頂点バッファの生成
	void VertexBuffer(void);

	// マップ
	long Map(void);


	// ウィンドウ
	std::weak_ptr<Window>win;

	// ルート
	std::weak_ptr<Root>root;

	// パイプ
	std::weak_ptr<Pipe>pipe;

	// プリミティブタイプ
	prm::PrimitiveType type;

	// リソースID
	int rsc;

	// 頂点データ
	std::vector<prm::Vertex>vertex;
};
