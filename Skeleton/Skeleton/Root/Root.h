#pragma once
#include <string>

struct ID3D10Blob;
typedef ID3D10Blob ID3DBlob;
struct ID3D12RootSignature;

class Root
{
public:
	// コンストラクタ
	Root(const std::wstring& fileName);
	// デストラクタ
	~Root();

	// ルートの取得
	ID3D12RootSignature* Get(void) const {
		return root;
	}
	// シグネチャの取得
	ID3DBlob* GetSig(void) const {
		return sig;
	}
	// 頂点シェーダの取得
	ID3DBlob* GetVertex(void) const {
		return vertex;
	}
	// ジオメトリーシェーダの取得
	ID3DBlob* GetGeometry(void) const {
		return geometry;
	}
	// ピクセルシェーダの取得
	ID3DBlob* GetPixel(void) const {
		return pixel;
	}
	// コンピュートシェーダの取得
	ID3DBlob* GetCompute(void) const {
		return compute;
	}

private:
	// シェーダのコンパイル
	long Compile(const std::wstring& fileName);

	// ルート情報の取得
	long GetRootInfo(void);

	// ルートの生成
	long CreateRoot(void);


	// ルート
	ID3D12RootSignature* root;

	// シグネチャ
	ID3DBlob* sig;

	// 頂点シェーダ
	ID3DBlob* vertex;

	// ジオメトリーシェーダー
	ID3DBlob* geometry;

	// ピクセルシェーダ
	ID3DBlob* pixel;

	// コンピュートシェーダ
	ID3DBlob* compute;
};
