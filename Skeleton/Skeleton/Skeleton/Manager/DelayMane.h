#pragma once
#include "Manager.h"

class DelayMane :
	public Manager
{
public:
	// コンストラクタ
	DelayMane(std::weak_ptr<Application>app, std::weak_ptr<Sound>sound, std::weak_ptr<Mouse>mouse);
	// デストラクタ
	~DelayMane();

	// 描画
	void Draw(void);

	// 処理
	void UpData(void);

private:
	// 減衰率
	float attenuation;

	// 前の減衰率
	float oldAtten;

	// 遅延時間
	float delayTime;

	// 前の遅延時間
	float oldTime;

	// ループ回数
	float loop;

	// 前のループ回数
	float oldLoop;
};
