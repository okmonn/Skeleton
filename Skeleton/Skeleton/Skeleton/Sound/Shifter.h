#pragma once
#include <vector>

class Shifter
{
public:
	// コンストラクタ
	Shifter();
	// デストラクタ
	~Shifter();

	// 早送り
	std::vector<float> FastForward(const std::vector<float>& input, const float& rate, const int& sample);

private:

};
