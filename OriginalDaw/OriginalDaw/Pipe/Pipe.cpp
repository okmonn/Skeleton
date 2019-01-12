#include "Pipe.h"
#include "../Device/Device.h"
#include "../Root/Root.h"
#include "../etc/Release.h"
#include <dxgi1_6.h>

// バックバッファの数
#define BACK_BUFFER 2

// コンストラクタ
Pipe::Pipe(std::weak_ptr<Root>root) : root(root),
	pipe(nullptr)
{
}

// デストラクタ
Pipe::~Pipe()
{
	Release(pipe);
}

// パイプラインの生成
long Pipe::CreateGraphics(const D3D12_INPUT_ELEMENT_DESC & input, const size_t & num, const D3D12_PRIMITIVE_TOPOLOGY_TYPE & type, const bool & depth)
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
	for (unsigned int i = 0; i < BACK_BUFFER; i++)
	{
		blend.RenderTarget[i]    = renderBlend;
	}

	D3D12_GRAPHICS_PIPELINE_STATE_DESC desc{};
	desc.BlendState = blend;
	desc.DepthStencilState.DepthEnable                  = depth;
	desc.DepthStencilState.DepthWriteMask               = D3D12_DEPTH_WRITE_MASK::D3D12_DEPTH_WRITE_MASK_ALL;
	desc.DepthStencilState.DepthFunc                    = D3D12_COMPARISON_FUNC::D3D12_COMPARISON_FUNC_LESS;
	desc.DepthStencilState.StencilEnable                = depth;
	desc.DepthStencilState.StencilReadMask              = 0xff;
	desc.DepthStencilState.StencilWriteMask             = 0xff;
	desc.DepthStencilState.FrontFace.StencilFailOp      = D3D12_STENCIL_OP::D3D12_STENCIL_OP_KEEP;
	desc.DepthStencilState.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP::D3D12_STENCIL_OP_KEEP;
	desc.DepthStencilState.FrontFace.StencilFunc        = D3D12_COMPARISON_FUNC::D3D12_COMPARISON_FUNC_ALWAYS;
	desc.DepthStencilState.FrontFace.StencilPassOp      = D3D12_STENCIL_OP::D3D12_STENCIL_OP_INCR;
	desc.DepthStencilState.BackFace.StencilFailOp       = D3D12_STENCIL_OP::D3D12_STENCIL_OP_KEEP;
	desc.DepthStencilState.BackFace.StencilDepthFailOp  = D3D12_STENCIL_OP::D3D12_STENCIL_OP_KEEP;
	desc.DepthStencilState.BackFace.StencilPassOp       = D3D12_STENCIL_OP::D3D12_STENCIL_OP_DECR;
	desc.DepthStencilState.BackFace.StencilFunc         = D3D12_COMPARISON_FUNC::D3D12_COMPARISON_FUNC_ALWAYS;
	desc.DSVFormat                                      = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
	if (root.lock()->GetGeometry() != nullptr)
	{
		desc.GS.pShaderBytecode                         = root.lock()->GetGeometry()->GetBufferPointer();
		desc.GS.BytecodeLength                          = root.lock()->GetGeometry()->GetBufferSize();
	}
	desc.InputLayout                                    = { &input, static_cast<unsigned int>(num) };
	desc.NumRenderTargets                               = 1;
	desc.PrimitiveTopologyType                          = type;
	desc.pRootSignature                                 = root.lock()->Get();
	desc.PS.pShaderBytecode                             = root.lock()->GetPixel()->GetBufferPointer();
	desc.PS.BytecodeLength                              = root.lock()->GetPixel()->GetBufferSize();
	desc.RasterizerState                                = rasterizer;
	desc.RTVFormats[0]                                  = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleMask                                     = UINT_MAX;
	desc.SampleDesc                                     = { 1, 0 };
	desc.VS.pShaderBytecode                             = root.lock()->GetVertex()->GetBufferPointer();
	desc.VS.BytecodeLength                              = root.lock()->GetVertex()->GetBufferSize();

	auto hr = Device::Get().GetDev()->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&pipe));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n描画用パイプラインの生成：失敗\n"));
	}

	return hr;
}

// 計算用パイプラインの生成
long Pipe::CreateCompute(void)
{
	D3D12_COMPUTE_PIPELINE_STATE_DESC desc{};
	desc.CS.pShaderBytecode = root.lock()->GetCompute()->GetBufferPointer();
	desc.CS.BytecodeLength  = root.lock()->GetCompute()->GetBufferSize();
	desc.Flags              = D3D12_PIPELINE_STATE_FLAGS::D3D12_PIPELINE_STATE_FLAG_NONE;
	desc.NodeMask           = 0;
	desc.pRootSignature     = root.lock()->Get();

	auto hr = Device::Get().GetDev()->CreateComputePipelineState(&desc, IID_PPV_ARGS(&pipe));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n計算用パイプラインの生成：失敗\n"));
	}

	return hr;
}
