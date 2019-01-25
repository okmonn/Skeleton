#include "../Release.h"

// 機能レベル一覧
const D3D_FEATURE_LEVEL levels[] = {
	D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_12_1,
	D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_12_0,
	D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_1,
	D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0
};

// コンストラクタ
Device::Device() : 
	dev(nullptr)
{
	CreateDev();
}

// デストラクタ
Device::~Device()
{
	Release(dev);
}

// デバイスの生成
long Device::CreateDev(void)
{
	long hr = S_OK;
	for (auto& i : levels)
	{
		hr = D3D12CreateDevice(nullptr, i, IID_PPV_ARGS(&dev));
		if (hr == S_OK)
		{
			OutputDebugString(_T("\nデバイスの生成：成功\n"));
			break;
		}
	}

	return hr;
}
