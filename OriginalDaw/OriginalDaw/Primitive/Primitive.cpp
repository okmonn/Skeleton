#include "Primitive.h"
#include "../DescriptorMane/DescriptorMane.h"
#include "../etc/Release.h"

// コンストラクタ
Primitive::Primitive(const prm::PrimitiveType & type) : type(type), 
	rsc(0)
{
	VertexBuffer();
}

// デストラクタ
Primitive::~Primitive()
{
	DescriptorMane::Get().DeleteRsc(rsc);
}

// 頂点バッファの生成
void Primitive::VertexBuffer(void)
{
	D3D12_HEAP_PROPERTIES prop{};
	prop.CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	prop.CreationNodeMask     = 0;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
	prop.Type                 = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD;
	prop.VisibleNodeMask      = 0;

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
	desc.Width            = sizeof(prm::Vertex) * static_cast<unsigned __int64>(type);

	DescriptorMane::Get().CreateRsc(rsc, prop, desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ);
}

// リソースの取得
ID3D12Resource * Primitive::GetRsc(void)
{
	return DescriptorMane::Get().GetRsc(rsc);
}
