#include "Device.h"
#include "../Release.h"
#include <vector>

// �@�\���x���ꗗ
const D3D_FEATURE_LEVEL levels[] = {
	D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_12_1,
	D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_12_0,
	D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_1,
	D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0,
};

// �R���X�g���N�^
Device::Device() :
	dev(nullptr), factory(nullptr)
{
	Init();
}

// �f�X�g���N�^
Device::~Device()
{
	Release(factory);
	Release(dev);
}

// �t�@�N�g���[����
long Device::CreateFactory(void)
{
	auto hr = CreateDXGIFactory(IID_PPV_ARGS(&factory));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n�t�@�N�g���[�����F���s\n"));
	}

	return hr;
}

// �f�o�C�X����
long Device::CreateDev(void)
{
	//�A�_�v�^�[�̌���
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
			OutputDebugString(_T("\n�f�o�C�X�����F����\n"));
			Release(adapter);
			break;
		}
	}

	return hr;
}

// ������
void Device::Init(void)
{
	CreateFactory();
	CreateDev();
}

// �C���X�^���X�ϐ��擾
Device & Device::Get(void)
{
	static Device instance;
	return instance;
}

// �f�o�C�X�擾
ID3D12Device * Device::GetDev(void) const
{
	return dev;
}

// �t�@�N�g���[�擾
IDXGIFactory4 * Device::GetFactory(void) const
{
	return factory;
}
