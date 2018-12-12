#include <d3dcompiler.h>
#include "Root.h"
#include "../Device/Device.h"
#include "../etc/Release.h"

// コンストラクタ
Root::Root(std::weak_ptr<Device> dev, const std::tstring & fileName) : 
	dev(dev), root(nullptr), sig(nullptr), error(nullptr), vertex(nullptr), geometry(nullptr), pixel(nullptr)
{
	ShaderCompile(fileName);
	Create();
}

// デストラクタ
Root::~Root()
{
	Release(pixel);
	Release(vertex);
	Release(sig);
	Release(error);
	Release(root);
}

// シェーダ読み込み
long Root::ShaderCompile(const std::tstring & fileName)
{
	auto hr = D3DCompileFromFile(fileName.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VS", "vs_5_1", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &vertex, &error);
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n頂点シェーダの読み込み：失敗\n"));
		return hr;
	}

	hr = D3DCompileFromFile(fileName.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "GS", "gs_5_1", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &geometry, &error);
	if(FAILED(hr))
	{
		OutputDebugString(_T("\nジオメトリシェーダの読み込み：失敗\n"));
	}

	hr = D3DCompileFromFile(fileName.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PS", "ps_5_1", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &pixel, &error);
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nピクセルシェーダの読み込み：失敗\n"));
		return hr;
	}

	//シェーダからルートシグネチャの情報を取得
	hr = D3DGetBlobPart(vertex->GetBufferPointer(), vertex->GetBufferSize(), D3D_BLOB_ROOT_SIGNATURE, 0, &sig);
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nルートシグネチャ情報の取得：失敗\n"));
	}

	return hr;
}

// ルートシグネチャの生成
long Root::Create(void)
{
	auto hr = dev.lock()->Get()->CreateRootSignature(0, sig->GetBufferPointer(), sig->GetBufferSize(), IID_PPV_ARGS(&root));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nルートシグネチャの生成：失敗\n"));
	}

	return hr;
}
