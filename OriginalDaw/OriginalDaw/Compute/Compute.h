#pragma once
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

class Queue;
class List;
class Fence;

class Compute
{
	// 情報
	struct Info {
		//リソースID
		int rsc;
		//送信データ
		void* data;
	};
public:
	// コンストラクタ
	Compute();
	// デストラクタ
	virtual ~Compute();

	// データのコピー
	template<typename T>
	void Copy(const std::string& name, const T& data);

protected:
	// シェーダの読み込み
	void Load(const std::string& name, const std::wstring& fileName);

	// マップ
	long Map(const std::string& name);

	// CBVの生成
	void CBV(const std::string& name, const size_t& size);

	// UAVの生成
	void UAV(const std::string& name, const size_t& stride, const size_t& num);

	// アンマップ
	void Unmap(const std::string& name);

	// リセット
	void Reset(const std::string& name);

	// データのコピー
	template<typename T>
	void Copy(const std::string& name, const std::vector<T>& data);

	// ヒープのセット
	void SetHeap(void);

	// リソースのセット
	void SetRsc(void);

	// データの更新
	void UpData(const std::string& name, std::vector<float>& data);

	// 終了
	void End(void);


	// キュー
	std::shared_ptr<Queue>queue;

	// リスト
	std::unique_ptr<List>list;

	// フェンス
	std::unique_ptr<Fence>fence;

	// ヒープID
	int heap;

	// リソース情報
	std::unordered_map<std::string, Info>info;
};

// データのコピー
template<typename T>
inline void Compute::Copy(const std::string & name, const T & data)
{
	if (info.find(name) == info.end())
	{
		return;
	}

	memcpy(info[name].data, &data, sizeof(T));
}

// データのコピー
template<typename T>
inline void Compute::Copy(const std::string & name, const std::vector<T>& data)
{
	if (info.find(name) == info.end())
	{
		return;
	}

	memcpy(info[name].data, data.data(), sizeof(T) * data.size());
}
