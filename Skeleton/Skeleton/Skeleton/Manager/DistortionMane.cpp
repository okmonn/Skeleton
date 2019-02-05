#include "DistortionMane.h"
#include "../Application/Application.h"
#include "../Mouse/Mouse.h"

// �ő呝����
#define AMP_MAX 20.0f
// �ŏ�������
#define AMP_MIN 1.0f

// �R���X�g���N�^
DistortionMane::DistortionMane(std::weak_ptr<Application> app, std::weak_ptr<Sound> sound, std::weak_ptr<Mouse> mouse) : 
	amplification(AMP_MIN), oldAmp(AMP_MIN)
{
	this->app   = app;
	this->sound = sound;
	this->mouse = mouse;

	Load("en", "maru.png", 64.0f);
	SetBox("en", { 64.0f, 0.0f }, 64.0f);
}

// �f�X�g���N�^
DistortionMane::~DistortionMane()
{
	End();
}

// �`��
void DistortionMane::Draw(void)
{
	DrawImg("en");
}

// ����
void DistortionMane::UpData(void)
{
	UpDataAngle("en", amplification, AMP_MAX);

	if (mouse.lock()->GetClick() == -1 || mouse.lock()->GetPos() == 1)
	{
		oldAmp = amplification;
		return;
	}

	if (CheckMouse("en"))
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
	
}
