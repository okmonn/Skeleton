#include "Manager.h"
#include "../Application/Application.h"
#include "../Mouse/Mouse.h"

// コンストラクタ
Manager::Manager()
{
	imag.clear();
}

// デストラクタ
Manager::~Manager()
{
}

// 読み込み
void Manager::Load(const std::string & name, const std::string & fileName, const Vec2f & uvSize, const Vec2f & uvPos)
{
	if (imag.find(name) != imag.end())
	{
		return;
	}
	app.lock()->LoadTex(imag[name].id, fileName);
	imag[name].uvPos  = uvPos;
	imag[name].uvSize = uvSize;
}

// 座標のセット
void Manager::SetPos(const std::string & name, const Vec2f & pos)
{
	if (imag.find(name) == imag.end())
	{
		return;
	}
	imag[name].pos = pos;
}

// サイズのセット
void Manager::SetSize(const std::string & name, const Vec2f & size)
{
	if (imag.find(name) == imag.end())
	{
		return;
	}
	imag[name].size = size;
}

// ボックスのセット
void Manager::SetBox(const std::string & name, const Vec2f & pos, const Vec2f & size)
{
	SetPos(name, pos);
	SetSize(name, size);
}

// 描画
void Manager::DrawImg(const std::string & name, const float & alpha)
{
	if (imag.find(name) == imag.end())
	{
		return;
	}
	app.lock()->DrawTex(imag[name].id, imag[name].pos, imag[name].size, imag[name].uvPos, imag[name].uvSize, alpha, imag[name].angle);
}

// マウスが範囲に入っているかチェック
bool Manager::CheckMouse(const std::string & name)
{
	if ((imag[name].pos.x < mouse.lock()->GetClick().x && mouse.lock()->GetClick().x < imag[name].pos.x + imag[name].size.x)
		&& (imag[name].pos.y < mouse.lock()->GetClick().y && mouse.lock()->GetClick().y < imag[name].pos.y + imag[name].size.y))
	{
		return true;
	}

	return false;
}

// 角度の更新
void Manager::UpDataAngle(const std::string & name, const float & now, const float & max)
{
	if (imag.find(name) == imag.end())
	{
		return;
	}

	imag[name].angle = PIF - (PIF * -(now / max));
}

// 終了
void Manager::End(void)
{
	for (auto itr = imag.begin(); itr != imag.end(); ++itr)
	{
		app.lock()->DeleteTex(itr->second.id);
	}
}
