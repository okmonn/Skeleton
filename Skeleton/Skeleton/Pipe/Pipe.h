#pragma once
#include <memory>

enum D3D12_PRIMITIVE_TOPOLOGY_TYPE : int;
struct D3D12_INPUT_ELEMENT_DESC;
struct ID3D12PipelineState;
class Root;

class Pipe
{
public:
	// コンストラクタ
	Pipe(std::weak_ptr<Root>root);
	// デストラクタ
	~Pipe();

	// 描画用パイプラインの生成
	long CreateGraphics(const D3D12_INPUT_ELEMENT_DESC& input, const size_t& num, const D3D12_PRIMITIVE_TOPOLOGY_TYPE& type, const bool& depth);

	// 計算用パイプラインの生成
	long CreateCompute(void);

	// パイプの取得
	ID3D12PipelineState* Get(void) const {
		return pipe;
	}

private:
	// ルート
	std::weak_ptr<Root>root;

	// パイプ
	ID3D12PipelineState* pipe;
};
