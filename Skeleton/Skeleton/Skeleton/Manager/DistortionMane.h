#pragma once
#include "Manager.h"

class DistortionMane : 
	public Manager
{
public:
	// コンストラクタ
	DistortionMane(std::weak_ptr<Application>app, std::weak_ptr<Sound>sound, std::weak_ptr<Mouse>mouse);
	// デストラクタ
	~DistortionMane();

	// 描画
	void Draw(void);

	// 処理
	void UpData(void);

	// 増幅率の取得
	float GetAmp(void) const {
		return amplification;
	}

private:
	// 増幅率
	float amplification;

	// 過去の増幅率
	float oldAmp;
};
