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

	// ルートシグネチャの取得
	ID3D12RootSignature* Get(void) const {
		return root;
	}
	// 頂点シェーダの取得
	ID3DBlob* GetVertex(void) const {
		return vertex;
	}
	// ジオメトリシェーダの取得
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
	long CompileShader(const std::wstring& fileName);

	// ルートシグネチャ情報の取得
	long GetRootInfo(void);

	// ルートシグネチャの生成
	long Create(void);


	// ルートシグネチャ
	ID3D12RootSignature* root;

	// メッセージ
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
