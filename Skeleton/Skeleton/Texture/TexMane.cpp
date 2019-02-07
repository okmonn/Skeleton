#include "TexMane.h"
#include "Texture.h"
#include "../Window/Window.h"
#include "../List/List.h"
#include "../Root/Root.h"
#include "../Root/RootMane.h"
#include "../Pipe/Pipe.h"
#include "../Pipe/PipeMane.h"
#include "../Release.h"

// コンストラクタ
TexMane::TexMane(std::weak_ptr<Window> win) : win(win)
{
	texture.clear();

	Init();
}

// デストラクタ
TexMane::~TexMane()
{
}

// 初期化
void TexMane::Init(void)
{
	RootMane::Get().Create("tex", "Shader/Texture.hlsl");
	PipeMane::Get().Create("tex", RootMane::Get().GetRoot("tex"), D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, { 0, 2 }, false);
}

// 読み込み
void TexMane::Load(int & addr, const std::string & fileName)
{
	if (texture.find(&addr) != texture.end())
	{
		return;
	}

	texture[&addr] = std::make_unique<Texture>(win, RootMane::Get().GetRoot("tex"), PipeMane::Get().GetPipe("tex"));
	texture[&addr]->Load(fileName);
}

// 描画
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

// テクスチャの削除
void TexMane::Delete(int & addr)
{
	if (texture.find(&addr) != texture.end())
	{
		texture.erase(texture.find(&addr));
	}
}
