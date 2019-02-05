#pragma once
#include "Manager.h"

class NoiseMane :
	public Manager
{
public:
	// コンストラクタ
	NoiseMane(std::weak_ptr<Application>app, std::weak_ptr<Sound>sound, std::weak_ptr<Mouse>mouse);
	// デストラクタ
	~NoiseMane();

	// 描画
	void Draw(void);

	// 処理
	void UpData(void);

private:
	// 閾値
	float threshold;

	// 前の閾値
	float oldThd;
};
