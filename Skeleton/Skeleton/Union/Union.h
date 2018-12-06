#pragma once
#include "../etc/tString.h"
#include <map>
#include <list>
#include <vector>
#include <memory>

enum D3D12_PRIMITIVE_TOPOLOGY_TYPE : int;
class RootMane;
class PipeMane;
class Window;
class Device;
class Queue;
class List;
class Swap;
class Render;
class Depth;
class Fence;
class Texture;
class Point;
class Line;
class Triangle;
class Camera;
class Pmd;

class Union
{
public:
	// コンストラクタ
	Union(std::weak_ptr<Window>win);
	// デストラクタ
	~Union();

	// 画像の読み込み
	void LoadImg(const std::string& fileName, int& i);

	// 配列データから画像を生成
	void CreateImg(const std::vector<unsigned char>&data, const unsigned int& width, const unsigned int& height, int& i);

	// 画像の描画
	void DrawImg(int& i, const float& x, const float& y, const float& sizeX, const float& sizeY, const float& rectX, const float& rectY, 
		const float& rectSizexX, const float& rectSizeY, const float& alpha = 1.0f, const bool& turnX = false, const bool& turnY = false);

	// 点の描画
	void DrawPoint(const float& x, const float& y, const float& r, const float& g, const float& b, const float& alpha = 1.0f);

	// 線の描画
	void DrawLine(const float& x1, const float& y1, const float& x2, const float& y2, 
		const float& r, const float& g, const float& b, const float& alpha = 1.0f);

	// 三角形の描画
	void DrawTriangle(const float& x1, const float& y1, const float& x2, const float& y2, const float& x3, const float& y3,
		const float& r, const float& g, const float& b, const float& alpha = 1.0f);

	// 四角形の描画
	void DrawBox(const float& x, const float& y, const float& sizeX, const float& sizeY,
		const float& r, const float& g, const float& b, const float& alpha = 1.0f);

	// PMDの読み込み
	void LoadPmd(const std::string& fileName, int& i);

	// PMDのY軸回転
	void RotatePmd(int& i, const float& angle);

	// PMDの描画
	void DrawPmd(int& i);

	// 画面クリア
	void Clear(void);

	// 実行
	void Execution(void);

	// デバイスの取得
	std::shared_ptr<Device>GetDev(void) const {
		return dev;
	}

private:
	// ルートシグネチャの生成
	void CreateRoot(const std::string& name, const std::tstring& fileName);
	void CreateRoot(void);

	// パイプラインの生成
	void CreatePipe(const std::string& name, const std::string& rootName, const D3D12_PRIMITIVE_TOPOLOGY_TYPE& type,
		const std::initializer_list<int>&index, const bool& depth = false);
	void CreatePipe(void);

	// クラスの生成
	void Create(void);

	// ルートマネジャー
	RootMane& root;

	// パイプマネジャー
	PipeMane& pipe;

	// ウィンドウ
	std::weak_ptr<Window>win;

	// デバイス
	std::shared_ptr<Device>dev;

	// キュー
	std::shared_ptr<Queue>queue;

	// リスト
	std::shared_ptr<List>list;

	// スワップ
	std::shared_ptr<Swap>swap;

	// レンダー
	std::unique_ptr<Render>ren;

	// 深度
	std::unique_ptr<Depth>depth;

	// フェンス
	std::unique_ptr<Fence>fence;

	// ルートシグネチャID
	std::map<std::string, int>rootNo;

	// パイプラインID
	std::map<std::string, int>pipeNo;

	// テクスチャ
	std::unique_ptr<Texture>tex;

	// ポイント
	std::list<std::shared_ptr<Point>>point;

	// ライン
	std::list<std::shared_ptr<Line>>line;

	// トライアングル
	std::list<std::shared_ptr<Triangle>>triangle;

	// カメラ
	std::shared_ptr<Camera>cam;

	// PMD
	std::unique_ptr<Pmd>pmd;
};
