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
		//UV座標
		Vec2f pos;
		//UVサイズ
		Vec2f size;
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

	// 座標のセット
	void SetPos(const Vec2f& pos) {
		this->pos = pos;
	}
	// サイズのセット
	void SetSize(const Vec2f& size) {
		this->size = size;
	}

protected:
	// 読み込み
	void Load(const std::string& name, const std::string& fileName, const Vec2f& uvSize, const Vec2f& uvPos = 0.0f);

	// 描画
	void DrawImg(const std::string& name, const Vec2f& pos, const Vec2f& size, const float& alpha = 1.0f);

	// マウスが範囲に入っているかチェック
	bool CheckMouse(const Vec2f& pos, const Vec2f& size);

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

	// 座標
	Vec2f pos; 

	// サイズ
	Vec2f size;

	// 画像情報
	std::unordered_map<std::string, Imag>imag;
};
