#pragma once
#include "../Vector2.h"
#include <memory>

class Application;

class Mouse
{
public:
	// コンストラクタ
	Mouse(std::weak_ptr<Application>app);
	// デストラクタ
	~Mouse();

	// 処理
	void UpData(void);

	// クリック座標の取得
	Vec2 GetClick(void) const {
		return click;
	}
	// 現在の座標の取得
	Vec2 GetPos(void) const {
		return pos;
	}

private:
	// アプリケーション
	std::weak_ptr<Application>app;

	// クリック座標
	Vec2 click;

	// 現在の座標
	Vec2 pos;
};
