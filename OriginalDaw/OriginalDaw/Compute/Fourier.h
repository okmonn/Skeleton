#pragma once
#include "Compute.h"
#include <vector>

class Fourier : 
	public Compute
{
public:
	// コンストラクタ
	Fourier(std::wstring& fileName);
	// デストラクタ
	~Fourier();

	// 初期化
	void Init(const size_t& num);

	// 実行
	void Execution(const std::vector<float>& input, std::vector<float>& real, std::vector<float>imag);

private:
	// データのコピー
	void Copy(const std::string& name, const std::vector<float>& data);

	// データの更新
	void UpData(const std::string& name, std::vector<float>& data);
};
