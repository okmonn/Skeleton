#include "Application.h"
#include "../Root/RootMane.h"
#include "../Pipe/PipeMane.h"
#include "../Window/Window.h"
#include "../Queue/Queue.h"
#include "../List/List.h"
#include "../Fence/Fence.h"
#include "../Swap/Swap.h"
#include "../Render/Render.h"
#include "../Depth/Depth.h"
#include "../Primitive/PrimitiveMane.h"
#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

// �R���X�g���N�^
Application::Application()
{
#ifdef _DEBUG
	ID3D12Debug* debug = nullptr;
	D3D12GetDebugInterface(IID_PPV_ARGS(&debug));
	debug->EnableDebugLayer();
#endif

	Create();
}

// �f�X�g���N�^
Application::~Application()
{
}

// �N���X�̐���
void Application::Create(void)
{
	win    = std::make_shared<Window>();
	queue  = std::make_shared<Queue>(D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT);
	list   = std::make_shared<List>(D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT);
	fence  = std::make_shared<Fence>(queue);
	swap   = std::make_shared<Swap>(win, queue);
	render = std::make_shared<Render>(swap);
	depth  = std::make_shared<Depth>(win);

	CreateRoot();
	CreatePipe();

	primitive = std::make_unique<PrimitiveMane>(win, RootMane::Get().GetRoot("primitive"));
}

// ���[�g�̐���
void Application::CreateRoot(void)
{
	RootMane::Get().Create("primitive", L"Shader/Primitive.hlsl");
}

// �p�C�v�̐���
void Application::CreatePipe(void)
{
	PipeMane::Get().CreatePipe("point", RootMane::Get().GetRoot("primitive"), D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT,
		{ 0, 3 }, false);
}

// ���b�Z�[�W�̊m�F
bool Application::CheckMsg(void)
{
	static MSG msg{};

	if (msg.message == WM_QUIT)
	{
		return false;
	}

	//�Ăяo�����X���b�h�����L���Ă���E�B���h�E�ɑ��M���ꂽ���b�Z�[�W�ۗ̕�����Ă��镨���擾
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		//���z�L�[���b�Z�[�W�𕶎����b�Z�[�W�ɕϊ�
		TranslateMessage(&msg);
		//1�̃E�B�h�E�v���V�[�W���Ƀ��b�Z�[�W�𑗏o����
		DispatchMessage(&msg);
	}

	return true;
}

// �L�[����
bool Application::CheckKey(const int & key)
{
	if (GetKeyState(key) & 0x80)
	{
		return true;
	}

	return false;
}

// �|�C���g�̕`��
void Application::DrawPoint(const Vec2f & pos, const Color & color)
{
	prm::Vertex v[] = {
		{ { pos.x, pos.y, 0.0f }, { color.r, color.g, color.b, color.a } },
	};
	primitive->Draw(list, prm::PrimitiveType::point, v, _countof(v));
}

// ���C���̕`��
void Application::DrawLine(const Vec2f & pos1, const Vec2f & pos2, const Color & color)
{
	prm::Vertex v[] = {
		{ { pos1.x, pos1.y, 0.0f }, { color.r, color.g, color.b, color.a } },
		{ { pos2.x, pos2.y, 0.0f }, { color.r, color.g, color.b, color.a } },
	};
	primitive->Draw(list, prm::PrimitiveType::line, v, _countof(v));
}

// �g���C�A���O���̕`��
void Application::DrawTriangle(const Vec2f & pos1, const Vec2f & pos2, const Vec2f & pos3, const Color & color)
{
	prm::Vertex v[] = {
		{ { pos1.x, pos1.y, 0.0f }, { color.r, color.g, color.b, color.a } },
		{ { pos2.x, pos2.y, 0.0f }, { color.r, color.g, color.b, color.a } },
		{ { pos3.x, pos3.y, 0.0f }, { color.r, color.g, color.b, color.a } },
	};
	primitive->Draw(list, prm::PrimitiveType::triangle, v, _countof(v));
}

// �{�b�N�X�̕`��
void Application::DrawBox(const Vec2f & pos, const Vec2f & size, const Color & color)
{
	DrawTriangle(pos, { pos.x + size.x, pos.y }, { pos.x, pos.y + size.y }, color);
	DrawTriangle({ pos.x + size.x, pos.y }, pos + size, { pos.x, pos.y + size.y }, color);
}

// ��ʃN���A
void Application::Clear(void)
{
	list->Reset();

	list->SetView(win->GetWidth(), win->GetHeight());
	list->SetScissor(win->GetWidth(), win->GetHeight());

	list->Barrier(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET, render->GetRsc());

	render->Clear(list, depth->GetHeap());
	depth->Clear(list);
}

// �R�}���h�̎��s
void Application::Execution(void)
{
	list->Barrier(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT, render->GetRsc());

	list->Close();
	
	ID3D12CommandList* com[] = {
		list->GetList(),
	};
	queue->Execution(com, _countof(com));

	swap->Present();

	fence->Wait();

	primitive->Clear();
}
