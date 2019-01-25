#include "PrimitiveMane.h"
#include "Primitive.h"
#include "../Pipe/PipeMane.h"
#include "../etc/Release.h"

// �R���X�g���N�^
PrimitiveMane::PrimitiveMane(std::weak_ptr<Window> win, std::weak_ptr<Root> root) : win(win), root(root)
{
	primitive.clear();

	CreatePipe();
	Init();
}

// �f�X�g���N�^
PrimitiveMane::~PrimitiveMane()
{
}

// �p�C�v�̐���
void PrimitiveMane::CreatePipe(void)
{
	PipeMane::Get().Create("point",    root, D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT,    { 0, 3 }, false);
	PipeMane::Get().Create("line",     root, D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE,     { 0, 3 }, false);
	PipeMane::Get().Create("triangle", root, D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, { 0, 3 }, false);
}

// ������
void PrimitiveMane::Init(void)
{
	func.clear();

	func[prm::PrimitiveType::point]    = [&](const size_t& num)->void {
		primitive.push_back(std::make_unique<Primitive>(win, root, PipeMane::Get().GetPipe("point"), num));
	};
	func[prm::PrimitiveType::line]     = [&](const size_t& num)->void {
		primitive.push_back(std::make_unique<Primitive>(win, root, PipeMane::Get().GetPipe("line"), num));
	};
	func[prm::PrimitiveType::triangle] = [&](const size_t& num)->void {
		primitive.push_back(std::make_unique<Primitive>(win, root, PipeMane::Get().GetPipe("triangle"), num));
	};
}

// �`��
void PrimitiveMane::Draw(std::weak_ptr<List> list, const std::vector<prm::Vertex> & vertex, const prm::PrimitiveType & type, const D3D_PRIMITIVE_TOPOLOGY & pType)
{
	func[type](vertex.size());
	primitive.back()->Draw(list, vertex, pType);
}

// �N���A
void PrimitiveMane::Clear(void)
{
	primitive.clear();
}
