#include "FilterMane.h"
#include "../Application/Application.h"
#include "../Mouse/Mouse.h"

// 増減幅
#define OFFSET 100.0f

// コンストラクタ
FilterMane::FilterMane(std::weak_ptr<Application> app, std::weak_ptr<Sound> sound, std::weak_ptr<Mouse> mouse) : 
	q(1.0f / std::sqrt(2.0f))
{
	this->app   = app;
	this->sound = sound;
	this->mouse = mouse;

	SetPos(64.f);
	SetSize(64.0f);
	Load("en", "maru.png", 64.0f);

	cut    = use::Floor(static_cast<float>(sound.lock()->Getinfo().sample / 2), 2);
	oldCut = cut;
}

// デストラクタ
FilterMane::~FilterMane()
{
	End();
}

// 描画
void FilterMane::Draw(void)
{
	DrawImg("en", pos, size);
}

// 処理
void FilterMane::UpData(void)
{
	if (mouse.lock()->GetClick() == -1 || mouse.lock()->GetPos() == 1)
	{
		oldCut = cut;
		return;
	}

	//最大値
	const float max = use::Floor(static_cast<float>(sound.lock()->Getinfo().sample / 2), 2);
	//最小値
	const float min = OFFSET;

	if (CheckMouse(pos, size))
	{
		auto tmp = (mouse.lock()->GetClick().y - mouse.lock()->GetPos().y);
		cut = oldCut + tmp * OFFSET;
		if (cut > max)
		{
			cut = max;
		}
		else if (cut < min)
		{
			cut = min;
		}
	}
	sound.lock()->LowPass(cut, q);
	UpDataAngle("en", cut, max);
}
