#include "Phase.h"
#include "../Application/Application.h"

// �E�B���h�E�T�C�Y
const Vec2f winSize = { 200.0f, 200.0f };

// �R���X�g���N�^
Phase::Phase(std::weak_ptr<Application> app, std::weak_ptr<Sound> sound) : sound(sound)
{
	this->app = std::make_unique<Application>(app, winSize);
}

// �f�X�g���N�^
Phase::~Phase()
{
}
