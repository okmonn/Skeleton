#pragma once
#include "Manager.h"

class CompMane :
	public Manager
{
public:
	// コンストラクタ
	CompMane(std::weak_ptr<Application>app, std::weak_ptr<Sound>sound, std::weak_ptr<Mouse>mouse);
	// デストラクタ
	~CompMane();

	// 描画
	void Draw(void);

	// 処理
	void UpData(void);

private:
	// 閾値
	float threshold;

	// 前の閾値
	float oldThd;

	// 圧縮比率
	float ratio;

	// 前の圧縮比率
	float oldRatio;
};
