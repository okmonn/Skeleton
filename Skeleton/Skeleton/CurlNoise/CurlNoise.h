#pragma once
#include "../etc/tString.h"
#include <map>
#include <memory>
#include <vector>
#include <DirectXMath.h>

struct ID3D10Blob;
typedef ID3D10Blob ID3DBlob;
struct ID3D12RootSignature;
struct ID3D12PipelineState;
struct ID3D12DescriptorHeap;
struct ID3D12Resource;
class Device;
class Queue;
class List;
class Fence;

class CurlNoise
{
	struct Info {
		//リソース
		ID3D12Resource* rsc;
		//送信データ
		DirectX::XMFLOAT3* data;
		//番号
		int index;
	};

public:
	// コンストラクタ
	CurlNoise(std::weak_ptr<Device>dev, const std::tstring& fileName);
	// デストラクタ
	~CurlNoise();

	// UAVの生成
	void UAV(const std::string& name, const unsigned int& stride, const unsigned int& num);

	// データの初期化
	void Init(const std::vector<DirectX::XMFLOAT3>& pos);

	// 実行
	void Execution(void);

private:
	// ルートシグネチャの生成
	long CreateRoot(const std::tstring& fileName);

	// パイプラインの生成
	long CreatePipe(void);

	// ヒープの生成
	long CreateHeap(void);

	// UAVリソースの生成
	long CreateUavRsc(const std::string& name, const unsigned int& size);

	// UAVの生成
	void CreateUnorderView(const std::string& name, const unsigned int& stride, const unsigned int& num);

	// マップ
	long Map(const std::string& name);


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

	// コンピュートルートシグネチャ
	ID3D12RootSignature* root;

	// コンピュートパイプライン
	ID3D12PipelineState* pipe;

	// ヒープ
	ID3D12DescriptorHeap* heap;

	// リソース番号
	int index;

	// バッファ
	std::map<std::string, Info>info;

	// データ
	std::vector<DirectX::XMFLOAT3>pos;
};
