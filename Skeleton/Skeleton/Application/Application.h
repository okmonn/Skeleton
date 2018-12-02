#pragma once
#include "../etc/KeyData.h"
#include <memory>

class Window;
class Input;
class Union;
class Effector;
class Sound;

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

	// キーの入力
	bool InputKey(const int& i);

	// 描画
	void Draw(void);

private:
	// コンストラクタ
	Application();
	Application(const Application&) {
	}
	void operator=(const Application&) {
	}

	// クラスの生成
	void Create(void);


	// ウィンドウ
	std::shared_ptr<Window>win;

	// インプット
	std::shared_ptr<Input>input;

	// ユニオン
	std::shared_ptr<Union>un;

	// エフェクター
	std::shared_ptr<Effector>effector;

	// サウンド
	std::shared_ptr<Sound>sound;
};
