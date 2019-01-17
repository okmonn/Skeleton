#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include <DirectXMath.h>

class Texture;
class Window;
class List;
class Root;

class TexMane
{
public:
	// コンストラクタ
	TexMane(std::weak_ptr<Window>win, std::weak_ptr<Root>root);
	// デストラクタ
	~TexMane();

	// 読み込み
	void Load(int& addr, const std::string& fileName);

	// 描画
	void Draw(std::weak_ptr<List>list, int& addr, const DirectX::XMFLOAT2& pos, const DirectX::XMFLOAT2& size,
		const DirectX::XMFLOAT2& uvPos, const DirectX::XMFLOAT2& uvSize, const float& alpha, const float& angle, const bool& turnX, const bool& turnY);

	// 削除
	void Delete(int& addr);

private:
	// ウィンドウ
	std::weak_ptr<Window>win;

	// ルート
	std::weak_ptr<Root>root;

	// テクスチャ
	std::unordered_map<int*, std::unique_ptr<Texture>>texture;
};
