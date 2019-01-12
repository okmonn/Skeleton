#pragma once
#include "PrimitiveInfo.h"
#include <list>
#include <vector>
#include <memory>
#include <unordered_map>

struct ID3D12Resource;
class Primitive;
class Window;
class List;
class Root;
class Pipe;

class PrimitiveMane
{
	struct Copy {
		Copy() {}
		Copy(ID3D12Resource* rsc, std::vector<prm::Vertex>vertex) : 
		rsc(rsc), vertex(vertex){}
		ID3D12Resource* rsc;
		std::vector<prm::Vertex>vertex;
	};

public:
	// �R���X�g���N�^
	PrimitiveMane(std::weak_ptr<Window>win, std::weak_ptr<Root>root);
	// �f�X�g���N�^
	~PrimitiveMane();

	// �`��
	void Draw(std::weak_ptr<List>list, std::weak_ptr<Pipe>pipe, const prm::PrimitiveType& type, prm::Vertex* vertex, const size_t& num);

private:
	// �I���W���̃Z�b�g
	void Origin(void);

	// �}�b�v
	long Map(Copy& copy);


	// �E�B���h�E
	std::weak_ptr<Window>win;

	// ���[�g
	std::weak_ptr<Root>root;

	// �I���W��
	std::unordered_map<prm::PrimitiveType, std::unique_ptr<Primitive>>origin;

	// �R�s�[
	std::list<Copy>primitive;
};
