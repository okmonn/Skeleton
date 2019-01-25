#pragma once
#include <memory>

class Application;
class Sound;

class Characteristic
{
public:
	// コンストラクタ
	Characteristic(std::weak_ptr<Application>app, std::weak_ptr<Sound>sound);
	// デストラクタ
	Characteristic();

	// 描画
	void Draw(void);

	// 処理
	void UpData(void);

private:
	// アプリケーション
	std::unique_ptr<Application>app;

	// サウンド
	std::weak_ptr<Sound>sound;
};
