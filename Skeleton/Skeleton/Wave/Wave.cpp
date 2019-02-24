#include "Wave.h"
#include "../Application/Application.h"
#include <algorithm>

// ウィンドウサイズ
const Vec2 winSize = { 400, 480 };

// コンストラクタ
Wave::Wave(std::weak_ptr<Application> app, std::weak_ptr<Sound> sound) : sound(sound)
{
	this->app = std::make_unique<Application>(app, winSize, Vec2(app.lock()->GetWinPos().x - winSize.x, app.lock()->GetWinPos().y));
	this->app->ChangeTitle("音声波形");
}

// デストラクタ
Wave::~Wave()
{
}

// 描画
void Wave::Draw(void)
{
	if (sound.expired())
	{
		return;
	}

	app->Clear();

	std::vector<float>tmp = sound.lock()->GetData();

	const int channel = sound.lock()->Getinfo().channel;

	unsigned int index = 0;
	std::vector<float>data(tmp.size() / channel);
	std::for_each(data.begin(), data.end(), [&](float& i)->void {
		for (int n = 0; n < channel; ++n)
		{
			i += tmp[index + n];
		}
		index += channel;
	});

	//Xの描画間隔
	const float x = static_cast<float>(winSize.x) / static_cast<float>(data.size());

	index = 0;
	std::vector<Vec2f>pos1(data.size());
	std::for_each(pos1.begin(), pos1.end(), [&](Vec2f& pos)->void {
		pos = { x * index, static_cast<float>(winSize.y / 2) };
		++index;
	});

	index = 0;
	std::vector<Vec2f>pos2(data.size());
	std::for_each(pos2.begin(), pos2.end(), [&](Vec2f& pos)->void {
		pos = { x * index, static_cast<float>(winSize.y / 2) - data[index] * 10 };
		++index;
	});

	app->DrawMultiLine(pos1, pos2, { 1.0f, 0.0f, 0.0f, 1.0f });

	app->Execution();
}
