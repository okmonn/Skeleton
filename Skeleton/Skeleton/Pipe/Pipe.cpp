#include "Pipe.h"
#include "../Device/Device.h"
#include "../Swap/Swap.h"
#include "../Root/Root.h"
#include "../etc/Release.h"
#include <dxgi1_6.h>

// コンストラクタ
Pipe::Pipe(std::weak_ptr<Device>dev, std::weak_ptr<Swap>swap, std::weak_ptr<Root>root) :
	dev(dev), swap(swap), root(root), pipe(nullptr)
{
}

// デストラクタ
Pipe::~Pipe()
{
	Release(pipe);
}

// パイプラインの生成
long Pipe::Create(const D3D12_INPUT_ELEMENT_DESC & input, const unsigned int & num, const D3D12_PRIMITIVE_TOPOLOGY_TYPE & type, const bool & depth)
{
	D3D12_RASTERIZER_DESC rasterizer{};
	rasterizer.AntialiasedLineEnable = FALSE;
	rasterizer.ConservativeRaster    = D3D12_CONSERVATIVE_RASTERIZATION_MODE::D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
	rasterizer.CullMode              = D3D12_CULL_MODE::D3D12_CULL_MODE_NONE;
	rasterizer.DepthBias             = D3D12_DEFAULT_DEPTH_BIAS;
	rasterizer.DepthBiasClamp        = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
	rasterizer.DepthClipEnable       = true;
	rasterizer.FillMode              = D3D12_FILL_MODE::D3D12_FILL_MODE_SOLID;
	rasterizer.ForcedSampleCount     = 0;
	rasterizer.FrontCounterClockwise = false;
	rasterizer.MultisampleEnable     = false;
	rasterizer.SlopeScaledDepthBias  = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;

	D3D12_RENDER_TARGET_BLEND_DESC renderBlend{};
	renderBlend.BlendEnable           = true;
	renderBlend.BlendOp               = D3D12_BLEND_OP::D3D12_BLEND_OP_ADD;
	renderBlend.BlendOpAlpha          = D3D12_BLEND_OP::D3D12_BLEND_OP_ADD;
	renderBlend.DestBlend             = D3D12_BLEND::D3D12_BLEND_INV_SRC_ALPHA;
	renderBlend.DestBlendAlpha        = D3D12_BLEND::D3D12_BLEND_ZERO;
	renderBlend.LogicOp               = D3D12_LOGIC_OP::D3D12_LOGIC_OP_NOOP;
	renderBlend.LogicOpEnable         = false;
	renderBlend.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE::D3D12_COLOR_WRITE_ENABLE_ALL;
	renderBlend.SrcBlend              = D3D12_BLEND::D3D12_BLEND_SRC_ALPHA;
	renderBlend.SrcBlendAlpha         = D3D12_BLEND::D3D12_BLEND_ONE;

	D3D12_BLEND_DESC blend{};
	blend.AlphaToCoverageEnable  = false;
	blend.IndependentBlendEnable = false;
	DXGI_SWAP_CHAIN_DESC1 tmp{};
	swap.lock()->Get()->GetDesc1(&tmp);
	for (UINT i = 0; i < tmp.BufferCount; i++)
	{
		blend.RenderTarget[i] = renderBlend;
	}

	D3D12_GRAPHICS_PIPELINE_STATE_DESC desc{};
	desc.BlendState                       = blend;
	desc.DepthStencilState.DepthEnable    = depth;
	desc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK::D3D12_DEPTH_WRITE_MASK_ALL;
	desc.DepthStencilState.DepthFunc      = D3D12_COMPARISON_FUNC::D3D12_COMPARISON_FUNC_LESS;
	desc.DepthStencilState.StencilEnable  = FALSE;
	desc.DSVFormat                        = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
	desc.InputLayout                      = { &input, num };
	desc.NumRenderTargets                 = 1;
	desc.PrimitiveTopologyType            = type;
	desc.pRootSignature                   = root.lock()->Get();
	desc.PS.pShaderBytecode               = root.lock()->GetPixel()->GetBufferPointer();
	desc.PS.BytecodeLength                = root.lock()->GetPixel()->GetBufferSize();
	desc.RasterizerState                  = rasterizer;
	desc.RTVFormats[0]                    = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleMask                       = UINT_MAX;
	desc.SampleDesc                       = { 1, 0 };
	desc.VS.pShaderBytecode               = root.lock()->GetVertex()->GetBufferPointer();
	desc.VS.BytecodeLength                = root.lock()->GetVertex()->GetBufferSize();

	auto hr = dev.lock()->Get()->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&pipe));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nパイプラインの生成：失敗\n"));
	}

	return hr;
}
