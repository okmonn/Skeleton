#pragma once
#include <DirectXMath.h>

namespace vmd {
	// ���[�V�����f�[�^
	struct Motion {
		//�t���[��
		unsigned long flam;
		//�ʒu
		DirectX::XMFLOAT3 pos;
		//�N�H�[�^�j�I��
		DirectX::XMFLOAT4 rotation;
		//�⊮
		DirectX::XMFLOAT2 a;
		DirectX::XMFLOAT2 b;
	};
}
