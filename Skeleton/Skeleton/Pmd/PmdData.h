#pragma once
#include <DirectXMath.h>

namespace pmd {
#pragma pack(1)
	// ヘッダー
	struct Header {
		//タイプ
		unsigned char type[3];
		//バージョン
		float ver;
		//名前
		unsigned char name[20];
		//コメント
		unsigned char comment[256];
	};
#pragma pack()

	// 頂点データ
	struct Vertex {
		//座標
		DirectX::XMFLOAT3 pos;
		//法線
		DirectX::XMFLOAT3 normal;
		//UV
		DirectX::XMFLOAT2 uv;
		//ボーン番号
		unsigned short bornNo[2];
		//ウェイト
		unsigned char weight;
		//輪郭線フラグ
		unsigned char edge;
	};

#pragma pack(1)
	// マテリアル
	struct Material {
		//基本色
		DirectX::XMFLOAT3 diffuse;
		//透明度
		float alpha;
		//反射強度
		float specularity;
		//反射色
		DirectX::XMFLOAT3 specula;
		//環境色
		DirectX::XMFLOAT3 mirror;
		//トゥーン番号
		unsigned char toonIndex;
		//輪郭線フラグ
		unsigned char edge;
		//インデックス数
		unsigned int indexNum;
		//テクスチャパス
		unsigned char texPath[20];
	};
#pragma pack()

	// ボーン
	struct Born {
		//名前
		char name[20];
		//親ボーン番号
		unsigned short pIndex;
		//子ボーン番号
		unsigned short cIndex;
		//タイプ
		unsigned char type;
		//IK親ボーン番号
		unsigned short IKpIndex;
		//座標
		DirectX::XMFLOAT3 pos;
	};

	// 送信時のマテリアル情報
	struct Mat {
		//基本色
		DirectX::XMFLOAT3 diffuse;
		//透明度
		float alpha;
		//反射強度
		float specularity;
		//反射色
		DirectX::XMFLOAT3 specula;
		//環境色
		DirectX::XMFLOAT3 mirror;
	};
}
