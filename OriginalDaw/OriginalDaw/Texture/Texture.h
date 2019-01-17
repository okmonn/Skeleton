#pragma once
#include "TextureInfo.h"
#include <string>

class Window;
class List;
class Root;
class Pipe;

class Texture
{
public:
	// コンストラクタ
	Texture(std::weak_ptr<Window>win, std::weak_ptr<Root>root, std::weak_ptr<Pipe>pipe);
	// デストラクタ
	~Texture();

	// 読み込み
	void Load(const std::string& fileName);

	// 描画準備
	void SetDraw(const DirectX::XMFLOAT2& pos, const DirectX::XMFLOAT2& size, const DirectX::XMFLOAT2& uvPos, const DirectX::XMFLOAT2& uvSize,
		const float& alpha, const float& angle, const bool& turnX, const bool& turnY);

	// 描画
	void Draw(std::weak_ptr<List>list);

private:
	// 頂点バッファの生成
	void VertexBuffer(void);

	// 定数バッファの生成
	void ConstantBuffer(void);

	// シェーダバッファの生成
	void ShaderBuffer(void);

	// サブリソースに書き込む
	long WriteSub(const std::string& fileName);


	// ウィンドウ
	std::weak_ptr<Window>win;

	// ルート
	std::weak_ptr<Root>root;

	// パイプ
	std::weak_ptr<Pipe>pipe;

	// サブデータ
	std::weak_ptr<D3D12_SUBRESOURCE_DATA>sub;

	// 定数データ
	tex::CbvInfo* data;

	// シェーダリソース
	ID3D12Resource* rsc;

	// 定数バッファID
	int constant;

	// 頂点リソースID
	int vRsc;
};
