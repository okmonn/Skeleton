#include "Device.h"
#include "../Release.h"
#include <vector>

// 機能レベル一覧
const D3D_FEATURE_LEVEL levels[] = {
	D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_12_1,
	D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_12_0,
	D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_1,
	D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0,
};

// コンストラクタ
Device::Device() :
	dev(nullptr), factory(nullptr)
{
	Init();
}

// デストラクタ
Device::~Device()
{
	Release(factory);
	Release(dev);
}

// ファクトリー生成
long Device::CreateFactory(void)
{
	auto hr = CreateDXGIFactory(IID_PPV_ARGS(&factory));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nファクトリー生成：失敗\n"));
	}

	return hr;
}

// デバイス生成
long Device::CreateDev(void)
{
	//アダプターの検索
	std::vector<IDXGIAdapter*>adapters;
	IDXGIAdapter* adapter = nullptr;
	for (UINT i = 0; factory->EnumAdapters(i, &adapter) != DXGI_ERROR_NOT_FOUND; ++i)
	{
		adapters.push_back(adapter);
	}
	for (auto& i : adapters)
	{
		DXGI_ADAPTER_DESC desc{};
		i->GetDesc(&desc);
		std::wstring name = desc.Description;
		if (name.find(L"NVIDIA") != std::string::npos)
		{
			adapter = i;
			break;
		}
		if (name.find(L"AMD") != std::string::npos)
		{
			adapter = adapters[0];
			break;
		}
	}

	auto hr = S_OK;
	for (auto& i : levels)
	{
		hr = D3D12CreateDevice(adapter, i, IID_PPV_ARGS(&dev));
		if (hr == S_OK)
		{
			OutputDebugString(_T("\nデバイス生成：成功\n"));
			Release(adapter);
			break;
		}
	}

	return hr;
}

// 初期化
void Device::Init(void)
{
	CreateFactory();
	CreateDev();
}

// インスタンス変数取得
Device & Device::Get(void)
{
	static Device instance;
	return instance;
}

// デバイス取得
ID3D12Device * Device::GetDev(void) const
{
	return dev;
}

// ファクトリー取得
IDXGIFactory4 * Device::GetFactory(void) const
{
	return factory;
}
