#include "FilterMane.h"
#include "../Application/Application.h"
#include "../Mouse/Mouse.h"

// オフセット
#define OFFSET 100.0f

// コンストラクタ
FilterMane::FilterMane(std::weak_ptr<Application> app, std::weak_ptr<Sound> sound, std::weak_ptr<Mouse> mouse) : 
	type(FilterType::low)
{
	this->app   = app;
	this->sound = sound;
	this->mouse = mouse;

	cut    = use::Floor(static_cast<float>(sound.lock()->Getinfo().sample / 2), 2);
	oldCut = cut;

	Load("en", "maru.png", 64.0f);
	SetBox("en", 0.0f, 64.0f);
	Load("low", "maru.png", 64.0f);
	SetBox("low", {0.0f, 300.0f}, 64.0f);
	Load("high", "maru.png", 64.0f);
	SetBox("high", {64.0f, 300.0f}, 64.0f);

	Init();
}

// デストラクタ
FilterMane::~FilterMane()
{
	End();
}

// 描画
void FilterMane::Draw(void)
{
	for (auto itr = imag.begin(); itr != imag.end(); ++itr)
	{
		DrawImg(itr->first);
	}
}

// 初期化
void FilterMane::Init(void)
{
	func[FilterType::low] = [&](const float& cut)->void {
		sound.lock()->LowPass(cut);
	};
	func[FilterType::high] = [&](const float& cut)->void {
		sound.lock()->HighPass(cut);
	};
}

// 処理
void FilterMane::UpData(void)
{
	//最大値
	const float max = use::Floor(static_cast<float>(sound.lock()->Getinfo().sample / 2), 2);
	UpDataAngle("en", cut, max);

	if (mouse.lock()->GetClick() == -1 || mouse.lock()->GetPos() == 1)
	{
		oldCut = cut;
		return;
	}

	if (CheckMouse("low"))
	{
		type = FilterType::low;
		func[type](cut);
		return;
	}

	if (CheckMouse("high"))
	{
		type = FilterType::high;
		func[type](cut);
		return;
	}

	if (CheckMouse("en"))
	{
		auto tmp = (mouse.lock()->GetClick().y - mouse.lock()->GetPos().y);
		cut = oldCut + tmp * OFFSET;
		if (cut > max)
		{
			cut = max;
		}
		else if (cut < OFFSET)
		{
			cut = OFFSET;
		}
		func[type](cut);
		return;
	}
}
