#include "NoiseMane.h"
#include "../Application/Application.h"
#include "../Mouse/Mouse.h"

// 閾値の最大数
#define THD_MAX 0.5f

// コンストラクタ
NoiseMane::NoiseMane(std::weak_ptr<Application> app, std::weak_ptr<Sound> sound, std::weak_ptr<Mouse> mouse) : 
	threshold(0.0f)
{
	this->app   = app;
	this->sound = sound;
	this->mouse = mouse;

	oldThd = threshold;

	Load("noise", "maru.png", 64.0f);
	SetBox("noise", { 64.0f *0, 64.0f }, 64.0f);
}

// デストラクタ
NoiseMane::~NoiseMane()
{
}

// 描画
void NoiseMane::Draw(void)
{
	DrawImg("noise");
}

// 処理
void NoiseMane::UpData(void)
{
	UpDataAngle("noise", threshold, THD_MAX);

	if (mouse.lock()->GetClick() == -1 || mouse.lock()->GetPos() == 1)
	{
		oldThd = threshold;
		return;
	}

	if (CheckMouse("noise"))
	{
		auto tmp = (mouse.lock()->GetClick().y - mouse.lock()->GetPos().y) * 0.01f;
		threshold = oldThd + tmp;
		if (threshold > THD_MAX)
		{
			threshold = THD_MAX;
		}
		else if (threshold < 0.0f)
		{
			threshold = 0.0f;
		}
		auto param = sound.lock()->GetParam();
		param.noiseThd = threshold;
		sound.lock()->SetParam(param);
		return;
	}
}
