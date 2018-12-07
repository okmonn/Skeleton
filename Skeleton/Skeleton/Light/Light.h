#pragma once
#include <DirectXMath.h>

class Light
{
public:
	// コンストラクタ
	Light();
	// デストラクタ
	~Light();

	// 座標の取得
	DirectX::XMFLOAT3 GetPos(void) const {
		return pos;
	}
	// 座標のセット
	void SetPos(const DirectX::XMFLOAT3& pos) {
		this->pos = pos;
	}

private:
	// 座標
	DirectX::XMFLOAT3 pos;
};
