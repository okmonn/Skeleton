#pragma once
#include <memory>
#include <DirectXMath.h>

class Window;
class Device;

class Camera
{
	// VP
	struct VP {
		//ビュー
		DirectX::XMFLOAT4X4 view;
		//プロジェクション
		DirectX::XMFLOAT4X4 projection;
		//視線座標
		DirectX::XMFLOAT3 eye;
	};

public:
	// コンストラクタ
	Camera(std::weak_ptr<Window>win, std::weak_ptr<Device>dev);
	// デストラクタ
	~Camera();

	// ビューの更新
	void ChangeView(const DirectX::XMFLOAT3& eye, const DirectX::XMFLOAT3& target, const DirectX::XMFLOAT3& up = { 0.0f, 1.0f, 0.0f });

	// ビューの取得
	DirectX::XMFLOAT4X4 GetView(void) const {
		return vp.view;
	}
	// プロジェクションの取得
	DirectX::XMFLOAT4X4 GetProjection(void) const {
		return vp.projection;
	}
	// 視線座標の取得
	DirectX::XMFLOAT3 GetEye(void) const {
		return vp.eye;
	}

private:
	// VPの初期化
	void InitVP(void);

	// ウィンドウ
	std::weak_ptr<Window>win;

	// デバイス
	std::weak_ptr<Device>dev;

	// VP
	VP vp;
};
