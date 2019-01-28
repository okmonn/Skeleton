#include "TexMane.h"
#include "Texture.h"
#include "../Window/Window.h"
#include "../List/List.h"
#include "../Root/Root.h"
#include "../Root/RootMane.h"
#include "../Pipe/Pipe.h"
#include "../Pipe/PipeMane.h"
#include "../Release.h"

// �R���X�g���N�^
TexMane::TexMane(std::weak_ptr<Window> win) : win(win)
{
	texture.clear();

	Init();
}

// �f�X�g���N�^
TexMane::~TexMane()
{
}

// ������
void TexMane::Init(void)
{
	RootMane::Get().Create("tex", "Shader/Texture.hlsl");
	PipeMane::Get().Create("tex", RootMane::Get().GetRoot("tex"), D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, { 0, 2 }, false);
}

// �ǂݍ���
void TexMane::Load(int & addr, const std::string & fileName)
{
	if (texture.find(&addr) != texture.end())
	{
		return;
	}

	texture[&addr] = std::make_unique<Texture>(win, RootMane::Get().GetRoot("tex"), PipeMane::Get().GetPipe("tex"));
	texture[&addr]->Load(fileName);
}

// �`��
void TexMane::Draw(std::weak_ptr<List> list, int & addr, const Vec2f & pos, const Vec2f & size, const Vec2f & uvPos, const Vec2f & uvSize, 
	const float & alpha, const float & angle, const bool & turnX, const bool & turnY)
{
	if (texture.find(&addr) == texture.end())
	{
		return;
	}

	texture[&addr]->SetDraw({ pos.x, pos.y }, { size.x, size.y }, { uvPos.x, uvPos.y }, { uvSize.x, uvSize.y },
		alpha, angle, turnX, turnY);
	texture[&addr]->Draw(list);
}

// �e�N�X�`���̍폜
void TexMane::Delete(int & addr)
{
	if (texture.find(&addr) != texture.end())
	{
		texture.erase(texture.find(&addr));
	}
}
