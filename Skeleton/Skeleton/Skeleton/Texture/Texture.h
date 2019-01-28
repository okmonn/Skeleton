#pragma once
#include "TexInfo.h"
#include <string>
#include <vector>

struct ID3D12DescriptorHeap;
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

	// 描画の準備
	void SetDraw(const DirectX::XMFLOAT2& pos, const DirectX::XMFLOAT2& size, const DirectX::XMFLOAT2& uvPos, const DirectX::XMFLOAT2& uvSize,
		const float& alpha, const float& angle, const bool& turnX, const bool& turnY);

	// 描画
	void Draw(std::weak_ptr<List>list);

private:
	// 頂点リソースの生成
	long CreateVRsc(void);

	// 定数用ヒープの生成
	long CreateHeap(void);

	// 定数リソースの生成
	long CreateCRsc(void);

	// CBVの生成
	void CBV(void);

	// マップ
	long Map(void);

	// 初期化
	void Init(void);

	// SRVの生成
	void SRV(void);

	// サブリソースに書込み
	long WriteSub(const std::string& fileName);


	// ウィンドウ
	std::weak_ptr<Window>win;

	// ルート
	std::weak_ptr<Root>root;

	// パイプ
	std::weak_ptr<Pipe>pipe;

	// ヒープ
	ID3D12DescriptorHeap* heap;

	// 定数用リソース
	ID3D12Resource* cRsc;

	// シェーダリソース
	ID3D12Resource* sRsc;

	// 頂点用リソース
	ID3D12Resource* vRsc;

	// 送信用データ
	tex::CbvInfo* info;

	// インデックス
	unsigned int index;
};
