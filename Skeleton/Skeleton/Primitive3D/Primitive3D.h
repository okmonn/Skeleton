#pragma once
#include <vector>
#include <memory>
#include <DirectXMath.h>

class DescriptorMane;
class Device;
class List;
class Camera;
class Root;
class Pipe;

namespace prm3d {
	struct Vertex
	{
		//座標
		DirectX::XMFLOAT3 pos;
		//UV
		DirectX::XMFLOAT2 uv;
		//色
		DirectX::XMFLOAT4 color;
	};
}

class Primitive3D
{
	struct WVP {
		DirectX::XMFLOAT4X4 world;
		DirectX::XMFLOAT4X4 view;
		DirectX::XMFLOAT4X4 projection;
	};

public:
	// コンストラクタ
	Primitive3D();
	// デストラクタ
	virtual ~Primitive3D();

private:
	// 定数リソースの生成
	long CreateRsc(void);

	// 定数ビューの生成
	void CreateView(void);

	// 定数のマップ
	long Map(void);

	// 頂点リソースの生成
	long CreateVertex(void);

	// ディスクリプタマネージャー
	DescriptorMane& descMane;

	// デバイス
	std::weak_ptr<Device>dev;

	// リスト
	std::unique_ptr<List>list;

	// カメラ
	std::weak_ptr<Camera>cam;

	// ルート
	std::weak_ptr<Root>root;

	// パイプ
	std::weak_ptr<Pipe>pipe;

	// 定数ヒープ・リソース
	int constant;

	// 頂点リソース
	int vRsc;

	// 頂点
	std::vector<prm3d::Vertex>vertex;
};
