#pragma once
#include <map>
#include <memory>

enum D3D12_PRIMITIVE_TOPOLOGY_TYPE : int;
struct D3D12_INPUT_ELEMENT_DESC;
class Device;
class Swap;
class Root;
class RootCompute;
class Pipe;
class PipeCompute;

class PipeMane
{
public:
	// デストラクタ
	~PipeMane();

	// インスタンス変数の取得
	static PipeMane& Get(void) {
		static PipeMane instance;
		return instance;
	}

	// パイプラインの生成
	void CreatePipe(int& i, std::weak_ptr<Device>dev, std::weak_ptr<Swap>swap, std::weak_ptr<Root>root, const D3D12_PRIMITIVE_TOPOLOGY_TYPE& type, const std::initializer_list<int>&index, const bool& depth);

	// パイプラインクラスの取得
	std::shared_ptr<Pipe>Get(int& i) {
		return pipe[&i];
	}

private:
	// コンストラクタ
	PipeMane();
	PipeMane(const PipeMane&)       = delete;
	void operator=(const PipeMane&) = delete;

	// パイプライン
	std::map<int*, std::shared_ptr<Pipe>>pipe;
};
