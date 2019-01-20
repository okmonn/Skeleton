#pragma once
#include "Compute.h"
#include <vector>

// フーリエ変換タイプ
enum class FourierType {
	//離散フーリエ変換
	DFT,
	//高速フーリエ変換
	FFT
};

class Fourier : 
	public Compute
{
	// パラメータ
	struct Param {
		//タイプ
		FourierType type;
		//数
		unsigned int stage;
	};

public:
	// コンストラクタ
	Fourier(const std::wstring& fileName);
	// デストラクタ
	~Fourier();

	// 初期化
	void Init(const size_t& num);

	// タイプの変更
	void SetParam(const FourierType& type, const unsigned int& stage = 0) {
		param = { type, stage };
	}

	// 実行
	void Execution(const std::vector<float>& input, std::vector<float>& real, std::vector<float>& imag);

private:
	// データのコピー
	void Copy(const std::string& name, const std::vector<float>& data);
	void Copy(const std::string& name, const Param& param);

	// データの更新
	void UpData(const std::string& name, std::vector<float>& data);

	
	// パラメータ
	Param param;
};
