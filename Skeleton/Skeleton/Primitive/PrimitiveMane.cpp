#include "PrimitiveMane.h"
#include "../Root/Root.h"
#include "../Root/RootMane.h"
#include "../Pipe/Pipe.h"
#include "../Pipe/PipeMane.h"
#include "Primitive.h"
#include "../Release.h"

// コンストラクタ
PrimitiveMane::PrimitiveMane(std::weak_ptr<Window> win) : win(win)
{
	primitive.clear();

	Init();
}

// デストラクタ
PrimitiveMane::~PrimitiveMane()
{
}

// 初期化
void PrimitiveMane::Init(void)
{
	RootMane::Get().Create("primitive", "Shader/Primitive.hlsl");

	PipeMane::Get().Create("point",    RootMane::Get().GetRoot("primitive"), D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT,    { 0, 3 }, false);
	PipeMane::Get().Create("line",     RootMane::Get().GetRoot("primitive"), D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE,     { 0, 3 }, false);
	PipeMane::Get().Create("triangle", RootMane::Get().GetRoot("primitive"), D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, { 0, 3 }, false);
	func.clear();

	func[prm::PrimitiveType::point] = [&](const size_t& num)->void {
		primitive.push_back(std::make_unique<Primitive>(win, RootMane::Get().GetRoot("primitive"), PipeMane::Get().GetPipe("point"),    num));
	};
	func[prm::PrimitiveType::line] = [&](const size_t& num)->void {
		primitive.push_back(std::make_unique<Primitive>(win, RootMane::Get().GetRoot("primitive"), PipeMane::Get().GetPipe("line"),     num));
	};
	func[prm::PrimitiveType::triangle] = [&](const size_t& num)->void {
		primitive.push_back(std::make_unique<Primitive>(win, RootMane::Get().GetRoot("primitive"), PipeMane::Get().GetPipe("triangle"), num));
	};
}

// 描画
void PrimitiveMane::Draw(std::weak_ptr<List> list, const std::vector<prm::Vertex>& vertex, const prm::PrimitiveType & type, const D3D_PRIMITIVE_TOPOLOGY & pType)
{
	func[type](vertex.size());
	primitive.back()->Draw(list, vertex, pType);
}

// クリア
void PrimitiveMane::Clear(void)
{
	primitive.clear();
}

