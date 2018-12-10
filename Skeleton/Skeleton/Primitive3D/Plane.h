#pragma once
#include "Primitive3D.h"

class ShadowMap;

class Plane :
	public Primitive3D
{
public:
	// コンストラクタ
	Plane(std::weak_ptr<Device>dev, std::weak_ptr<Camera>cam, std::weak_ptr<Root>root, std::weak_ptr<Pipe>pipe);
	// デストラク
	~Plane();

	// 頂点のセット
	long SetVertex(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& size,
		const DirectX::XMFLOAT3& color, const float& alpha);

	// 描画
	void Draw(std::weak_ptr<List>list, std::weak_ptr<ShadowMap>shadow);

private:
	// 初期化
	void Init(void);
};
