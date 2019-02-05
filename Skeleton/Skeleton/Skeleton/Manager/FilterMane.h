#pragma once
#include "Manager.h"
#include <functional>

class Application;
class Sound;
class Mouse;

class FilterMane : 
	public Manager
{
	enum class FilterType {
		low,
		high
	};
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

private:
	// 初期化
	void Init(void);


	// カット周波数
	float cut;

	// 過去の周波数
	float oldCut;

	// フィルタタイプ
	FilterType type;

	// 関数ポインタ
	std::unordered_map<FilterType, std::function<void(const float&)>>func;
};
