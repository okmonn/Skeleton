#pragma once
#include "SndFunc.h"

class Shifter
{
public:
	// コンストラクタ
	Shifter();
	// デストラクタ
	~Shifter();

	// 再生速度調節
	std::vector<float> TimeShift(const std::vector<float>& input, const float& rate);

	// ピッチ調節
	std::vector<float> PitchShift(const std::vector<float>& input, const float& pitch);

private:

};
