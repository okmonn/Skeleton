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
		//�������W
		DirectX::XMFLOAT3 eye;
	};

public:
	// �R���X�g���N�^
	Camera(std::weak_ptr<Window>win, std::weak_ptr<Device>dev);
	// �f�X�g���N�^
	~Camera();

	// �r���[�̍X�V
	void ChangeView(const DirectX::XMFLOAT3& eye, const DirectX::XMFLOAT3& target, const DirectX::XMFLOAT3& up = { 0.0f, 1.0f, 0.0f });

	// �r���[�̎擾
	DirectX::XMFLOAT4X4 GetView(void) const {
		return vp.view;
	}
	// �v���W�F�N�V�����̎擾
	DirectX::XMFLOAT4X4 GetProjection(void) const {
		return vp.projection;
	}
	// �������W�̎擾
	DirectX::XMFLOAT3 GetEye(void) const {
		return vp.eye;
	}

private:
	// VP�̏�����
	void InitVP(void);

	// �E�B���h�E
	std::weak_ptr<Window>win;

	// �f�o�C�X
	std::weak_ptr<Device>dev;

	// VP
	VP vp;
};
