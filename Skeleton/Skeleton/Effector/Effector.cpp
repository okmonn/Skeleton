#include <d3dcompiler.h>
#include "Effector.h"
#include "../Device/Device.h"
#include "../Queue/Queue.h"
#include "../List/List.h"
#include "../Fence/Fence.h"
#include "../Root/Root.h"
#include "../Pipe/Pipe.h"
#include "../etc/Release.h"

// リソース数
#define RSC_MAX 3

// データサイズ
#define DATA_MAX 44100

// コンストラクタ
Effector::Effector(std::weak_ptr<Device>dev, const std::tstring & fileName) :
	dev(dev), root(nullptr), shader(nullptr), pipe(nullptr), heap(nullptr), index(0)
{
	param = {};
	info.clear();

	queue = std::make_shared<Queue>(dev, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_COMPUTE);
	list  = std::make_unique<List>(dev, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_COMPUTE);
	fence = std::make_unique<Fence>(dev, queue);

	CreateRoot(fileName);
	Init();
}

// デストラクタ
Effector::~Effector()
{
	for (auto itr = info.begin(); itr != info.end(); ++itr)
	{
		Release(itr->second.rsc);
	}

	Release(heap);
	Release(pipe);
	Release(shader);
	Release(root);
}

// ルートシグネチャの生成
long Effector::CreateRoot(const std::tstring & fileName)
{
	auto hr = D3DCompileFromFile(fileName.c_str(), nullptr, nullptr, "CS", "cs_5_1", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &shader, nullptr);
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nコンピュートシェーダの読み込み：失敗\n"));
		return hr;
	}

	ID3DBlob* sig = nullptr;

	hr = D3DGetBlobPart(shader->GetBufferPointer(), shader->GetBufferSize(), D3D_BLOB_ROOT_SIGNATURE, 0, &sig);
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nコンピュートルートシグネチャ情報の取得：失敗\n"));
		return hr;
	}

	hr = dev.lock()->Get()->CreateRootSignature(0, sig->GetBufferPointer(), sig->GetBufferSize(), IID_PPV_ARGS(&root));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nコンピュートルートシグネチャの生成：失敗\n"));
	}

	Release(sig);

	return hr;
}

// パイプラインの生成
long Effector::CreatePipe(void)
{
	D3D12_COMPUTE_PIPELINE_STATE_DESC desc{};
	desc.CS.pShaderBytecode = shader->GetBufferPointer();
	desc.CS.BytecodeLength  = shader->GetBufferSize();
	desc.Flags              = D3D12_PIPELINE_STATE_FLAGS::D3D12_PIPELINE_STATE_FLAG_NONE;
	desc.NodeMask           = 0;
	desc.pRootSignature     = root;

	auto hr = dev.lock()->Get()->CreateComputePipelineState(&desc, IID_PPV_ARGS(&pipe));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nパイプラインの生成：失敗\n"));
	}

	return hr;
}

// ヒープの生成
long Effector::CreateHeap(void)
{
	D3D12_DESCRIPTOR_HEAP_DESC desc{};
	desc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.NodeMask       = 0;
	desc.NumDescriptors = RSC_MAX;
	desc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	auto hr = dev.lock()->Get()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&heap));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nエフェクト用ヒープの生成：失敗\n"));
	}

	return hr;
}

// リソースの生成
long Effector::CreateCbvRsc(const std::string & name, const unsigned int & size)
{
	D3D12_HEAP_PROPERTIES prop{};
	prop.CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	prop.CreationNodeMask     = 1;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
	prop.Type                 = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD;
	prop.VisibleNodeMask      = 1;

	D3D12_RESOURCE_DESC desc{};
	desc.Alignment        = 0;
	desc.DepthOrArraySize = 1;
	desc.Dimension        = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Flags            = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;
	desc.Format           = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	desc.Height           = 1;
	desc.Layout           = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	desc.MipLevels        = 1;
	desc.SampleDesc       = { 1, 0 };
	desc.Width            = (size + 0xff) &~0xff;

	auto hr = dev.lock()->Get()->CreateCommittedResource(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,
		&desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&info[name].rsc));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nエフェクト用CBVリソースの生成：失敗\n"));
		return hr;
	}

	info[name].index = index++;

	return hr;
}

// UAVリソースの生成
long Effector::CreateUavRsc(const std::string & name, const unsigned int & size)
{
	D3D12_HEAP_PROPERTIES prop{};
	prop.CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	prop.CreationNodeMask     = 1;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_L0;
	prop.Type                 = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_CUSTOM;
	prop.VisibleNodeMask      = 1;

	D3D12_RESOURCE_DESC desc{};
	desc.Alignment        = 0;
	desc.DepthOrArraySize = 1;
	desc.Dimension        = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Flags            = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
	desc.Format           = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	desc.Height           = 1;
	desc.Layout           = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	desc.MipLevels        = 1;
	desc.SampleDesc       = { 1, 0 };
	desc.Width            = (size + 0xff) &~0xff;

	auto hr = dev.lock()->Get()->CreateCommittedResource(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,
		&desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS, nullptr, IID_PPV_ARGS(&info[name].rsc));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nエフェクト用UAVリソースの生成：失敗\n"));
		return hr;
	}

	info[name].index = index++;

	return hr;
}

// CBVの生成
void Effector::CreateConstantView(const std::string & name, const unsigned int & size)
{
	D3D12_CONSTANT_BUFFER_VIEW_DESC desc{};
	desc.BufferLocation = info[name].rsc->GetGPUVirtualAddress();
	desc.SizeInBytes    = (size + 0xff) &~0xff;

	auto handle = heap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += info[name].index * dev.lock()->Get()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	dev.lock()->Get()->CreateConstantBufferView(&desc, handle);
}

// UAVの生成
void Effector::CreateUnorderView(const std::string & name, const unsigned int & stride, const unsigned int & num)
{
	D3D12_UNORDERED_ACCESS_VIEW_DESC desc{};
	desc.ViewDimension              = D3D12_UAV_DIMENSION::D3D12_UAV_DIMENSION_BUFFER;
	desc.Format                     = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	desc.Buffer.NumElements         = num;
	desc.Buffer.StructureByteStride = stride;

	auto handle = heap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += info[name].index * dev.lock()->Get()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	
	dev.lock()->Get()->CreateUnorderedAccessView(info[name].rsc, nullptr, &desc, handle);
}

// マップ
long Effector::Map(const std::string & name)
{
	D3D12_RANGE range{ 0, 1 };

	auto hr = info[name].rsc->Map(0, &range, reinterpret_cast<void**>(&info[name].data));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nエフェクト用リソースのマップ：失敗\n"));
	}

	return hr;
}

// CBVの生成
void Effector::CBV(const std::string & name, const unsigned int & size)
{
	CreateCbvRsc(name, size);
	CreateConstantView(name, size);
	Map(name);
}

// UAVの生成
void Effector::UAV(const std::string & name, const unsigned int & stride, const unsigned int & num)
{
	CreateUavRsc(name, stride * num);
	CreateUnorderView(name, stride, num);
	Map(name);
}

// 初期化
void Effector::Init(void)
{
	CreatePipe();

	CreateHeap();

	CBV("b0", sizeof(Param));
	UAV("u0", sizeof(float), DATA_MAX);
	UAV("u1", sizeof(float), DATA_MAX);
}

#include <DirectXMath.h>

// 逆離散フーリエ変換
void IDFT(const std::vector<float>& real, const std::vector<float>&imag)
{
	//円周率
	const float p = 3.14159265f;
	auto size = real.size();

	auto hanning = [&](const int& index, const int& size)->float {
		float tmp = 0.0f;

		tmp = (size % 2 == 0) ?
			//偶数
			0.5f - 0.5f * cosf(2.0f * p * (float)index / (float)size) :
			//奇数
			0.5f - 0.5f * cosf(2.0f * p * ((float)index + 0.5f) / (float)size);

		if (tmp == 0.0f)
		{
			tmp = 1.0f;
		}

		return tmp;
	};

	DirectX::XMFLOAT2 tmp;

	//元の波形データ
	std::vector<float>reReal(size, 0.0f);
	//無視
	std::vector<float>reImag(size, 0.0f);
	for (int i = 0; i < size; ++i)
	{
		for (int n = 0; n < size; ++n)
		{
			tmp.x = cosf(2.0f * p * i * n / size);
			tmp.y = sinf(2.0f * p * i * n / size);

			reReal[i] += (tmp.x * real[n] - tmp.y * imag[n]) / size;
			reImag[i] += (tmp.x * imag[n] + tmp.y * real[n]) / size;
		}

		reReal[i] /= hanning(i, size);
		reImag[i] /= hanning(i, size);
	}

	for (int i = 0; i < size; ++i)
	{
		printf("%f %f\n", reReal[i], reImag[i]);
	}
}

#include <complex>

// 離散フーリエ変換
void DFT(void)
{
	//円周率
	const float p = 3.14159265f;
	//データサイズ
	int size = 64;

	//元データ
	std::vector<float>origin(size);
	for (int i = 0; i < size; ++i)
	{
		origin[i] = 0.5f * sinf(2.0f * p * 500.0f * i / 8000);
	}

	auto hanning = [&](const int& index, const int& size)->float {
		float tmp = 0.0f;

		tmp = (size % 2 == 0) ?
			//偶数
			0.5f - 0.5f * cosf(2.0f * p * (float)index / (float)size) :
			//奇数
			0.5f - 0.5f * cosf(2.0f * p * ((float)index + 0.5f) / (float)size);

		if (tmp == 0.0f)
		{
			tmp = 1.0f;
		}

		return tmp;
	};

	//実部
	std::vector<float>real(size, 0.0f);
	//虚部
	std::vector<float>imag(size, 0.0f);

	memcpy(real.data(), origin.data(), sizeof(float) * size);

	//ダミー
	DirectX::XMFLOAT2 tmp;

	for (int i = 0; i < size; ++i)
	{
		for (int n = 0; n < size; ++n)
		{
			tmp.x =  cosf(2.0f * p * i * n / size);
			tmp.y = -sinf(2.0f * p * i * n / size);

			real[i] += tmp.x * (origin[n] * 1) - tmp.y * 0.0f;
			imag[i] += tmp.x * 0.0f            + tmp.y * (origin[n] * 1);
		}
	}
	
	/* 周波数特性 */
	for (int i = 0; i < size; ++i)
	{
		//振幅スペクトル
		float a = sqrtf(real[i] * real[i] + imag[i] * imag[i]);
		//位相スペクトル
		float d = atanf((imag[i] / real[i]));
	
		printf("%f %f\n", a, d);
	}

	getchar();
}

// フェード
void Fade(void)
{
	//円周率
	const float p = 3.14159265f;
	//データサイズ
	int size = 200;

	//元データ
	std::vector<float>origin(size);
	for (int i = 0; i < size; ++i)
	{
		origin[i] = 0.25f * sinf(2.0f * p * 250.0f * i / 8000);
	}

	//適応データ
	auto s = origin;

	//0.01秒の単調増加・減少
	for (int i = 0; i < size * 0.01f; ++i)
	{
		//先頭から
		s[i] *= i / (size * 0.01f);
		//末尾から
		s[size - i - 1] *= i / (size * 0.01f);
	}

	origin = s;
}

// FIR_LPF
std::vector<float> FIR_LPF(const std::vector<float>&input)
{
	std::vector<float>out(input.size());

	//円周率
	const float p = 3.14159265f;
	// エッジ周波数
	float edge = 1000.0f / 44100.0f;
	//遷移帯域幅
	float delta = 1000.0f / 44100.0f;

	//遅延器の数
	int delayer = (3.1f / delta + 0.5f) - 1.0f;
	if (delayer % 2 != 0)
	{
		++delayer;
	}

	// フィルタ係数
	int filterCof = delayer + 1;

	auto hanning = [&](const int& index, const int& size)->float {
		float tmp = 0.0f;

		tmp = (size % 2 == 0) ?
			//偶数
			0.5f - 0.5f * cosf(2.0f * p * (float)index / (float)size) :
			//奇数
			0.5f - 0.5f * cosf(2.0f * p * ((float)index + 0.5f) / (float)size);

		if (tmp == 0.0f)
		{
			tmp = 1.0f;
		}

		return tmp;
	};

	std::vector<float>b(filterCof);

	for (int i = 0; i < input.size(); ++i)
	{
		for (int n = 0; n < filterCof; ++n)
		{
			if (i - n < 0)
			{
				continue;
			}

			out[i] += filterCof * input[i - n];
		}
	}

	return out;
}

// 実行
void Effector::Execution(const std::vector<float> & wave, std::vector<float> & adaptation, const unsigned int & index, const unsigned int & sample)
{
	//DFT();

	param.attenuation = 0.5f;
	param.time = 0.375f;
	param.loop = 10;
	param.index = index;
	param.sample = sample;

	memcpy(info["b0"].data, &param, sizeof(Param));
	memcpy(info["u0"].data, &wave[0], sizeof(float) * wave.size());
	memset(info["u1"].data, 0, sizeof(float) * wave.size());

	list->Reset();

	list->GetList()->SetComputeRootSignature(root);
	list->GetList()->SetPipelineState(pipe);

	list->GetList()->SetDescriptorHeaps(1, &heap);

	auto size = dev.lock()->Get()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	auto handle = heap->GetGPUDescriptorHandleForHeapStart();
	handle.ptr = heap->GetGPUDescriptorHandleForHeapStart().ptr + size * info["b0"].index;
	list->GetList()->SetComputeRootDescriptorTable(0, handle);
	handle.ptr = heap->GetGPUDescriptorHandleForHeapStart().ptr + size * info["u0"].index;
	list->GetList()->SetComputeRootDescriptorTable(1, handle);
	handle.ptr = heap->GetGPUDescriptorHandleForHeapStart().ptr + size * info["u1"].index;
	list->GetList()->SetComputeRootDescriptorTable(2, handle);

	list->GetList()->Dispatch(static_cast<unsigned int>(wave.size()), 1, 1);

	list->GetList()->Close();

	ID3D12CommandList* com[] = {
		list->GetList(),
	};

	queue->Get()->ExecuteCommandLists(1, com);

	fence->Wait();

	adaptation.assign(info["u1"].data, info["u1"].data + wave.size());
}
