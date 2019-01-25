#pragma once
#include <string>
#include <memory>
#include <unordered_map>

enum D3D12_PRIMITIVE_TOPOLOGY_TYPE : int;
class Root;
class Pipe;

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

	// パイプの生成
	void Create(const std::string& name, std::weak_ptr<Root>root, const D3D12_PRIMITIVE_TOPOLOGY_TYPE& type, const std::initializer_list<int>&index, const bool& depth);
	void Create(const std::string& name, std::weak_ptr<Root>root);

	// パイプの削除
	void Delete(const std::string& name);

	// パイプの取得
	std::shared_ptr<Pipe> GetPipe(const std::string& name);

private:
	// コンストラクタ
	PipeMane();
	PipeMane(const PipeMane&) = delete;
	void operator=(const PipeMane&) = delete;


	// パイプ
	std::unordered_map<std::string, std::shared_ptr<Pipe>>pipe;
};
