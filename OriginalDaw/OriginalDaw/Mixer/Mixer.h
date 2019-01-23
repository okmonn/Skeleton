#pragma once
#include <vector>
#include <memory>
#include <thread>

class Application;
class Sound;
class Waveform;

class Mixer
{
public:
	// コンストラクタ
	Mixer();
	// デストラクタ
	~Mixer();

	// 処理
	void Run(void);

private:
	// 初期化
	void Init(void);

	// 描画
	void Draw(void);

	// 処理
	void UpData(void);

	// サウンドの波形周波数の描画
	void DrawWave(void);


	// アプリケーション
	std::shared_ptr<Application>app;
	
	// サウンド
	std::shared_ptr<Sound>sound;

	// 波形
	std::unique_ptr<Waveform>wave;

	// 再生フラグ
	bool playFlag;

	// スレッドフラグ
	bool threadFlag;

	// スレッド
	std::vector<std::thread>th;
};
