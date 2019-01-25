#pragma once
#include "../Useful/Useful.h"
#include <memory>

class Window;
class Queue;
class List;
class Fence;
class Swap;

class Application
{
public:
	// コンストラクタ
	Application(const Vec2& size);
	Application(const Application& app, const Vec2& size);
	Application(std::weak_ptr<Application>app, const Vec2& size);
	// デストラクタ
	~Application();

	// タイトルの変更
	void ChangeTitle(const std::string& title) const;

	// ウィンドウサイズの取得
	Vec2 GetWinSize(void) const;

	// マウス座標の取得
	Vec2 GetMousePos(void) const ;

	// メッセージの確認
	bool CheckMag(void) const;

	// クリア
	void Clear(void) const;

	// 実行
	void Execution(void) const;

private:
	void operator=(const Application&) = delete;

	// クラスのインスタンス
	void Instance(const Vec2& size, void* parent = nullptr);


	// ウィンドウ
	std::shared_ptr<Window>win;

	// キュー
	std::shared_ptr<Queue>queue;

	// リスト
	std::shared_ptr<List>list;

	// フェンス
	std::unique_ptr<Fence>fence;

	// スワップ
	std::shared_ptr<Swap>swap;
};
