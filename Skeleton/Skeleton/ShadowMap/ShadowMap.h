#pragma once
#include <memory>

class DescriptorMane;
class Window;
class Device;
class List;
class Queue;
class Fence;

class ShadowMap
{
public:
	// コンストラクタ　
	ShadowMap(std::weak_ptr<Window>win, std::weak_ptr<Device>dev, std::weak_ptr<List>list);
	// デストラクタ
	~ShadowMap();

	// 描画準備
	void Set(void);

	// 実行
	void Execution(void);

	// 深度ヒープIDの取得
	int& GetDepthHeap(void) {
		return dHeap;
	}
	// シェーダーヒープIDの取得
	int& GetShaderHeap(void) {
		return sHeap;
	}

private:
	// リソースの生成
	long CreateRsc(void);

	// 深度ビューの生成
	void CreateDepthView(void);

	// シェーダービューの生成
	void CreateShaderView(void);

	// 初期化
	void Init(void);


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

	// 深度ヒープ
	int dHeap;
	
	// シェーダーヒープ
	int sHeap;

	// リソース
	int rsc;
};
