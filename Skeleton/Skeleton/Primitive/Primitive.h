#pragma once
#include <vector>
#include <memory>
#include <DirectXMath.h>

enum D3D_PRIMITIVE_TOPOLOGY : int;
class DescriptorMane;
class Window;
class Device;
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

protected:
	// 定数リソースの生成
	long CreateRsc(void);

	// 定数バッファビューの生成
	void CreateView(void);

	// 定数のマップ
	long Map(void);

	// 頂点リソースの生成
	long CreateVrsc(void);


	// ディスクリプターマネージャー
	DescriptorMane& descMane;

	// ウィンドウ
	std::weak_ptr<Window>win;

	// デバイス
	std::weak_ptr<Device>dev;

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
