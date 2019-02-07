#pragma once
#include "PrimitiveInfo.h"
#include <list>
#include <vector>
#include <memory>
#include <functional>
#include <unordered_map>

enum D3D_PRIMITIVE_TOPOLOGY : int;
class Window;
class List;
class Primitive;

class PrimitiveMane
{
public:
	// コンストラクタ
	PrimitiveMane(std::weak_ptr<Window>win);
	// デストラクタ
	~PrimitiveMane();

	// 描画
	void Draw(std::weak_ptr<List>list, const std::vector<prm::Vertex>& vertex, const prm::PrimitiveType& type, const D3D_PRIMITIVE_TOPOLOGY& pType);

	// クリア
	void Clear(void);

private:
	// 初期化
	void Init(void);


	// ウィンドウ
	std::weak_ptr<Window>win;

	// プリミティブ
	std::list<std::unique_ptr<Primitive>>primitive;

	// 関数ポインタ
	std::unordered_map<prm::PrimitiveType, std::function<void(const size_t)>>func;
};
