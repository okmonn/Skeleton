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
#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

// コンストラクタ
Application::Application()
{
#ifdef _DEBUG
	ID3D12Debug* debug = nullptr;
	D3D12GetDebugInterface(IID_PPV_ARGS(&debug));
	debug->EnableDebugLayer();
#endif

	Create();
}

// デストラクタ
Application::~Application()
{
}

// クラスの生成
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
}

// ルートの生成
void Application::CreateRoot(void)
{
	RootMane::Get().Create("primitive", L"Shader/Primitive.hlsl");
}

// パイプの生成
void Application::CreatePipe(void)
{
	PipeMane::Get().CreatePipe("point", RootMane::Get().GetRoot("primitive"), D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT,
		{ 0, 3 }, false);
}

// メッセージの確認
bool Application::CheckMsg(void)
{
	static MSG msg{};

	if (msg.message == WM_QUIT)
	{
		return false;
	}

	//呼び出し側スレッドが所有しているウィンドウに送信されたメッセージの保留されている物を取得
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		//仮想キーメッセージを文字メッセージに変換
		TranslateMessage(&msg);
		//1つのウィドウプロシージャにメッセージを送出する
		DispatchMessage(&msg);
	}

	return true;
}

// キー入力
bool Application::CheckKey(const int & key)
{
	if (GetKeyState(key) & 0x80)
	{
		return true;
	}

	return false;
}

// 画面クリア
void Application::Clear(void)
{
	list->Reset();

	list->SetView(win->GetWidth(), win->GetHeight());
	list->SetScissor(win->GetWidth(), win->GetHeight());

	list->Barrier(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET, render->GetRsc());

	render->Clear(list, depth->GetHeap());
	depth->Clear(list);
}

// コマンドの実行
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
}
