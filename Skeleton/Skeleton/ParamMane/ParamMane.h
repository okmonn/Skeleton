#pragma once
#include "../Vector2.h"
#include <string>
#include <memory>
#include <functional>
#include <unordered_map>

class Application;
class Sound;
class Mouse;

class ParamMane
{
	// つまみ情報
	struct Thumb {
		//画像ID
		int id;
		//座標
		Vec2f pos;
		//サイズ
		Vec2f size;
		//回転角度
		float angle;
		//最大効果量
		float max;
		//最小効果量
		float min;
		//効果量
		float effect;
	};

	// フィルタタイプ
	enum class FilterType {
		low,
		band,
		high
	};

	// フィルタタイプ変更ボックス
	struct FilterBox {
		//座標
		Vec2f pos;
		//サイズ
		Vec2f size;
		//タイプ
		FilterType type;
	};

public:
	// コンストラクタ
	ParamMane(std::weak_ptr<Application>app, std::weak_ptr<Sound>sound, std::weak_ptr<Mouse>mouse);
	// デストラクタ
	~ParamMane();

	// 描画
	void Draw(void);

	// 処理
	void UpData(void);

private:
	// 画像読み込み
	void Load(std::string name, const std::string& fileName, const Vec2f& size, const Vec2f& pos = 0.0f);

	// 効果情報のセット
	void SetEffect(const std::string& name, const float& min, const float& max, const float& init = 0.0f);

	// ボックスのセット
	void SetBox(const std::string& name, const FilterType& type, const Vec2f& pos, const Vec2f& size);

	// 初期化
	void Init(void);

	// 関数ポインタの列挙
	void InitFunc(void);


	// アプリケーション
	std::weak_ptr<Application>app;

	// サウンド
	std::weak_ptr<Sound>sound;

	// マウス
	std::weak_ptr<Mouse>mouse;

	// フィルタタイプ
	FilterType type;

	// 前の効果量
	float oldEffe;

	// つまみ
	std::unordered_map<std::string, Thumb>thumb;

	// フィルタ変更ボックス
	std::unordered_map<std::string, FilterBox>box;

	// 関数ポインタ
	std::unordered_map<std::string, std::function<void(void)>>func;
};
