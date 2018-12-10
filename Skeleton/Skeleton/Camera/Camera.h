#pragma once
#include <memory>
#include <DirectXMath.h>

class Window;
class Device;

class Camera
{
	// VP
	struct VP {
		//�r���[
		DirectX::XMFLOAT4X4 view;
		//�v���W�F�N�V����
		DirectX::XMFLOAT4X4 projection;
		//���C�g�r���[
		DirectX::XMFLOAT4X4 lightView;
		//���C�g�v���W�F�N�V����
		DirectX::XMFLOAT4X4 lightProjection;
		//�������W
		DirectX::XMFLOAT3 eye;
		//���C�g���W
		DirectX::XMFLOAT3 lightPos;
	};

public:
	// �R���X�g���N�^
	Camera(std::weak_ptr<Window>win, std::weak_ptr<Device>dev);
	// �f�X�g���N�^
	~Camera();

	// �r���[�̍X�V
	void ChangeView(const DirectX::XMFLOAT3& eye, const DirectX::XMFLOAT3& target, 
		const DirectX::XMFLOAT3& up = { 0.0f, 1.0f, 0.0f }, const DirectX::XMFLOAT3& lightPos = { 0.0f, 10.0f, -10.0f });

	// �r���[�̎擾
	DirectX::XMFLOAT4X4 GetView(void) const {
		return vp.view;
	}
	// �v���W�F�N�V�����̎擾
	DirectX::XMFLOAT4X4 GetProjection(void) const {
		return vp.projection;
	}
	// ���C�g�r���[�̎擾
	DirectX::XMFLOAT4X4 GetLightView(void) const {
		return vp.lightView;
	}
	// ���C�g�v���W�F�N�V�����̎擾
	DirectX::XMFLOAT4X4 GetLightProjection(void) const {
		return vp.lightProjection;
	}
	// �������W�̎擾
	DirectX::XMFLOAT3 GetEye(void) const {
		return vp.eye;
	}
	// ���C�g���W�̎擾
	DirectX::XMFLOAT3 GetLightPos(void) const {
		return vp.lightPos;
	}

private:
	// VP�̏�����
	void InitVP(void);

	float Magnitude(const DirectX::XMFLOAT3 &i);

	DirectX::XMFLOAT3 Normal(const DirectX::XMFLOAT3& i);

	// �E�B���h�E
	std::weak_ptr<Window>win;

	// �f�o�C�X
	std::weak_ptr<Device>dev;

	// VP
	VP vp;
};
