#include "Application.h"
#include "../Window/Window.h"
#include "../Queue/Queue.h"
#include "../List/List.h"
#include "../Fence/Fence.h"
#include "../Swap/Swap.h"
#include "../Render/Render.h"
#include "../Depth/Depth.h"
#include "../Primitive/PrimitiveMane.h"
#include "../Texture/TexMane.h"
#include "../Release.h"
#include <algorithm>

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

// �h���b�v���ꂽ�t�@�C���p�X�̎擾
std::string Application::GetDropFilePass(void)
{
	return win->GetDropPass();
}

// �N���X�̃C���X�^���X
void Application::Instance(const Vec2 & size, void * parent)
{
	win       = std::make_shared<Window>(size, parent);
	queue     = std::make_shared<Queue>(D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT);
	list      = std::make_shared<List>(D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT);
	fence     = std::make_unique<Fence>(queue);
	swap      = std::make_shared<Swap>(win, queue);
	render    = std::make_unique<Render>(swap);
	depth     = std::make_shared<Depth>(win);
	primitive = std::make_unique<PrimitiveMane>(win);
	texture   = std::make_unique<TexMane>(win);
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

// �摜�̓ǂݍ���
void Application::LoadTex(int & addr, const std::string & fileName)
{
	texture->Load(addr, fileName);
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

// �_�̕`��
void Application::DrawPoint(const Vec2f & pos, const Color & color)
{
	std::vector<prm::Vertex>vertex(1);
	vertex[0] = { {pos.x, pos.y, 0.0f}, {color.r, color.g, color.b, color.a} };
	primitive->Draw(list, vertex, prm::PrimitiveType::point, D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
}

// �_�̕����`��
void Application::DrawMultiPoint(const std::vector<Vec2f>& pos, const Color & color)
{
	if (pos.size() == 0)
	{
		return;
	}

	std::vector<prm::Vertex>vertex(pos.size());
	unsigned int index = 0;
	std::for_each(vertex.begin(), vertex.end(), [&](prm::Vertex& vertex)->void {
		vertex = { {pos[index].x, pos[index].y, 0.0f}, {color.r, color.g, color.b, color.a} };
		++index;
	});
	primitive->Draw(list, vertex, prm::PrimitiveType::point, D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
}

// ���̕`��
void Application::DrawLine(const Vec2f & pos1, const Vec2f & pos2, const Color & color)
{
	std::vector<prm::Vertex>vertex(2);
	vertex[0] = { {pos1.x, pos1.y, 0.0f}, {color.r, color.g, color.b, color.a} };
	vertex[1] = { {pos2.x, pos2.y, 0.0f}, {color.r, color.g, color.b, color.a} };
	primitive->Draw(list, vertex, prm::PrimitiveType::line, D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_LINELIST);
}

// ���̕����`��
void Application::DrawMultiLine(const std::vector<Vec2f>& pos1, const std::vector<Vec2f>& pos2, const Color & color)
{
	if (pos1.size() == 0 || pos2.size() == 0)
	{
		return;
	}

	std::vector<prm::Vertex>vertex(pos1.size() + pos2.size());
	unsigned int index = 0;
	for (unsigned int i = 0; i < vertex.size(); i += 2)
	{
		vertex[i]     = { {pos1[index].x, pos1[index].y, 0.0f}, {color.r, color.g,color.b, color.a} };
		vertex[i + 1] = { {pos2[index].x, pos2[index].y, 0.0f}, {color.r, color.g,color.b, color.a} };
		++index;
	}
	primitive->Draw(list, vertex, prm::PrimitiveType::line, D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_LINELIST);
}

// �g���C�A���O���̕`��
void Application::DrawTriangle(const Vec2f & pos1, const Vec2f & pos2, const Vec2f & pos3, const Color & color)
{
	std::vector<prm::Vertex>vertex(3);
	vertex[0] = { {pos1.x, pos1.y, 0.0f}, {color.r, color.g, color.b, color.a} };
	vertex[1] = { {pos2.x, pos2.y, 0.0f}, {color.r, color.g, color.b, color.a} };
	vertex[2] = { {pos3.x, pos3.y, 0.0f}, {color.r, color.g, color.b, color.a} };
	primitive->Draw(list, vertex, prm::PrimitiveType::triangle, D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

// �O�p�`�̕����`��
void Application::DrawMultiTriangle(const std::vector<Vec2f>& pos1, const std::vector<Vec2f>& pos2, const std::vector<Vec2f>& pos3, const Color & color)
{
	if (pos1.size() == 0 || pos2.size() == 0 || pos3.size() == 0)
	{
		return;
	}

	std::vector<prm::Vertex>vertex(pos1.size() + pos2.size() + pos3.size());
	unsigned int index = 0;
	for (unsigned int i = 0; i < vertex.size(); i += 3)
	{
		vertex[i]     = { {pos1[index].x, pos1[index].y, 0.0f}, {color.r, color.g,color.b, color.a} };
		vertex[i + 1] = { {pos2[index].x, pos2[index].y, 0.0f}, {color.r, color.g,color.b, color.a} };
		vertex[i + 2] = { {pos3[index].x, pos3[index].y, 0.0f}, {color.r, color.g,color.b, color.a} };
		++index;
	}
	primitive->Draw(list, vertex, prm::PrimitiveType::triangle, D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

// �l�p�`�̕`��
void Application::DrawBox(const Vec2f & pos, const Vec2f & size, const Color & color)
{
	std::vector<prm::Vertex>vertex(4);
	vertex[0] = { {pos.x,          pos.y,          0.0f}, {color.r, color.g, color.b, color.a} };
	vertex[1] = { {pos.x + size.x, pos.y,          0.0f}, {color.r, color.g, color.b, color.a} };
	vertex[2] = { {pos.x,          pos.y + size.y, 0.0f}, {color.r, color.g, color.b, color.a} };
	vertex[3] = { {pos.x + size.x, pos.y + size.y, 0.0f}, {color.r, color.g, color.b, color.a} };
	primitive->Draw(list, vertex, prm::PrimitiveType::triangle, D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}

// �摜�̕`��
void Application::DrawTex(int & addr, const Vec2f & pos, const Vec2f & size, const Vec2f & uvPos, const Vec2f & uvSize, 
	const float & alpha, const float & angle, const bool & turnX, const bool & turnY)
{
	texture->Draw(list, addr, pos, size, uvPos, uvSize, alpha, angle, turnX, turnY);
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

	primitive->Clear();
}

// �摜�̍폜
void Application::DeleteTex(int & addr)
{
	texture->Delete(addr);
}
