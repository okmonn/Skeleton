#pragma once
#include <string>
#include <memory>
#include <unordered_map>

struct ID3D12DescriptorHeap;
struct ID3D12Resource;
class Queue;
class List;
class Fence;

class Compute
{
public:
	// コンストラクタ
	Compute();
	// デストラクタ
	virtual ~Compute();

private:
	// 定数リソースの生成
	long CreateCRsc(const std::string& name, const size_t& size);

	// 非順序リソースの生成
	long CreateURsc(const std::string& name, const size_t& stride, const size_t& num);

protected:
	// ヒープの生成
	long CreateHeap(const unsigned int& num);

	// CBVの生成
	void CBV(const std::string& name, const size_t& size);

	// UAVの生成
	void UAV(const std::string& name, const size_t& stride, const size_t& num);

	// マップ
	long Map(const std::string& name);

	// アンマップ
	void Unmap(const std::string& name);

	// データのリセット
	void Reset(const std::string& name);

	// 終了
	void End(void);


	// キュー
	std::shared_ptr<Queue>queue;

	// リスト
	std::unique_ptr<List>list;

	// フェンス
	std::unique_ptr<Fence>fence;

	// ヒープ
	ID3D12DescriptorHeap* heap;

	// インデックス
	unsigned int index;

	// リソース
	std::unordered_map<std::string, ID3D12Resource*>rsc;

	// 送受信用データ
	std::unordered_map<std::string, void*>data;
};
