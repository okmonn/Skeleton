#pragma once
#include "Manager.h"

class Application;
class Sound;
class Mouse;

class FilterMane : 
	public Manager
{
public:
	// コンストラクタ
	FilterMane(std::weak_ptr<Application>app, std::weak_ptr<Sound>sound, std::weak_ptr<Mouse>mouse);
	// デストラクタ
	~FilterMane();

	// 描画
	void Draw(void);

	// 処理
	void UpData(void);

	// カット周波数の取得
	float GetCut(void) const {
		return cut;
	}
	// クオリティファクタの取得
	float GetQuality(void) const {
		return q;
	}
	// クオリティファクタのセット
	void SetQuality(const float& q) {
		this->q = q;
	}

private:
	// カット周波数
	float cut;

	// 過去の周波数
	float oldCut;

	// クオリティファクタ
	float q;
};
