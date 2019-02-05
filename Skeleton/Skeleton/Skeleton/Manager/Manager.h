#pragma once
#include "../Vector2.h"
#include <string>
#include <memory>
#include <unordered_map>

class Application;
class Sound;
class Mouse;

class Manager
{
	struct Imag {
		//ID
		int id;
		//座標
		Vec2f pos;
		//サイズ
		Vec2f size;
		//UV座標
		Vec2f uvPos;
		//UVサイズ
		Vec2f uvSize;
		//角度
		float angle;
	};
public:
	// コンストラクタ
	Manager();
	// デストラクタ
	virtual ~Manager();

	// 描画
	virtual void Draw(void) = 0;

	// 処理
	virtual void UpData(void) = 0;

protected:
	// 読み込み
	void Load(const std::string& name, const std::string& fileName, const Vec2f& uvSize, const Vec2f& uvPos = 0.0f);

	// 座標のセット
	void SetPos(const std::string& name, const Vec2f& pos);

	// サイズのセット
	void SetSize(const std::string& name, const Vec2f& size);

	// ボックスのセット
	void SetBox(const std::string& name, const Vec2f& pos, const Vec2f& size);

	// 描画
	void DrawImg(const std::string& name, const float& alpha = 1.0f);

	// マウスが範囲に入っているかチェック
	bool CheckMouse(const std::string& name);

	// 角度の更新
	void UpDataAngle(const std::string& name, const float& now, const float& max);

	// 終了
	void End(void);


	// アプリケーション
	std::weak_ptr<Application>app;

	// サウンド
	std::weak_ptr<Sound>sound;

	// マウス
	std::weak_ptr<Mouse>mouse;

	// 画像情報
	std::unordered_map<std::string, Imag>imag;
};
