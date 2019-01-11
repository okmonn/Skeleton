#include "List.h"
#include "../Device/Device.h"
#include "../etc/Release.h"

// �R���X�g���N�^
List::List(const D3D12_COMMAND_LIST_TYPE & type) : 
	allo(nullptr), list(nullptr)
{
	CreateAllo(type);
	CreateList(type);
}

// �f�X�g���N�^
List::~List()
{
	Release(list);
	Release(allo);
}

// �A���P�[�^�̐���
long List::CreateAllo(const D3D12_COMMAND_LIST_TYPE & type)
{
	auto hr = Device::Get().GetDev()->CreateCommandAllocator(type, IID_PPV_ARGS(&allo));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n�A���P�[�^�̐����F���s\n"));
	}

	return hr;
}

// ���X�g�̐���
long List::CreateList(const D3D12_COMMAND_LIST_TYPE & type)
{
	auto hr = Device::Get().GetDev()->CreateCommandList(0, type, allo, nullptr, IID_PPV_ARGS(&list));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n���X�g�̐����F���s\n"));
		return hr;
	}

	list->Close();

	return hr;
}

// �r���[�|�[�g�̃Z�b�g
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

// �V�U�[�̃Z�b�g
void List::SetScissor(const unsigned int & width, const unsigned int & height)
{
	RECT scissor{};
	scissor.bottom = static_cast<long>(height);
	scissor.left   = 0;
	scissor.right  = static_cast<long>(width);
	scissor.top    = 0;

	list->RSSetScissorRects(1, &scissor);
}
