#include "Characteristic.h"
#include "../Application/Application.h"
#include <algorithm>

// ウィンドウサイズ
const Vec2 winSize = { 200, 200 };

// コンストラクタ
Characteristic::Characteristic(std::weak_ptr<Application> app, std::weak_ptr<Sound> sound) : sound(sound)
{
	this->app = std::make_unique<Application>(app, winSize);
}

// デストラクタ
Characteristic::Characteristic()
{
}

// 描画
void Characteristic::Draw(void)
{
	app->Clear();

	auto data = sound.lock()->GetData();
	const float large = static_cast<float>(winSize.x) / static_cast<float>(sound.lock()->GetData().size());
	std::vector<Vec2f>pos1(data.size());
	std::vector<Vec2f>pos2(data.size());

	unsigned int index = 0;
	std::for_each(pos1.begin(), pos1.end(), [&](Vec2f& pos)->void {
		pos = { large * index, static_cast<float>(winSize.y / 2) };
		++index;
	});

	index = 0;
	std::for_each(pos2.begin(), pos2.end(), [&](Vec2f& pos)->void {
		pos = { large * index, static_cast<float>(winSize.y / 2) + data[index] * 100 };
		++index;
	});

	app->DrawMultiLine(pos1, pos2, { 1.0f, 0.0f, 0.0f, 1.0f });

	app->Execution();
}

// 処理
void Characteristic::UpData(void)
{
}
