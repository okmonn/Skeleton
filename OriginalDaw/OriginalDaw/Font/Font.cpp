#include "Font.h"
#include "../DescriptorMane/DescriptorMane.h"
#include <vector>
#include <Windows.h>
#include "../etc/Release.h"

// リソース数
#define RSC_MAX 1

const MAT2 mat = { {0,1}, {0,0}, {0,0}, {0,1} };

// コンストラクタ
Font::Font(std::weak_ptr<Root> root, std::weak_ptr<Pipe> pipe, const std::tstring & code) : 
	srv(0), rsc(0)
{
}

// デストラクタ
Font::~Font()
{
	DescriptorMane::Get().DeleteRsc(srv);
	DescriptorMane::Get().DeleteRsc(rsc);
	DescriptorMane::Get().DeleteHeap(srv);
}

// フォント情報の取得
void Font::GetInfo(void * font, const std::tstring & code)
{
	HDC hdc = GetDC(nullptr);
	HFONT oldFont = (HFONT)SelectObject(hdc, font);

#if _UNICODE
	unsigned int tmp = (unsigned int)*code.c_str();
#else
	unsigned int tmp = 0;
	if (IsDBCSLeadByte(*code.c_str()))
		tmp = (BYTE)code[0] << 8 | (BYTE)code[1];
	else
		tmp = code[0];
#endif

	TEXTMETRIC tm{};
	GetTextMetrics(hdc, &tm);
	GLYPHMETRICS gm{};
	auto size = GetGlyphOutline(hdc, tmp, GGO_GRAY4_BITMAP, &gm, 0, NULL, &mat);
	std::vector<unsigned char>ptr(size, 0);
	GetGlyphOutline(hdc, tmp, GGO_GRAY4_BITMAP, &gm, size, ptr.data(), &mat);

	SelectObject(hdc, oldFont);
	ReleaseDC(nullptr, hdc);

	DescriptorMane::Get().CreateHeap(srv, D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE, 
		RSC_MAX, D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	{
		D3D12_HEAP_PROPERTIES prop{};
		prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
		prop.CreationNodeMask = 0;
		prop.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_L0;
		prop.Type = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_CUSTOM;
		prop.VisibleNodeMask = 0;

		D3D12_RESOURCE_DESC desc{};
		desc.Alignment = 0;
		desc.DepthOrArraySize = 1;
		desc.Dimension = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		desc.Flags = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;
		desc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.Height = tm.tmHeight;
		desc.Layout = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_UNKNOWN;
		desc.MipLevels = 1;
		desc.SampleDesc = { 1, 0 };
		desc.Width = gm.gmCellIncX;

		DescriptorMane::Get().CreateRsc(srv, prop, desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ);
		DescriptorMane::Get().SRV(srv, srv);
	}
}

// SRVの生成
void Font::SRV(void)
{
	
}
