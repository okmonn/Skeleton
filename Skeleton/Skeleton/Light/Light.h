#pragma once
#include <DirectXMath.h>

class Light
{
public:
	// �R���X�g���N�^
	Light();
	// �f�X�g���N�^
	~Light();

	// ���W�̎擾
	DirectX::XMFLOAT3 GetPos(void) const {
		return pos;
	}
	// ���W�̃Z�b�g
	void SetPos(const DirectX::XMFLOAT3& pos) {
		this->pos = pos;
	}

private:
	// ���W
	DirectX::XMFLOAT3 pos;
};
