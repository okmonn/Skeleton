#pragma once
#include "../Vector2.h"
#include <string>
#include <memory>
#include <unordered_map>

class Window;
class List;
class Texture;

class TexMane
{
public:
	// コンストラクタ
	TexMane(std::weak_ptr<Window>win);
	// デストラクタ
	~TexMane();

	// 読み込み
	void Load(int& addr, const std::string& fileName);

	// 描画
	void Draw(std::weak_ptr<List>list, int& addr, const Vec2f& pos, const Vec2f& size, const Vec2f& uvPos, const Vec2f& uvSize,
		const float& alpha, const float& angle, const bool& turnX, const bool& turnY);

	// テクスチャの削除
	void Delete(int& addr);

private:
	// 初期化
	void Init(void);


	// ウィンドウ
	std::weak_ptr<Window>win;

	// 画像
	std::unordered_map<int*, std::unique_ptr<Texture>>texture;
};
