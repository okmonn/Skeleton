#pragma once
#include <memory>

class Application;
class Sound;

class Wave
{
public:
	// コンストラクタ
	Wave(std::weak_ptr<Application>app, std::weak_ptr<Sound>sound);
	// デストラクタ
	~Wave();

	// 描画
	void Draw(void);

private:
	// アプリケーション
	std::unique_ptr<Application>app;

	// サウンド
	std::weak_ptr<Sound>sound;
};
