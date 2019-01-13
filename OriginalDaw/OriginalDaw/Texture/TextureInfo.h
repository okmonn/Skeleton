#pragma once
#include <memory>
#include <DirectXMath.h>

struct D3D12_SUBRESOURCE_DATA;
struct ID3D12Resource;

namespace tex {
	// 元データ
	struct Info {
		//リソース
		ID3D12Resource* rsc;
		//デコード
		std::unique_ptr<unsigned char[]>decode;
		//サブデータ
		std::shared_ptr<D3D12_SUBRESOURCE_DATA>sub;
	};

	// 頂点
	struct Vertex {
		//座標
		DirectX::XMFLOAT3 pos;
		//uv
		DirectX::XMFLOAT2 uv;
	};

	// 定数バッファ情報
	struct CbvInfo {
		//移動行列
		DirectX::XMFLOAT4X4 matrix;
		//uv座標
		DirectX::XMFLOAT2 uvPos;
		//uvサイズ
		DirectX::XMFLOAT2 uvSize;
		//反転
		DirectX::XMFLOAT2 reverse;
		//アルファ値
		float alpha;
	};
}
