#include "CompMane.h"
#include "../Application/Application.h"
#include "../Mouse/Mouse.h"

// コンストラクタ
CompMane::CompMane(std::weak_ptr<Application> app, std::weak_ptr<Sound> sound, std::weak_ptr<Mouse> mouse) : 
	threshold(1.0f), ratio(1.0f / 10.0f)
{
	this->app   = app;
	this->sound = sound;
	this->mouse = mouse;

	oldThd  = threshold;
	oldRatio = ratio;

	Load("thd", "maru.png", 64.0f);
	SetBox("thd", { 64.0f * 1, 64.0f }, 64.0f);
	Load("ratio", "maru.png", 64.0f);
	SetBox("ratio", { 64.0f * 2, 64.0f }, 64.0f);
}

// デストラクタ
CompMane::~CompMane()
{
	End();
}

// 描画
void CompMane::Draw(void)
{
	for (auto itr = imag.begin(); itr != imag.end(); ++itr)
	{
		DrawImg(itr->first);
	}
}

// 処理
void CompMane::UpData(void)
{
	UpDataAngle("thd", threshold, 1.0f);
	UpDataAngle("ratio", ratio, 1.0f);

	if (mouse.lock()->GetClick() == -1 || mouse.lock()->GetPos() == 1)
	{
		oldThd   = threshold;
		oldRatio = ratio;
		return;
	}

	if (CheckMouse("thd"))
	{
		auto tmp = (mouse.lock()->GetClick().y - mouse.lock()->GetPos().y) * 0.01f;
		threshold = oldThd + tmp;
		if (threshold > 1.0f)
		{
			threshold = 1.0f;
		}
		else if (threshold < 0.0f)
		{
			threshold = 0.0f;
		}
		auto param = sound.lock()->GetParam();
		param.threshold = threshold;
		sound.lock()->SetParam(param);
		return;
	}

	if (CheckMouse("ratio"))
	{
		auto tmp = (mouse.lock()->GetClick().y - mouse.lock()->GetPos().y) * 0.01f;
		ratio = oldRatio + tmp;
		if (ratio > 1.0f)
		{
			ratio = 1.0f;
		}
		else if (ratio < 1.0f / 10.0f)
		{
			ratio = 1.0f / 10.0f;
		}
		auto param = sound.lock()->GetParam();
		param.ratio = ratio;
		sound.lock()->SetParam(param);
		return;
	}
}
