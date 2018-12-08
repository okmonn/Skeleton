#pragma once
#include <memory>

class DescriptorMane;
class Device;
class List;

class ShadowMap
{
public:
	// コンストラクタ　
	ShadowMap(std::weak_ptr<Device>dev, const unsigned int& width, const unsigned int& height);
	// デストラクタ
	~ShadowMap();

	// 深度ヒープIDの取得
	int& GetDepthHeap(void) {
		return dHeap;
	}

	// シェーダーヒープIDの取得
	int& GetShaderHeap(void) {
		return sHeap;
	}

private:
	// ディスクリプタマネージャー
	DescriptorMane& descMane;

	// デバイス
	std::weak_ptr<Device>dev;

	// 深度ヒープ
	int dHeap;
	
	// シェーダーヒープ
	int sHeap;

	// リソース
	int rsc;
};
