#pragma once
#include "../Input/Input.h"
#include "../Sound/Sound.h"
#include "../Useful/Useful.h"
#include "../Color.h"

class Window;
class Queue;
class List;
class Fence;
class Swap;
class Render;
class Depth;
class PrimitiveMane;
class TexMane;

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

	// 画像の読み込み
	void LoadTex(int& addr, const std::string& fileName);

	// メッセージの確認
	bool CheckMag(void) const;

	// クリア
	void Clear(void) const;

	// 点の描画
	void DrawPoint(const Vec2f& pos, const Color& color);

	// 点の複数描画
	void DrawMultiPoint(const std::vector<Vec2f>& pos, const Color& color);

	// 線の描画
	void DrawLine(const Vec2f& pos1, const Vec2f& pos2, const Color& color);

	// 線の複数描画
	void DrawMultiLine(const std::vector<Vec2f>& pos1, const std::vector<Vec2f>& pos2, const Color& color);

	// 三角形の描画
	void DrawTriangle(const Vec2f& pos1, const Vec2f& pos2, const Vec2f& pos3, const Color& color);

	// 三角形の複数描画
	void DrawMultiTriangle(const std::vector<Vec2f>& pos1, const std::vector<Vec2f>& pos2, const std::vector<Vec2f>& pos3, const Color& color);

	// 四角形の描画
	void DrawBox(const Vec2f& pos, const Vec2f& size, const Color& color);

	// 画像の描画
	void DrawTex(int& addr, const Vec2f& pos, const Vec2f& size, const Vec2f& uvPos, const Vec2f& uvSize,
		const float& alpha = 1.0f, const float& angle = 0.0f, const bool& turnX = false, const bool& turnY = false);

	// 実行
	void Execution(void) const;

	// 画像の削除
	void DeleteTex(int& addr);

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

	// レンダー
	std::unique_ptr<Render>render;

	// デプス
	std::shared_ptr<Depth>depth;

	// プリミティブ
	std::unique_ptr<PrimitiveMane>primitive;

	// テクスチャ
	std::unique_ptr<TexMane>texture;
};
