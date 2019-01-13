#pragma once
#include "PrimitiveInfo.h"
#include <list>
#include <string>
#include <memory>

class Primitive;
class Window;
class List;
class Root;

class PrimitiveMane
{
public:
	// コンストラクタ
	PrimitiveMane(std::weak_ptr<Window>win, std::weak_ptr<Root>root);
	// デストラクタ
	~PrimitiveMane();

	// 描画
	void Draw(std::weak_ptr<List>list, const prm::PrimitiveType& type, prm::Vertex* vertex, const size_t& num);

	// クリア
	void Clear(void);

private:
	// パイプの生成
	void CreatePipe(void);


	// ウィンドウ
	std::weak_ptr<Window>win;

	// ルート
	std::weak_ptr<Root>root;

	// プリミティブ
	std::list<std::unique_ptr<Primitive>>primitive;
};
