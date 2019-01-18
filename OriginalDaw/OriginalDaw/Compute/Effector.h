#pragma once
#include "Compute.h"
#include "../Sound/SndFunc.h"
#include <vector>

class Effector :
	public Compute
{
public:
	// コンストラクタ
	Effector(const std::wstring& fileName);
	// デストラクタ
	~Effector();

	// 初期化
	void Init(const size_t& num);

	// データのコピー
	void Copy(const std::string& name, const snd::Param& param);

	// 実行
	void Execution(std::vector<float>& data);

private:
	// データのコピー
	void Copy(const std::string& name, const std::vector<float>& data);

	// データの更新
	void UpData(const std::string& name, std::vector<float>& data);
};
