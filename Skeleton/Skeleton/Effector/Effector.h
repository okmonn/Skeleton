#pragma once
#include "../etc/tString.h"
#include <map>
#include <vector>
#include <memory>

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
class Root;
class Pipe;

class Effector
{
	struct Info {
		//リソース
		ID3D12Resource* rsc;
		//送信データ
		float* data;
		//番号
		int index;
	};

	// パラメータ
	struct Param {
		//全体の波形数からの相対数
		unsigned int index;
		//サンプリング周波数
		int sample;

		//LowPass適応フラグ
		int low;
		//HightPass適応フラグ
		int hight;
		//BundPass適応フラグ
		int bund;
		//カットオフ周波数
		float cutoff;
		//クオリティ
		float q;
		//帯域幅
		float octave;
	};

public:
	// コンストラクタ
	Effector(std::weak_ptr<Device>dev, const std::tstring& fileName);
	// デストラクタ
	~Effector();

	// フィルターのパラメータのセット
	void SetFilterParam(const float& cutoff, const float& q = 1.0f / sqrt(2.0f), const float& octave = 1.0f);

	// LowPassFilterの実行管理
	void LowPassFilter(const bool& flag);

	// HightPassFilterの実行管理
	void HightPassFilter(const bool& flag);

	// BundPassFilterの実行管理
	void BundPassFilter(const bool& flag);

	// 実行
	void Execution(const std::vector<float>& wave, std::vector<float>& adaptation, const unsigned int& index, const unsigned int& sample = 44100);

	// イベントハンドルの取得
	void* GetHandle(void) {
		return handle;
	}

private:
	// ルートシグネチャの生成
	long CreateRoot(const std::tstring& fileName);

	// パイプラインの生成
	long CreatePipe(void);

	// ヒープの生成
	long CreateHeap(void);

	// CBVリソースの生成
	long CreateCbvRsc(const std::string& name, const unsigned int& size);

	// UAVリソースの生成
	long CreateUavRsc(const std::string& name, const unsigned int& size);

	// CBVの生成
	void CreateConstantView(const std::string& name, const unsigned int& size);

	// UAVの生成
	void CreateUnorderView(const std::string& name, const unsigned int& stride, const unsigned int& num);

	// マップ
	long Map(const std::string& name);

	// CBVの生成
	void CBV(const std::string& name, const unsigned int& size);

	// UAVの生成
	void UAV(const std::string& name, const unsigned int& stride, const unsigned int& num);

	// 初期化
	void Init(void);


	// デバイス
	std::weak_ptr<Device>dev;

	// キュー
	std::shared_ptr<Queue>queue;

	// リスト
	std::unique_ptr<List>list;

	// フェンス
	std::unique_ptr<Fence>fence;

	// ルートシグネチャ
	ID3D12RootSignature* root;

	// シェーダー情報
	ID3DBlob* shader;

	// パイプライン
	ID3D12PipelineState* pipe;

	// ヒープ
	ID3D12DescriptorHeap* heap;

	// リソース番号
	int index;

	// パラメータ
	Param param;

	// バッファ
	std::map<std::string, Info>info;

	// イベントハンドル
	void* handle;
};
