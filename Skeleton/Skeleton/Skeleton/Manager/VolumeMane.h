#pragma once
#include "Manager.h"

class VolumeMane :
	public Manager
{
public:
	// コンストラクタ
	VolumeMane(std::weak_ptr<Application>app, std::weak_ptr<Sound>sound, std::weak_ptr<Mouse>mouse);
	// デストラクタ
	~VolumeMane();

	// 描画
	void Draw(void);

	// 処理
	void UpData(void);

private:
	// 音量
	float volume;

	// 前の音量
	float oldVolume;
};
