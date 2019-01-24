#pragma once
#include "PrimitiveInfo.h"
#include <vector>
#include <memory>

enum D3D_PRIMITIVE_TOPOLOGY : int;
struct ID3D12Resource;
class Window;
class List;
class Root;
class Pipe;

class Primitive
{
public:
	// �R���X�g���N�^
	Primitive(std::weak_ptr<Window>win, std::weak_ptr<Root>root, std::weak_ptr<Pipe>pipe, const size_t& num);
	// �f�X�g���N�^
	~Primitive();

	// �`��
	void Draw(std::weak_ptr<List>list, const std::vector<prm::Vertex>& vertex, const D3D_PRIMITIVE_TOPOLOGY& type);

protected:
	// ���_�o�b�t�@�̐���
	void VertexBuffer(const size_t& num);

	// �}�b�v
	long Map(const std::vector<prm::Vertex>& vertex);


	// �E�B���h�E
	std::weak_ptr<Window>win;

	// ���[�g
	std::weak_ptr<Root>root;

	// �p�C�v
	std::weak_ptr<Pipe>pipe;

	// ���\�[�XID
	int rsc;
};
