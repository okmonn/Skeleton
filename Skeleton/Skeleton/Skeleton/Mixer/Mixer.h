#pragma once
#include <memory>

class Application;
class Sound;

class Mixer
{
public:
	// コンストラクタ
	Mixer();
	// デストラクタ
	~Mixer();

	// 実行
	void Execution(void);

private:
	// 描画
	void Draw(void);

	// 処理
	void UpData(void);


	// アプリケーション
	std::shared_ptr<Application>app;

	//サウンド
	std::shared_ptr<Sound>sound;
};
