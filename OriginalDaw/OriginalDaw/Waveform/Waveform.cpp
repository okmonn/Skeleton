#include "Waveform.h"
#include "../Application/Application.h"

// �E�B���h�E�T�C�Y
const Vec2 winSize = { 200, 200 };

// �R���X�g���N�^
Waveform::Waveform(std::weak_ptr<Application> app, std::weak_ptr<Sound> sound) :sound(sound)
{
	this->app = std::make_unique<Application>(app, winSize);
}

// �f�X�g���N�^
Waveform::~Waveform()
{
}

// �`��
void Waveform::Draw()
{
	app->Clear();
	auto data = sound.lock()->GetData();
	float large = static_cast<float>(winSize.x) / static_cast<float>(sound.lock()->GetData().size());
	for (unsigned int i = 0; i < data.size(); ++i)
	{
		Vec2f pos1 = { i * large, static_cast<float>(winSize.y / 2) };
		Vec2f pos2 = { i * large, static_cast<float>(winSize.y / 2) - data[i] * 100 };
		app->DrawLine(pos1, pos2, { 1.0f, 0.0f, 0.0f, 1.0f });
	}
	app->Execution();
}

// ����
void Waveform::UpData(void)
{
}
