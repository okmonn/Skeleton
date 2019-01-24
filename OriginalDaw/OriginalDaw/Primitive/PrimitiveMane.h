#pragma once
#include "PrimitiveInfo.h"
#include <list>
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <unordered_map>

enum D3D_PRIMITIVE_TOPOLOGY : int;
class Primitive;
class Window;
class List;
class Root;

class PrimitiveMane
{
public:
	// �R���X�g���N�^
	PrimitiveMane(std::weak_ptr<Window>win, std::weak_ptr<Root>root);
	// �f�X�g���N�^
	~PrimitiveMane();

	// �`��
	void Draw(std::weak_ptr<List>list, const std::vector<prm::Vertex>& vertex, const prm::PrimitiveType& type, const D3D_PRIMITIVE_TOPOLOGY& pType);

	// �N���A
	void Clear(void);

private:
	// �p�C�v�̐���
	void CreatePipe(void);

	// ������
	void Init(void);


	// �E�B���h�E
	std::weak_ptr<Window>win;

	// ���[�g
	std::weak_ptr<Root>root;

	// �v���~�e�B�u
	std::list<std::unique_ptr<Primitive>>primitive;

	// �֐��|�C���^
	std::unordered_map<prm::PrimitiveType, std::function<void(const size_t)>>func;
};
