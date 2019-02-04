#pragma once
#include "../Vector2.h"
#include <string>
#include <vector>
#include <memory>
#include <thread>
#include <unordered_map>

class Application;
class Sound;
class Mouse;
class DistortionMane;
class FilterMane;
class Wave;
class Characteristic;

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

	// 特性の描画
	void DrawChara(void);


	// アプリケーション
	std::shared_ptr<Application>app;

	//サウンド
	std::shared_ptr<Sound>sound;

	// マウス
	std::shared_ptr<Mouse>mouse;

	// ディストーション
	std::unique_ptr<DistortionMane>distortion;

	// デジタルフィルタ
	std::unique_ptr<FilterMane>filter;

	// 波形
	std::unique_ptr<Wave>wave;
	
	// 特性
	std::unique_ptr<Characteristic>chara;

	// 再生フラグ
	bool play;

	// スレッドフラグ
	bool threadFlag;

	// スレッド
	std::vector<std::thread>th;
};
