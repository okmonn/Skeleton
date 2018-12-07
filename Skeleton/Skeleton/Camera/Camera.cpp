#include "Camera.h"
#include "../Window/Window.h"
#include "../Device/Device.h"
#include "../etc/Release.h"
#include "../etc/Func.h"

// コンストラクタ
Camera::Camera(std::weak_ptr<Window>win, std::weak_ptr<Device> dev)  :
	dev(dev), win(win)
{
	vp = {};

	InitVP();
}

// デストラクタ
Camera::~Camera()
{
}

// VPの初期化
void Camera::InitVP(void)
{
	//ダミー宣言
	FLOAT pos = 0.0f;
	//カメラの位置
	DirectX::XMVECTOR eye    = { 0.0f, pos, -1.0f };
	//カメラの焦点 
	DirectX::XMVECTOR target = { 0.0f, pos,  0.0f };
	//カメラの上方向
	DirectX::XMVECTOR upper  = { 0.0f, 1.0f, 0.0f };

	DirectX::XMStoreFloat4x4(&vp.view, DirectX::XMMatrixLookAtLH(eye, target, upper));
	DirectX::XMStoreFloat4x4(&vp.projection, DirectX::XMMatrixPerspectiveFovLH(RAD(90.0f), (float)win.lock()->GetX() / (float)win.lock()->GetY(), 0.5f, 500.0f));
	vp.eye = { 0.0f, pos, -1.0f };;
}

// ビューの更新
void Camera::ChangeView(const DirectX::XMFLOAT3 & eye, const DirectX::XMFLOAT3 & target, const DirectX::XMFLOAT3 & up)
{
	//カメラの位置
	DirectX::XMVECTOR eyeVec = { eye.x,    eye.y,    eye.z };
	//カメラの焦点 
	DirectX::XMVECTOR tarVec = { target.x, target.y, target.z };
	//カメラの上方向
	DirectX::XMVECTOR upVec  = { up.x,     up.y,     up.z };

	DirectX::XMStoreFloat4x4(&vp.view, DirectX::XMMatrixLookAtLH(eyeVec, tarVec, upVec));
	vp.eye = eye;
}
