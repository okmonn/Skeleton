#include <d3dcompiler.h>
#include "Root.h"
#include "../Device/Device.h"
#include "../etc/Release.h"

#pragma comment(lib, "d3dcompiler.lib")

// コンストラクタ
Root::Root(const std::wstring& fileName) : 
	root(nullptr), sig(nullptr), vertex(nullptr), geometry(nullptr), pixel(nullptr), compute(nullptr)
{
	CompileShader(fileName);
	GetRootInfo();
	Create();
}

// デストラクタ
Root::~Root()
{
	Release(compute);
	Release(pixel);
	Release(geometry);
	Release(vertex);
	Release(sig);
	Release(root);
}

// シェーダのコンパイル
long Root::CompileShader(const std::wstring & fileName)
{
	auto hr = D3DCompileFromFile(fileName.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VS", "vs_5_1", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &vertex, nullptr);
	if (FAILED(hr))
	{
		hr = D3DCompileFromFile(fileName.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "CS", "cs_5_1", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &compute, nullptr);
		if (hr == S_OK)
		{
			OutputDebugString(_T("\nコンピュートシェーダの読み込み：成功\n"));
			return hr;
		}
		OutputDebugString(_T("\n頂点シェーダの読み込み：失敗\n"));
		return hr;
	}

	hr = D3DCompileFromFile(fileName.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "GS", "gs_5_1", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &geometry, nullptr);
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nジオメトリシェーダの読み込み：失敗\n"));
		OutputDebugString(_T("ジオメトリシェーダを使用していない場合は無視してOK\n"));
	}

	hr = D3DCompileFromFile(fileName.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PS", "ps_5_1", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &pixel, nullptr);
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nピクセルシェーダの読み込み：失敗\n"));
		return hr;
	}

	return hr;
}

// ルートシグネチャ情報の取得
long Root::GetRootInfo(void)
{
	auto hr = S_OK;
	if (vertex != nullptr)
	{
		hr = D3DGetBlobPart(vertex->GetBufferPointer(), vertex->GetBufferSize(), D3D_BLOB_PART::D3D_BLOB_ROOT_SIGNATURE, 0, &sig);
		if (FAILED(hr))
		{
			OutputDebugString(_T("\nルートシグネチャ情報の取得：失敗\n"));
		}
	}
	else
	{
		hr = D3DGetBlobPart(compute->GetBufferPointer(), compute->GetBufferSize(), D3D_BLOB_PART::D3D_BLOB_ROOT_SIGNATURE, 0, &sig);
		if (FAILED(hr))
		{
			OutputDebugString(_T("\nルートシグネチャ情報の取得：失敗\n"));
		}
	}

	return hr;
}

// ルートシグネチャの生成
long Root::Create(void)
{
	auto hr = Device::Get().GetDev()->CreateRootSignature(0, sig->GetBufferPointer(), sig->GetBufferSize(), IID_PPV_ARGS(&root));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nルートシグネチャの生成：失敗\n"));
	}

	return hr;
}
