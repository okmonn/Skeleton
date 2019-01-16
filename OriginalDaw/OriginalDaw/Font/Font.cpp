#include "Font.h"
#include "../DescriptorMane/DescriptorMane.h"
#include "../Window/Window.h"
#include "../List/List.h"
#include "../Root/Root.h"
#include "../Pipe/Pipe.h"
#include "../Helper/Helper.h"
#include "../etc/Release.h"
#include <Windows.h>

// リソース数
#define RSC_MAX 1

// 頂点数
#define VERTEX_MAX 4

const MAT2 mat = { {0,1}, {0,0}, {0,0}, {0,1} };

// コンストラクタ
Font::Font(std::weak_ptr<Window> win,  void * handle, const std::string & code) : win(win),
	srv(0), rsc(0)
{
	vertex.resize(VERTEX_MAX);

	GetInfo(handle, code);
}

// デストラクタ
Font::~Font()
{
	DescriptorMane::Get().DeleteRsc(srv);
	DescriptorMane::Get().DeleteRsc(rsc);
	DescriptorMane::Get().DeleteHeap(srv);
}

// SRVの生成
void Font::SRV(const TEXTMETRIC & tm, const GLYPHMETRICS & gm)
{
	DescriptorMane::Get().CreateHeap(srv, D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,
		RSC_MAX, D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	D3D12_HEAP_PROPERTIES prop{};
	prop.CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	prop.CreationNodeMask     = 0;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_L0;
	prop.Type                 = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_CUSTOM;
	prop.VisibleNodeMask      = 0;

	D3D12_RESOURCE_DESC desc{};
	desc.Alignment = 0;
	desc.DepthOrArraySize = 1;
	desc.Dimension  = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	desc.Flags      = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;
	desc.Format     = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.Height     = static_cast<unsigned int>(gm.gmBlackBoxY);
	desc.Layout     = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_UNKNOWN;
	desc.MipLevels  = 1;
	desc.SampleDesc = { 1, 0 };
	desc.Width      = static_cast<unsigned __int64>(gm.gmBlackBoxX);

	DescriptorMane::Get().CreateRsc(srv, prop, desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ);
	DescriptorMane::Get().SRV(srv, srv);
}

// サブリソースに書き込み
long Font::WriteSub(const std::vector<unsigned char>& data)
{
	D3D12_BOX box{};
	box.back   = 1;
	box.bottom = DescriptorMane::Get().GetRsc(srv)->GetDesc().Height;
	box.front  = 0;
	box.left   = 0;
	box.right  = static_cast<unsigned int>(DescriptorMane::Get().GetRsc(srv)->GetDesc().Width);
	box.top    = 0;

	auto hr = DescriptorMane::Get().GetRsc(srv)->WriteToSubresource(0, &box, data.data(), box.right * 4, box.bottom * 4);
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nシェーダリソースのサブへの書き込み：失敗"));
	}

	return hr;
}

// 頂点バッファの生成
void Font::VertexBuffer(void)
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
	desc.Width            = sizeof(font::Vertex) * vertex.size();

	DescriptorMane::Get().CreateRsc(rsc, prop, desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ);
}

// フォント情報の取得
void Font::GetInfo(void * handle, const std::string & code)
{
	HDC hdc = GetDC(nullptr);
	HFONT oldFont = (HFONT)SelectObject(hdc, handle);

#if _UNICODE
	unsigned int tmp = (unsigned int)*help::ChangeWString(code).c_str();
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
	std::vector<unsigned char>data(size, 0);
	GetGlyphOutline(hdc, tmp, GGO_GRAY4_BITMAP, &gm, size, data.data(), &mat);

	SelectObject(hdc, oldFont);
	ReleaseDC(nullptr, hdc);

	SRV(tm, gm);
	WriteSub(data);
	VertexBuffer();
}

// マップ
long Font::Map(void)
{
	void* data = nullptr;

	auto hr = DescriptorMane::Get().GetRsc(rsc)->Map(0, nullptr, &data);
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nフォント用頂点リソースのマップ：失敗\n"));
		return hr;
	}

	memcpy(data, vertex.data(), sizeof(font::Vertex) * vertex.size());

	DescriptorMane::Get().GetRsc(rsc)->Unmap(0, nullptr);

	return hr;
}

// 描画
void Font::Draw(std::weak_ptr<List> list, const DirectX::XMFLOAT2 & pos, const DirectX::XMFLOAT2 & size)
{
	vertex[0] = { {pos.x, pos.y, 0.0f} };
	vertex[1] = { {pos.x + size.x, pos.y, 0.0f} };
	vertex[2] = { {pos.x, pos.y + size.y, 0.0f} };
	vertex[3] = { {pos.x + size.x, pos.y + size.y, 0.0f} };
	Map();

	list.lock()->SetRoot(root.lock()->Get());
	list.lock()->SetPipe(pipe.lock()->Get());

	D3D12_VERTEX_BUFFER_VIEW view{};
	view.BufferLocation = DescriptorMane::Get().GetRsc(rsc)->GetGPUVirtualAddress();
	view.SizeInBytes = static_cast<unsigned int>(DescriptorMane::Get().GetRsc(rsc)->GetDesc().Width);
	view.StrideInBytes = sizeof(font::Vertex);
	list.lock()->SetVertexBufferView(view);

	auto heap = DescriptorMane::Get().GetHeap(srv);
	list.lock()->SetHeap(&heap, 1);
	list.lock()->SetRootTable(0, heap);

	heap = win.lock()->GetHeap();
	list.lock()->SetHeap(&heap, 1);
	list.lock()->SetRootTable(1, heap);

	list.lock()->SetTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	list.lock()->DrawVertex(this->vertex.size());
}
