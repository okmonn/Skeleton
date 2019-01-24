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
#include "../Texture/TexMane.h"
#include <Windows.h>
#include <algorithm>
#include <d3d12.h>
#include <dxgi1_6.h>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

// コンストラクタ
Application::Application(const Vec2 & winSize)
{
#ifdef _DEBUG
	ID3D12Debug* debug = nullptr;
	D3D12GetDebugInterface(IID_PPV_ARGS(&debug));
	debug->EnableDebugLayer();
#endif

	Init(winSize);
}

// コンストラクタ
Application::Application(const Application & app, const Vec2 & winSize)
{
	Init(winSize, app.win->Get());
}

// コンストラクタ
Application::Application(std::weak_ptr<Application> app, const Vec2 & winSize)
{
	Init(winSize, app.lock()->win->Get());
}

// デストラクタ
Application::~Application()
{
}

// ウィンドウサイズの取得
Vec2 Application::GetWinSize(void)
{
	return win->GetSize();
}

// マウスのクライアント座標の取得
Vec2 Application::GetMousePos(void)
{
	POINT point{};
	point.x = static_cast<long>(help::GetMousePos().x);
	point.y = static_cast<long>(help::GetMousePos().y);
	ScreenToClient(reinterpret_cast<HWND>(win->Get()), &point);

	return { static_cast<int>(point.x), static_cast<int>(point.y) };
}

// ドロップされたファイルのパス取得
std::string Application::GetDropFilePath(void)
{
	return win->GetDropFilePath();
}

// ルートの生成
void Application::CreateRoot(void)
{
	RootMane::Get().Create("primitive", L"Shader/Primitive.hlsl");
	RootMane::Get().Create("texture",   L"Shader/Texture.hlsl");
}

// 初期化
void Application::Init(const Vec2 & winSize, void * parent)
{
	win    = std::make_shared<Window>(winSize, parent);
	queue  = std::make_shared<Queue>(D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT);
	list   = std::make_shared<List>(D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT);
	fence  = std::make_unique<Fence>(queue);
	swap   = std::make_shared<Swap>(win, queue);
	render = std::make_unique<Render>(swap);
	depth  = std::make_unique<Depth>(win);

	CreateRoot();

	primitive = std::make_unique<PrimitiveMane>(win, RootMane::Get().GetRoot("primitive"));
	texture   = std::make_unique<TexMane>(win, RootMane::Get().GetRoot("texture"));
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

// ポイントの描画
void Application::DrawPoint(const Vec2f & pos, const Color & color)
{
	std::vector<prm::Vertex>vertex;
	vertex.push_back({ {pos.x, pos.y, 0.0f}, {color.r, color.g, color.b, color.a} });
	primitive->Draw(list, vertex, prm::PrimitiveType::point, D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
}

// ポイントの複数描画
void Application::DrawMultiPoint(const std::vector<Vec2f>& pos, const Color & color)
{
	std::vector<prm::Vertex>vertex(pos.size());
	unsigned int index = 0;
	std::for_each(vertex.begin(), vertex.end(), [&](prm::Vertex& vertex)->void {
		vertex = { {pos[index].x, pos[index].y, 0.0f}, {color.r, color.g, color.b, color.a} };
		++index;
	});
	primitive->Draw(list, vertex, prm::PrimitiveType::point, D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
}

// ラインの描画
void Application::DrawLine(const Vec2f & pos1, const Vec2f & pos2, const Color & color)
{
	std::vector<prm::Vertex>vertex;
	vertex.push_back({ {pos1.x, pos1.y, 0.0f}, {color.r, color.g, color.b, color.a} });
	vertex.push_back({ {pos2.x, pos2.y, 0.0f}, {color.r, color.g, color.b, color.a} });
	primitive->Draw(list, vertex, prm::PrimitiveType::line, D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_LINELIST);
}

// ラインの複数描画
void Application::DrawMultiLine(const std::vector<Vec2f>& pos1, const std::vector<Vec2f> & pos2, const Color & color)
{
	std::vector<prm::Vertex>vertex(pos1.size() + pos2.size());
	unsigned int index = 0;
	for (unsigned int i = 0; i < vertex.size(); i += 2)
	{
		vertex[i]     = { {pos1[index].x, pos1[index].y, 0.0f}, {color.r, color.g, color.b, color.a} };
		vertex[i + 1] = { {pos2[index].x, pos2[index].y, 0.0f}, {color.r, color.g, color.b, color.a} };
		++index;
	}
	primitive->Draw(list, vertex, prm::PrimitiveType::line, D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_LINELIST);
}

// トライアングルの描画
void Application::DrawTriangle(const Vec2f & pos1, const Vec2f & pos2, const Vec2f & pos3, const Color & color)
{
	std::vector<prm::Vertex>vertex;
	vertex.push_back({ {pos1.x, pos1.y, 0.0f}, {color.r, color.g, color.b, color.a} });
	vertex.push_back({ {pos2.x, pos2.y, 0.0f}, {color.r, color.g, color.b, color.a} });
	vertex.push_back({ {pos3.x, pos3.y, 0.0f}, {color.r, color.g, color.b, color.a} });
	primitive->Draw(list, vertex, prm::PrimitiveType::triangle, D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

// トライアングルの複数描画
void Application::DrawMultiTriangle(const std::vector<Vec2f> & pos1, const std::vector<Vec2f> & pos2, const std::vector<Vec2f> & pos3, const Color& color)
{
	std::vector<prm::Vertex>vertex(pos1.size() + pos2.size() + pos3.size());
	unsigned int index = 0;
	for (unsigned int i = 0; i < vertex.size(); i += 3)
	{
		vertex[i]     = { {pos1[index].x, pos1[index].y, 0.0f}, {color.r, color.g, color.b, color.a} };
		vertex[i + 1] = { {pos2[index].x, pos2[index].y, 0.0f}, {color.r, color.g, color.b, color.a} };
		vertex[i + 2] = { {pos3[index].x, pos3[index].y, 0.0f}, {color.r, color.g, color.b, color.a} };
		++index;
	}
	primitive->Draw(list, vertex, prm::PrimitiveType::triangle, D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

// ボックスの描画
void Application::DrawBox(const Vec2f & pos, const Vec2f & size, const Color & color)
{
	std::vector<prm::Vertex>vertex;
	vertex.push_back({ {pos.x, pos.y, 0.0f},                   {color.r, color.g, color.b, color.a} });
	vertex.push_back({ {pos.x + size.x, pos.y, 0.0f},          {color.r, color.g, color.b, color.a} });
	vertex.push_back({ {pos.x, pos.y + size.y, 0.0f},          {color.r, color.g, color.b, color.a} });
	vertex.push_back({ {pos.x + size.x, pos.y + size.y, 0.0f}, {color.r, color.g, color.b, color.a} });
	primitive->Draw(list, vertex, prm::PrimitiveType::triangle, D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}

// 画像の読み込み
void Application::LoadTex(int & addr, const std::string & fileName)
{
	texture->Load(addr, fileName);
}

// 画像の描画
void Application::DrawTex(int & addr, const Vec2f & pos, const Vec2f & size, const Vec2f & uvPos, const Vec2f & uvSize, 
	const float & alpha, const float & angle, const bool & turnX, const bool & turnY)
{
	texture->Draw(list, addr, { pos.x, pos.y }, { size.x, size.y }, { uvPos.x, uvPos.y }, { uvSize.x, uvSize.y }, alpha, angle, turnX, turnY);
}

// 画面クリア
void Application::Clear(void)
{
	list->Reset();

	list->SetView(win->GetSize());
	list->SetScissor(win->GetSize());

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

	primitive->Clear();
}

// 画像の削除
void Application::DeleteTex(int & addr)
{
	texture->Delete(addr);
}

// 終了
void Application::End(void)
{
	texture.reset();
	primitive.reset();
	depth.reset();
	render.reset();
	swap.reset();
	fence.reset();
	list.reset();
	queue.reset();
	win.reset();
}
