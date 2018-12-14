#pragma once
#include "../etc/tString.h"
#include <map>
#include <memory>

struct ID3D10Blob;
typedef ID3D10Blob ID3DBlob;
struct ID3D12RootSignature;
struct ID3D12PipelineState;
struct ID3D12DescriptorHeap;
struct ID3D12Resource;
class DescriptorMane;
class Device;
class Queue;
class List;
class Fence;

class Compute
{
	struct Info {
		//リソースID
		int rsc;
		//送受信データ
		void* data;
	};

public:
	// コンストラクタ
	Compute();
	// デストラクタ
	virtual ~Compute();

private:
	// UAVリソースの生成
	long CreateUavRsc(const std::string& name, const unsigned int& size);

	// UAVの生成
	void CreateUnorderView(const std::string& name, const unsigned int& stride, const unsigned int& num);

	// CBVリソースの生成
	long CreateCbvRsc(const std::string& name, const unsigned int& size);

	// CBVの生成
	void CreateConstantView(const std::string& name, const unsigned int& size);

protected:
	// クラスのインスタンス
	void Create(void);

	// ルートシグネチャの生成
	long CreateRoot(const std::tstring& fileName);

	// パイプラインの生成
	long CreatePipe(void);

	// マップ
	long Map(const std::string& name);

	// アンマップ
	void UnMap(const std::string& name);

	// UAVの生成
	void UAV(const std::string& name, const unsigned int& stride, const unsigned int& num);

	// CBVの生成
	void CBV(const std::string& name, const unsigned int& size);

	// 終了
	void End(void);


	// ディスクリプタマネージャー
	DescriptorMane& descMane;

	// デバイス
	std::weak_ptr<Device>dev;

	// キュー
	std::shared_ptr<Queue>queue;

	// リスト
	std::unique_ptr<List>list;

	// フェンス
	std::unique_ptr<Fence>fence;

	// コンピュートシェーダー情報
	ID3DBlob* shader;

	// コンピュート用ルートシグネチャ
	ID3D12RootSignature* root;

	// コンピュート用パイプライン
	ID3D12PipelineState* pipe;

	// ヒープID
	int heap;

	// リソース情報
	std::map<std::string, Info>info;
};
