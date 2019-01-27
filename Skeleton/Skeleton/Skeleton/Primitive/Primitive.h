#pragma once
#include <memory>

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

private:
	// ���_���\�[�X�̐���
	long CreateVRsc(const size_t& num);


	// �E�B���h�E
	std::weak_ptr<Window>win;

	// ���[�g
	std::weak_ptr<Root>root;

	// �p�C�v
	std::weak_ptr<Pipe>pipe;

	// ���_���\�[�X
	ID3D12Resource* rsc;
};
