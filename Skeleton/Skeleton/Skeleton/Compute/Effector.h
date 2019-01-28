#pragma once
#include "Compute.h"
#include "../Sound/SndFunc.h"

class Effector :
	public Compute
{
public:
	// コンストラクタ
	Effector(const std::string& fileName);
	// デストラクタ
	~Effector();

	// 初期化
	void Init(const size_t& num);

	// 実行
	std::vector<float> Execution(const std::vector<float>& input);

	// パラメータのコピー
	void Copy(const snd::Param& param) {
		this->param = param;
	}

private:
	// コピー
	void Copy(const std::string& name, const snd::Param& param);
	void Copy(const std::string& name, const std::vector<float>& data);

	// 更新
	void UpData(const std::string& name, std::vector<float>& data);


	// サウンドパラメータ
	snd::Param param;
};
