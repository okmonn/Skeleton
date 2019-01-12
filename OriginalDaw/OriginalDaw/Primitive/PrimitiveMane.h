#pragma once
#include "PrimitiveInfo.h"
#include <list>
#include <vector>
#include <memory>
#include <unordered_map>

struct ID3D12Resource;
class Primitive;
class Window;
class List;
class Root;
class Pipe;

class PrimitiveMane
{
	struct Copy {
		Copy() {}
		Copy(ID3D12Resource* rsc, std::vector<prm::Vertex>vertex) : 
		rsc(rsc), vertex(vertex){}
		ID3D12Resource* rsc;
		std::vector<prm::Vertex>vertex;
	};

public:
	// コンストラクタ
	PrimitiveMane(std::weak_ptr<Window>win, std::weak_ptr<Root>root);
	// デストラクタ
	~PrimitiveMane();

	// 描画
	void Draw(std::weak_ptr<List>list, std::weak_ptr<Pipe>pipe, const prm::PrimitiveType& type, prm::Vertex* vertex, const size_t& num);

private:
	// オリジンのセット
	void Origin(void);

	// マップ
	long Map(Copy& copy);


	// ウィンドウ
	std::weak_ptr<Window>win;

	// ルート
	std::weak_ptr<Root>root;

	// オリジン
	std::unordered_map<prm::PrimitiveType, std::unique_ptr<Primitive>>origin;

	// コピー
	std::list<Copy>primitive;
};
