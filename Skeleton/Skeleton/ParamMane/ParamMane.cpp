#include "ParamMane.h"
#include "../Application/Application.h"
#include "../Mouse/Mouse.h"

// コンストラクタ
ParamMane::ParamMane(std::weak_ptr<Application> app, std::weak_ptr<Sound> sound, std::weak_ptr<Mouse> mouse) : app(app), sound(sound), mouse(mouse),
	type(FilterType::low), oldEffe(-1.0f)
{
	Init();
	InitFunc();
}

// デストラクタ
ParamMane::~ParamMane()
{
	for (auto itr = thumb.begin(); itr != thumb.end(); ++itr)
	{
		app.lock()->DeleteTex(itr->second.id);
	}
}

// 画像読み込み
void ParamMane::Load(const std::string name, const std::string & fileName, const Vec2f & size, const Vec2f & pos)
{
	if (thumb.find(name) != thumb.end())
	{
		return;
	}

	app.lock()->LoadTex(thumb[name].id, fileName);
	thumb[name].size = size;
	thumb[name].pos  = pos;
}

// 効果情報のセット
void ParamMane::SetEffect(const std::string & name, const float & min, const float & max, const float & init)
{
	if (thumb.find(name) == thumb.end())
	{
		return;
	}

	thumb[name].min    = min;
	thumb[name].max    = max;
	thumb[name].effect = init;
	thumb[name].angle  = PIF + (PIF * (thumb[name].effect / thumb[name].max));
}

// ボックスのセット
void ParamMane::SetBox(const std::string & name, const FilterType & type, const Vec2f & pos, const Vec2f & size)
{
	if (box.find(name) != box.end())
	{
		return;
	}

	box[name].type = type;
	box[name].pos  = pos;
	box[name].size = size;
}

// 初期化
void ParamMane::Init(void)
{
	std::string file = "_rsc/Thumb.png";
	Vec2f size = 100.0f;

	Load("vol", file, size, { 355.0f, 340.0f });
	SetEffect("vol", 0.0f, 10.0f, 1.0f);

	Load("amp", file, size, { 505.0f, 340.0f });
	SetEffect("amp", 1.0f, 20.0f, 1.0f);

	Load("time", file, size, { 355.0f, 185.0f });
	SetEffect("time", 0.0f, 2.0f);

	Load("loop", file, size, { 505.0f, 185.0f });
	SetEffect("loop", 0.0f, 10.0f);

	Load("thd", file, size, { 355.0f, 25.0f });
	SetEffect("thd", 0.1f, 1.0f,1.0f);

	Load("ratio", file, size, { 505.0f, 25.0f });
	SetEffect("ratio", 0.0f, 1.0f);

	Load("noise", file, size, { 10.0f, 25.0f });
	SetEffect("noise", 0.0f, 1.0f);

	if (sound.expired())
	{
		return;
	}
	Load("filter", file, size, { 145.0f, 310.0f });
	SetEffect("filter", 100.0f, use::Floor(static_cast<float>(sound.lock()->Getinfo().sample / 2), 3), use::Floor(static_cast<float>(sound.lock()->Getinfo().sample / 2), 3));

	size = { 135.0f, 65.0f };
	SetBox("low",  FilterType::low,  { 0.0f, 410.0f }, size);
	SetBox("band", FilterType::band, { 0.0f, 330.0f }, size);
	SetBox("high", FilterType::high, { 0.0f, 253.0f }, size);
}

// 関数ポインタの列挙
void ParamMane::InitFunc(void)
{
	func["vol"] = [&]()->void {
		auto param = sound.lock()->GetParam();
		param.volume = thumb["vol"].effect;
		sound.lock()->SetParam(param);
	};

	func["amp"] = [&]()->void {
		auto param = sound.lock()->GetParam();
		param.amp = thumb["amp"].effect;
		sound.lock()->SetParam(param);
	};

	func["thd"] = [&]()->void {
		auto param = sound.lock()->GetParam();
		param.threshold = thumb["thd"].effect;
		sound.lock()->SetParam(param);
	};
	func["ratio"] = [&]()->void {
		auto param = sound.lock()->GetParam();
		param.ratio = thumb["ratio"].effect;
		sound.lock()->SetParam(param);
	};

	func["noise"] = [&]()->void {
		auto param = sound.lock()->GetParam();
		param.noiseThd = thumb["noise"].effect;
		sound.lock()->SetParam(param);
	};

	func["time"] = [&]()->void {
		auto param = sound.lock()->GetDelay();
		param.delayTime = thumb["time"].effect;
		sound.lock()->SetDelay(param);
	};
	func["loop"] = [&]()->void {
		auto param = sound.lock()->GetDelay();
		param.loop = static_cast<unsigned int>(thumb["loop"].effect);
		sound.lock()->SetDelay(param);
	};

	func["filter"] = [&]()->void {
		switch (type)
		{
		case ParamMane::FilterType::low:
			sound.lock()->LowPass(thumb["filter"].effect);
			break;
		case ParamMane::FilterType::band:
			sound.lock()->BandPass(thumb["filter"].effect);
			break;
		case ParamMane::FilterType::high:
			sound.lock()->HighPass(thumb["filter"].effect);
			break;
		default:
			break;
		}
	};
}

// 描画
void ParamMane::Draw(void)
{
	for (auto itr = box.begin(); itr != box.end(); ++itr)
	{
		if (type == itr->second.type)
		{
			app.lock()->DrawBox(itr->second.pos, itr->second.size, { 0.968f, 0.505f, 0.745f, 0.5f });
		}
	}

	for (auto itr = thumb.begin(); itr != thumb.end(); ++itr)
	{
		app.lock()->DrawTex(itr->second.id, itr->second.pos, itr->second.size, 0.0f, itr->second.size, 1.0f, itr->second.angle);
	}
}

// 処理
void ParamMane::UpData(void)
{
	if (mouse.lock()->GetClick() == -1 || mouse.lock()->GetPos() == -1)
	{
		oldEffe = -1.0f;
		return;
	}

	const int offset = 10;

	//フィルタ更新ボックスとのあたり判定
	for (auto itr = box.begin(); itr != box.end(); ++itr)
	{
		if (itr->second.pos.x < mouse.lock()->GetClick().x && mouse.lock()->GetClick().x < itr->second.pos.x + itr->second.size.x
			&& itr->second.pos.y < mouse.lock()->GetClick().y && mouse.lock()->GetClick().y < itr->second.pos.y + itr->second.size.y)
		{
			type = itr->second.type;
			sound.lock()->FilterReset();
			func["filter"]();
			break;
		}
	}

	//パラメータつまみとのあたり判定
	for (auto itr = thumb.begin(); itr != thumb.end(); ++itr)
	{
		if (std::hypot(itr->second.pos.x + itr->second.size.x / 2.0f - mouse.lock()->GetClick().x,
			itr->second.pos.y + itr->second.size.y / 2.0f - mouse.lock()->GetClick().y) <= itr->second.size.x / 2.0f)
		{
			if (oldEffe == -1.0f)
			{
				oldEffe = itr->second.effect;
			}
			float tmp = static_cast<float>((mouse.lock()->GetClick().y - mouse.lock()->GetPos().y) / offset) * 0.01f;

			itr->second.effect = oldEffe + itr->second.max * tmp;
			if (itr->second.effect > itr->second.max)
			{
				itr->second.effect = itr->second.max;
			}
			else if (itr->second.effect < itr->second.min)
			{
				itr->second.effect = itr->second.min;
			}
			itr->second.angle = PIF + (PIF * (itr->second.effect / itr->second.max));
			func[itr->first]();
			break;
		}
	}
}
