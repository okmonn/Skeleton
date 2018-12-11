#pragma once
#include <DirectXMath.h>

namespace vmd {
	// モーションデータ
	struct Motion {
		//フレーム
		unsigned long flam;
		//位置
		DirectX::XMFLOAT3 pos;
		//クォータニオン
		DirectX::XMFLOAT4 rotation;
		//補完
		DirectX::XMFLOAT2 a;
		DirectX::XMFLOAT2 b;
	};
}
