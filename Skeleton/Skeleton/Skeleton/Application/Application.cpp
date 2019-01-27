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

// コンストラクタ
Application::Application(const Vec2 & size)
{
#ifdef _DEBUG
	ID3D12Debug* debug = nullptr;
	D3D12GetDebugInterface(IID_PPV_ARGS(&debug));
	debug->EnableDebugLayer();
#endif

	Instance(size);
}

// コンストラクタ
Application::Application(const Application & app, const Vec2 & size)
{
	Instance(size, app.win->Get());
}

// コンストラクタ
Application::Application(std::weak_ptr<Application> app, const Vec2 & size)
{
	Instance(size, app.lock()->win->Get());
}

// デストラクタ
Application::~Application()
{
}

// タイトルの変更
void Application::ChangeTitle(const std::string & title) const
{
	SetWindowTextA(reinterpret_cast<HWND>(win->Get()), title.c_str());
}

// クラスのインスタンス
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

// ウィンドウサイズの取得
Vec2 Application::GetWinSize(void) const
{
	return win->GetSize();
}

// マウス座標の取得
Vec2 Application::GetMousePos(void) const
{
	POINT point{};
	point.x = static_cast<long>(use::GetMousePos().x);
	point.y = static_cast<long>(use::GetMousePos().y);
	ScreenToClient(reinterpret_cast<HWND>(win->Get()), &point);

	return { static_cast<int>(point.x), static_cast<int>(point.y) };
}

// メッセージの確認
bool Application::CheckMag(void) const
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

// クリア
void Application::Clear(void) const
{
	list->Reset();

	list->Viewport(win->GetSize());
	list->Scissor(win->GetSize());

	list->Barrier(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET, render->GetRsc());

	render->Clear(list, depth);
	depth->Clear(list);
}

// 実行
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
