#pragma once
#include <vector>
#include <memory>
#include <DirectXMath.h>

enum D3D_PRIMITIVE_TOPOLOGY : int;
class Window;
class List;
class Root;
class Pipe;

namespace prm {
	struct Vertex
	{
		//���W
		DirectX::XMFLOAT3 pos;
		//�F
		DirectX::XMFLOAT4 color;
	};
}

class Primitive
{
public:
	// �R���X�g���N�^
	Primitive();
	// �f�X�g���N�^
	virtual ~Primitive();

	// ���_�̃Z�b�g
	virtual void SetVertex(const DirectX::XMFLOAT2& pos, const DirectX::XMFLOAT4& color) = 0;

	// �`��
	void Draw(std::weak_ptr<List>list);

protected:
	// �萔�o�b�t�@�̐���
	void ConstantBuffer(void);

	// ���_�o�b�t�@�̐���
	void VertexBuffer(void);

	// �o���h���̃Z�b�g
	void Bundle(const D3D_PRIMITIVE_TOPOLOGY& type);


	// �E�B���h�E
	std::weak_ptr<Window>win;

	// ���X�g
	std::unique_ptr<List>list;

	// ���[�g
	std::weak_ptr<Root>root;

	// �p�C�v
	std::weak_ptr<Pipe>pipe;

	// �萔�q�[�v�E���\�[�X
	int constant;

	// ���_���\�[�X
	int vRsc;

	// ���_
	std::vector<prm::Vertex>vertex;
};
