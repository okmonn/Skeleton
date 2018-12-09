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
	void Execution(std::weak_ptr<Queue>queue, std::weak_ptr<Fence>fence);

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

	// リスト
	std::weak_ptr<List>list;

	// 深度ヒープ
	int dHeap;
	
	// シェーダーヒープ
	int sHeap;

	// リソース
	int rsc;
};
