#pragma once
#include "../etc/Vector2.h"
#include "../etc/Color.h"
#include <memory>

class Window;
class Queue;
class List;
class Fence;
class Swap;
class Render;
class Depth;
class PrimitiveMane;

class Application
{
public:
	// デストラクタ
	~Application();

	// インスタンス変数の取得
	static Application& Get(void) {
		static Application instance;
		return instance;
	}

	// メッセージの確認
	bool CheckMsg(void);

	// キー入力
	bool CheckKey(const int& key);

	// ポイントの描画
	void DrawPoint(const Vec2f& pos, const Color& color);

	// ラインの描画
	void DrawLine(const Vec2f& pos1, const Vec2f& pos2, const Color& color);

	// トライアングルの描画
	void DrawTriangle(const Vec2f& pos1, const Vec2f& pos2, const Vec2f& pos3, const Color& color);

	// ボックスの描画
	void DrawBox(const Vec2f& pos, const Vec2f& size, const Color& color);

	// 画面クリア
	void Clear(void);

	// コマンドの実行
	void Execution(void);

private:
	// コンストラクタ
	Application();
	Application(const Application&) = delete;
	void operator=(const Application&) = delete;

	// クラスの生成
	void Create(void);

	// ルートの生成
	void CreateRoot(void);

	// パイプの生成
	void CreatePipe(void);


	// ウィンドウ
	std::shared_ptr<Window>win;

	// キュー
	std::shared_ptr<Queue>queue;

	// リスト
	std::shared_ptr<List>list;

	// フェンス
	std::shared_ptr<Fence>fence;

	// スワップ
	std::shared_ptr<Swap>swap;

	// レンダー
	std::shared_ptr<Render>render;

	// デプス
	std::shared_ptr<Depth>depth;

	// プリミティブ
	std::unique_ptr<PrimitiveMane>primitive;
};
