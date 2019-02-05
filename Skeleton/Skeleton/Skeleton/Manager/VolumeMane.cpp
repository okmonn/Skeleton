#include "VolumeMane.h"
#include "../Application/Application.h"
#include "../Mouse/Mouse.h"

// 音量最大値
#define VOL_MAX 10.0f

// コンストラクタ
VolumeMane::VolumeMane(std::weak_ptr<Application> app, std::weak_ptr<Sound> sound, std::weak_ptr<Mouse> mouse) : 
	volume(1.0f)
{
	this->app   = app;
	this->sound = sound;
	this->mouse = mouse;

	oldVolume = volume;

	Load("vol", "maru.png", 64.0f);
	SetBox("vol", { 64.0f * 3, 64.0f }, 64.0f);
}

// デストラクタ
VolumeMane::~VolumeMane()
{
	End();
}

// 描画
void VolumeMane::Draw(void)
{
	DrawImg("vol");
}

// 処理
void VolumeMane::UpData(void)
{
	UpDataAngle("vol", volume, VOL_MAX);

	if (mouse.lock()->GetClick() == -1 || mouse.lock()->GetPos() == 1)
	{
		oldVolume = volume;
		return;
	}

	if (CheckMouse("vol"))
	{
		auto tmp = (mouse.lock()->GetClick().y - mouse.lock()->GetPos().y) / VOL_MAX;
		volume = oldVolume + tmp;
		if (volume > VOL_MAX)
		{
			volume = VOL_MAX;
		}
		else if (volume < 0.0f)
		{
			volume = 0.0f;
		}
		auto param = sound.lock()->GetParam();
		param.volume = volume;
		sound.lock()->SetParam(param);
		return;
	}
}
