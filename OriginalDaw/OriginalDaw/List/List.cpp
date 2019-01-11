#include "List.h"
#include "../Device/Device.h"
#include "../etc/Release.h"

// コンストラクタ
List::List(const D3D12_COMMAND_LIST_TYPE & type) : 
	allo(nullptr), list(nullptr)
{
	CreateAllo(type);
	CreateList(type);
}

// デストラクタ
List::~List()
{
	Release(list);
	Release(allo);
}

// アロケータの生成
long List::CreateAllo(const D3D12_COMMAND_LIST_TYPE & type)
{
	auto hr = Device::Get().GetDev()->CreateCommandAllocator(type, IID_PPV_ARGS(&allo));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nアロケータの生成：失敗\n"));
	}

	return hr;
}

// リストの生成
long List::CreateList(const D3D12_COMMAND_LIST_TYPE & type)
{
	auto hr = Device::Get().GetDev()->CreateCommandList(0, type, allo, nullptr, IID_PPV_ARGS(&list));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nリストの生成：失敗\n"));
		return hr;
	}

	list->Close();

	return hr;
}

// ビューポートのセット
void List::SetView(const unsigned int & width, const unsigned int & height)
{
	D3D12_VIEWPORT view{};
	view.Height   = static_cast<float>(height);
	view.MaxDepth = 1.0f;
	view.MinDepth = 0.0f;
	view.TopLeftX = 0.0f;
	view.TopLeftY = 0.0f;
	view.Width    = static_cast<float>(width);

	list->RSSetViewports(1, &view);
}

// シザーのセット
void List::SetScissor(const unsigned int & width, const unsigned int & height)
{
	RECT scissor{};
	scissor.bottom = static_cast<long>(height);
	scissor.left   = 0;
	scissor.right  = static_cast<long>(width);
	scissor.top    = 0;

	list->RSSetScissorRects(1, &scissor);
}
