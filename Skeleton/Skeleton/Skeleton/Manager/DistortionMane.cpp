#include "DistortionMane.h"
#include "../Application/Application.h"
#include "../Mouse/Mouse.h"

// �ő呝����
#define AMP_MAX 10.0f
// �ŏ�������
#define AMP_MIN 1.0f

// �R���X�g���N�^
DistortionMane::DistortionMane(std::weak_ptr<Application> app, std::weak_ptr<Sound> sound, std::weak_ptr<Mouse> mouse) : 
	amplification(AMP_MIN), oldAmp(AMP_MIN)
{
	this->app   = app;
	this->sound = sound;
	this->mouse = mouse;

	SetPos({ 64.0f, 0.0f });
	SetSize(64.0f);
	Load("en", "maru.png", 64.0f);
	Load("en1", "maru.png", 64.0f);
}

// �f�X�g���N�^
DistortionMane::~DistortionMane()
{
	End();
}

// �`��
void DistortionMane::Draw(void)
{
	DrawImg("en", pos, size);
	DrawImg("en1", pos + Vec2f(64.0f, 0.0f), size);
}

// ����
void DistortionMane::UpData(void)
{
	if (mouse.lock()->GetClick() == -1 || mouse.lock()->GetPos() == 1)
	{
		oldAmp = amplification;
		return;
	}

	if (CheckMouse(pos, size))
	{
		auto tmp = (mouse.lock()->GetClick().y - mouse.lock()->GetPos().y) / AMP_MAX;
		amplification = oldAmp + tmp;
		if (amplification > AMP_MAX)
		{
			amplification = AMP_MAX;
		}
		else if (amplification < AMP_MIN)
		{
			amplification = AMP_MIN;
		}
	}
	auto param = sound.lock()->GetParam();
	param.amp = amplification;
	sound.lock()->SetParam(param);
	UpDataAngle("en", amplification, AMP_MAX);
}
