#pragma once
#include <memory>

class DescriptorMane;
class Window;
class Device;
class List;
class Queue;
class Depth;
class Fence;
class ShadowMap;
class Root;
class Pipe;

class MultiPass
{
public:
	// コンストラクタ
	MultiPass(std::weak_ptr<Window>win, std::weak_ptr<Device>dev, std::weak_ptr<List>list);
	// デストラクタ
	~MultiPass();

	// 描画準備
	void Set(std::weak_ptr<Depth>depth);

	// 実行
	void Execution(void);

	// 描画
	void Draw(std::weak_ptr<List>list, std::weak_ptr<Root>root, std::weak_ptr<Pipe>pipe, std::weak_ptr<ShadowMap>shadow);

private:
	// リソースの生成
	long CreateRsc(void);

	// レンダーターゲットビューの生成
	void CreateRenderView(void);

	// シェーダービューの生成
	void CreateShaderView(void);

	// 頂点リソースの生成	
	long CreateVertexRsc(void);

	// 頂点マップ
	long Map(void);

	// 初期化
	void Init(void);

	// レンダーターゲットのクリア
	void Clear(std::weak_ptr<Depth> depth);


	// ディスクリプタマネージャー
	DescriptorMane& descMane;

	// ウィンドウ
	std::weak_ptr<Window>win;

	// デバイス
	std::weak_ptr<Device>dev;

	// キュー
	std::shared_ptr<Queue>queue;

	// リスト
	std::weak_ptr<List>list;

	// フェンス
	std::unique_ptr<Fence>fence;

	// レンダーヒープ
	int rHeap;

	// シェーダーヒープ
	int sHeap;

	// リソース
	int rsc;

	// 頂点リソース
	int vRsc;
};
