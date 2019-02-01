#pragma once
#include <vector>
#include <memory>
#include <thread>

class Application;
class Sound;
class Wave;

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

	// 再生管理
	void Play(void);

	// 処理
	void UpData(void);

	// 波形の描画
	void DrawWave(void);


	// アプリケーション
	std::shared_ptr<Application>app;

	//サウンド
	std::shared_ptr<Sound>sound;

	// 波形
	std::unique_ptr<Wave>wave;

	// 再生フラグ
	bool play;

	// スレッドフラグ
	bool threadFlag;

	// スレッド
	std::thread th;
};
