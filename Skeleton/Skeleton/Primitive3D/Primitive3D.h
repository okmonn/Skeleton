#pragma once
#include <vector>
#include <memory>
#include <DirectXMath.h>

class DescriptorMane;
class Device;
class List;
class Camera;
class Root;
class Pipe;

namespace prm3d {
	struct Vertex
	{
		//���W
		DirectX::XMFLOAT3 pos;
		//UV
		DirectX::XMFLOAT2 uv;
		//�F
		DirectX::XMFLOAT4 color;
	};
}

class Primitive3D
{
	struct WVP {
		DirectX::XMFLOAT4X4 world;
		DirectX::XMFLOAT4X4 view;
		DirectX::XMFLOAT4X4 projection;
		DirectX::XMFLOAT4X4 lightView;
		DirectX::XMFLOAT4X4 lightProjection;
	};

public:
	// �R���X�g���N�^
	Primitive3D();
	// �f�X�g���N�^
	virtual ~Primitive3D();

protected:
	// �萔���\�[�X�̐���
	long CreateRsc(void);

	// �萔�r���[�̐���
	void CreateView(void);

	// �萔�̃}�b�v
	long Map(void);

	// ���_���\�[�X�̐���
	long CreateVertex(void);

	// �f�B�X�N���v�^�}�l�[�W���[
	DescriptorMane& descMane;

	// �f�o�C�X
	std::weak_ptr<Device>dev;

	// �J����
	std::weak_ptr<Camera>cam;

	// ���[�g
	std::weak_ptr<Root>root;

	// �p�C�v
	std::weak_ptr<Pipe>pipe;

	// �萔�q�[�v�E���\�[�X
	int constant;

	// ���_���\�[�X
	int vRsc;

	// ���_
	std::vector<prm3d::Vertex>vertex;
};
