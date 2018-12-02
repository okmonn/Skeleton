#include "PipeMane.h"
#include "../Device/Device.h"
#include "../Swap/Swap.h"
#include "../Root/Root.h"
#include "Pipe.h"
#include <vector>
#include <d3d12.h>
#include <dxgi1_6.h>

// インプットエレメント
const D3D12_INPUT_ELEMENT_DESC inputs[] = {
	//0
	{ "POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	//1
	{ "NORMAL",   0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	//2
	{ "TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT,       0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	//3
	{ "COLOR",    0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	//4
	{ "ALPHA",    0, DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT,          0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	//5
	{ "BORN",     0, DXGI_FORMAT::DXGI_FORMAT_R16G16_UINT,        0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	//6
	{ "WEIGHT",   0, DXGI_FORMAT::DXGI_FORMAT_R8_UINT,            0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
};


// コンストラクタ
PipeMane::PipeMane()
{
	pipe.clear();
}

// デストラクタ
PipeMane::~PipeMane()
{
}

// パイプラインの生成
void PipeMane::CreatePipe(int & i, std::weak_ptr<Device>dev, std::weak_ptr<Swap>swap, std::weak_ptr<Root> root, const D3D12_PRIMITIVE_TOPOLOGY_TYPE & type, const std::initializer_list<int> & index, const bool & depth)
{
	std::vector< D3D12_INPUT_ELEMENT_DESC>input;
	for (auto& idx : index)
	{
		input.push_back(inputs[idx]);
	}

	pipe[&i] = std::make_shared<Pipe>(dev, swap, root);
	pipe[&i]->Create(*input.data(), input.size(), type, depth);
}
