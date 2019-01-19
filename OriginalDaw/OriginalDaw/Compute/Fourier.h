#pragma once
#include "Compute.h"
#include <vector>

// フーリエ変換タイプ
enum class FourierType {
	DFT,
	IDFT,
	FFT,
	IFFT
};

class Fourier : 
	public Compute
{
public:
	// コンストラクタ
	Fourier(const std::wstring& fileName);
	// デストラクタ
	~Fourier();

	// 初期化
	void Init(const size_t& num);

	// タイプの変更
	void ChangeType(const FourierType& type) {
		this->type = type;
	}

	// 実行
	void Execution(std::vector<float>& input, std::vector<float>& real, std::vector<float>& imag);

private:
	// データのコピー
	void Copy(const std::string& name, const std::vector<float>& data);
	void Copy(const std::string& name, const FourierType& type);

	// データの更新
	void UpData(const std::string& name, std::vector<float>& data);

	
	// タイプ
	FourierType type;
};
