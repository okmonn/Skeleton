#pragma once
#include <memory>

class Application;
class Sound;

class Phase
{
public:
	// コンストラクタ
	Phase(std::weak_ptr<Application>app, std::weak_ptr<Sound>sound);
	// デストラクタ
	~Phase();

private:
	// アプリケーション
	std::unique_ptr<Application>app;

	// サウンド
	std::weak_ptr<Sound>sound;
};
