#include "DelayMane.h"
#include "../Application/Application.h"
#include "../Mouse/Mouse.h"

// 遅延時間の最大値
#define TIME_MAX 2.0f
// ループの最大数
#define LOOP_MAX 20

// コンストラクタ
DelayMane::DelayMane(std::weak_ptr<Application> app, std::weak_ptr<Sound> sound, std::weak_ptr<Mouse> mouse) : 
	attenuation(1.0f), delayTime(0.0f), loop(0.0f)
{
	this->app   = app;
	this->sound = sound;
	this->mouse = mouse;

	oldAtten = attenuation;
	oldTime  = delayTime;
	oldLoop  = loop;

	Load("atten", "maru.png", 64.0f);
	SetBox("atten", { 64.0f * 2, 0.0f }, 64.0f);
	Load("time", "maru.png", 64.0f);
	SetBox("time", { 64.0f * 3, 0.0f }, 64.0f);
	Load("loop", "maru.png", 64.0f);
	SetBox("loop", { 64.0f * 4,0.0f }, 64.0f);
}

// デストラクタ
DelayMane::~DelayMane()
{
	End();
}

// 描画
void DelayMane::Draw(void)
{
	for (auto itr = imag.begin(); itr != imag.end(); ++itr)
	{
		DrawImg(itr->first);
	}
}

// 処理
void DelayMane::UpData(void)
{
	UpDataAngle("atten", attenuation, 1.0f);
	UpDataAngle("time", delayTime, TIME_MAX);
	UpDataAngle("loop", loop, LOOP_MAX);

	if (mouse.lock()->GetClick() == -1 || mouse.lock()->GetPos() == 1)
	{
		oldAtten = attenuation;
		oldTime  = delayTime;
		oldLoop  = loop;
		return;
	}

	if (CheckMouse("atten"))
	{
		auto tmp = (mouse.lock()->GetClick().y - mouse.lock()->GetPos().y) * 0.01f;
		attenuation = oldAtten + tmp;
		if (attenuation > 1.0f)
		{
			attenuation = 1.0f;
		}
		else if (attenuation < 0.0f)
		{
			attenuation = 0.0f;
		}
		auto param = sound.lock()->GetDelay();
		param.attenuation = attenuation;
		sound.lock()->SetDelay(param);
		return;
	}

	if (CheckMouse("time"))
	{
		auto tmp = (mouse.lock()->GetClick().y - mouse.lock()->GetPos().y) * 0.01f;
		delayTime = oldTime + tmp;
		if (delayTime > TIME_MAX)
		{
			delayTime = TIME_MAX;
		}
		else if (delayTime < 0.0f)
		{
			delayTime = 0.0f;
		}
		auto param = sound.lock()->GetDelay();
		param.delayTime = delayTime;
		sound.lock()->SetDelay(param);
		return;
	}

	if (CheckMouse("loop"))
	{
		auto tmp = (mouse.lock()->GetClick().y - mouse.lock()->GetPos().y) / LOOP_MAX;
		loop = oldLoop + tmp;
		if (loop > LOOP_MAX)
		{
			loop = LOOP_MAX;
		}
		else if (loop < 0)
		{
			loop = 0;
		}
		auto param = sound.lock()->GetDelay();
		param.loop = static_cast<unsigned int>(loop);
		sound.lock()->SetDelay(param);
		return;
	}
}
