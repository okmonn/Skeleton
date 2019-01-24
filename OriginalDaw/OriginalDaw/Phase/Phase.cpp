#include "Phase.h"
#include "../Application/Application.h"

// ウィンドウサイズ
const Vec2f winSize = { 200.0f, 200.0f };

// コンストラクタ
Phase::Phase(std::weak_ptr<Application> app, std::weak_ptr<Sound> sound) : sound(sound)
{
	this->app = std::make_unique<Application>(app, winSize);
}

// デストラクタ
Phase::~Phase()
{
}
