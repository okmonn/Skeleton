#include "Camera.h"
#include "../Window/Window.h"
#include "../Device/Device.h"
#include "../etc/Release.h"
#include "../etc/Func.h"

// �R���X�g���N�^
Camera::Camera(std::weak_ptr<Window>win, std::weak_ptr<Device> dev)  :
	dev(dev), win(win)
{
	vp = {};

	InitVP();
}

// �f�X�g���N�^
Camera::~Camera()
{
}

// VP�̏�����
void Camera::InitVP(void)
{
	//�_�~�[�錾
	FLOAT pos = 0.0f;
	//�J�����̈ʒu
	DirectX::XMVECTOR eye    = { 0.0f, pos, -1.0f };
	//�J�����̏œ_ 
	DirectX::XMVECTOR target = { 0.0f, pos,  0.0f };
	//�J�����̏����
	DirectX::XMVECTOR upper  = { 0.0f, 1.0f, 0.0f };

	DirectX::XMStoreFloat4x4(&vp.view, DirectX::XMMatrixLookAtLH(eye, target, upper));
	DirectX::XMStoreFloat4x4(&vp.projection, DirectX::XMMatrixPerspectiveFovLH(RAD(90.0f), (float)win.lock()->GetX() / (float)win.lock()->GetY(), 0.5f, 500.0f));
	vp.eye = { 0.0f, pos, -1.0f };;
}

// �r���[�̍X�V
void Camera::ChangeView(const DirectX::XMFLOAT3 & eye, const DirectX::XMFLOAT3 & target, const DirectX::XMFLOAT3 & up)
{
	//�J�����̈ʒu
	DirectX::XMVECTOR eyeVec = { eye.x,    eye.y,    eye.z };
	//�J�����̏œ_ 
	DirectX::XMVECTOR tarVec = { target.x, target.y, target.z };
	//�J�����̏����
	DirectX::XMVECTOR upVec  = { up.x,     up.y,     up.z };

	DirectX::XMStoreFloat4x4(&vp.view, DirectX::XMMatrixLookAtLH(eyeVec, tarVec, upVec));
	vp.eye = eye;
}
