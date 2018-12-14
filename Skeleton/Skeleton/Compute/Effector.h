#pragma once
#include "Compute.h"
#include "../etc/tString.h"
#include <vector>

class Effector :
	public Compute
{
	// パラメータ
	struct Param {
		//サンプリング周波数
		int sample;
	};

public:
	// コンストラクタ
	Effector(std::weak_ptr<Device>dev, const std::tstring& fileName);
	// デストラクタ
	~Effector();

	// 初期化
	void Init(const unsigned int& num);

	// 実行
	void Execution(const std::vector<float>& input, std::vector<float>& out);

private:
};
