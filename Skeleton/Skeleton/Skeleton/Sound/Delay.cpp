#include "Delay.h"
#include <algorithm>

// �R���X�g���N�^
Delay::Delay()
{
	param = { 1.0f, 0.0f, 0 };

	old.clear();
}

// �f�X�g���N�^
Delay::~Delay()
{
}

// �p�����[�^�̃Z�b�g
void Delay::SetParam(const snd::DelayParam & param)
{
	this->param = param;
}

// ���s
std::vector<float> Delay::Execution(const std::vector<float>& input, const snd::Info & info, const unsigned int offset)
{
	if (old.size() != info.channel)
	{
		old.resize(info.channel);
	}

	//�x���Ԋu
	const float delay = info.sample * param.delayTime;

	//�K���f�[�^
	std::vector<float>adap = input;

	std::vector<float>tmp(input.size() / info.channel);
	for (int i = 0; i < info.channel; ++i)
	{
		unsigned int index = i;
		std::for_each(tmp.begin(), tmp.end(), [&](float& n)->void {
			n = input[index];
			index += info.channel;
		});

		std::copy(tmp.begin(), tmp.end(), std::back_inserter(old[i]));

		index = 0;
		for (unsigned int n = i; n < adap.size(); n += info.channel)
		{
			for (unsigned int loop = 1; loop <= param.loop; ++loop)
			{
				int m = (offset / info.channel) + index - static_cast<unsigned int>(delay * loop);
				if (m >= 0)
				{
					adap[n] += old[i][m] * std::pow(param.attenuation, static_cast<float>(loop));
				}
			}
			++index;
		}
	}

	return adap;
}

// �N���A
void Delay::Clear(void)
{
	old.clear();
}
