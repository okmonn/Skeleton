#include "Application.h"
#include "../Window/Window.h"
#include "../Queue/Queue.h"
#include "../List/List.h"
#include "../Fence/Fence.h"
#include "../Swap/Swap.h"
#include "../Render/Render.h"
#include "../Depth/Depth.h"
#include "../Release.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

// �R���X�g���N�^
Application::Application(const Vec2 & size)
{
#ifdef _DEBUG
	ID3D12Debug* debug = nullptr;
	D3D12GetDebugInterface(IID_PPV_ARGS(&debug));
	debug->EnableDebugLayer();
#endif

	Instance(size);
}

// �R���X�g���N�^
Application::Application(const Application & app, const Vec2 & size)
{
	Instance(size, app.win->Get());
}

// �R���X�g���N�^
Application::Application(std::weak_ptr<Application> app, const Vec2 & size)
{
	Instance(size, app.lock()->win->Get());
}

// �f�X�g���N�^
Application::~Application()
{
}

// �^�C�g���̕ύX
void Application::ChangeTitle(const std::string & title) const
{
	SetWindowTextA(reinterpret_cast<HWND>(win->Get()), title.c_str());
}

// �N���X�̃C���X�^���X
void Application::Instance(const Vec2 & size, void * parent)
{
	win    = std::make_shared<Window>(size, parent);
	queue  = std::make_shared<Queue>(D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT);
	list   = std::make_shared<List>(D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT);
	fence  = std::make_unique<Fence>(queue);
	swap   = std::make_shared<Swap>(win, queue);
	render = std::make_unique<Render>(swap);
	depth  = std::make_shared<Depth>(win);
}

// �E�B���h�E�T�C�Y�̎擾
Vec2 Application::GetWinSize(void) const
{
	return win->GetSize();
}

// �}�E�X���W�̎擾
Vec2 Application::GetMousePos(void) const
{
	POINT point{};
	point.x = static_cast<long>(use::GetMousePos().x);
	point.y = static_cast<long>(use::GetMousePos().y);
	ScreenToClient(reinterpret_cast<HWND>(win->Get()), &point);

	return { static_cast<int>(point.x), static_cast<int>(point.y) };
}

// ���b�Z�[�W�̊m�F
bool Application::CheckMag(void) const
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

// �N���A
void Application::Clear(void) const
{
	list->Reset();

	list->Viewport(win->GetSize());
	list->Scissor(win->GetSize());

	list->Barrier(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET, render->GetRsc());

	render->Clear(list, depth);
	depth->Clear(list);
}

// ���s
void Application::Execution(void) const
{
	list->Barrier(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT, render->GetRsc());


	list->Close();

	ID3D12CommandList* com[] = {
		list->GetList(),
	};
	queue->Execution(com, _countof(com));

	swap->Present();

	fence->Wait();
}
