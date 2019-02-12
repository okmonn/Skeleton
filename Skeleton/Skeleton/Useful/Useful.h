#pragma once
#include "../Vector2.h"
#include <string>
#include <vector>

// �~����(double)
#define PI 3.141592653589979323846
// �~����(float)
#define PIF 3.141592653589979323846f

namespace use
{
	// ���j�R�[�h�ϊ�
	std::wstring ChangeCode(const std::string& code);

	// �}���`�o�C�g�ɕϊ�
	std::string ChangeCode(const std::wstring& code);

	// ���W�A���ϊ�
	float Rad(const float& i);

	// �o�C�g�ϊ�
	int Byte(const int& bit);

	// �C�ӂ̌�����؂�̂�
	float Floor(const float& i, const int& num);

	// �C�ӂ̌�����l�̌ܓ�
	float Round(const float& i, const int& num);

	// �����_���l�̎擾
	int GetRand(const int& min, const int& max);
	float GetRand(const float& min, const float& max);

	// �����̊m�F
	bool CheckChar(const std::string& find, const unsigned char* data, const size_t& num);

	// �L�[�̓��͂̊m�F
	unsigned int CheckKey(const unsigned int& key);

	// ���C���f�B�X�v���C�̉𑜓x�̎擾
	Vec2 GetDisplayResolution(void);

	// ��ʏ�̃}�E�X���W�̎擾
	Vec2 GetMousePos(void);

	// �V���N���֐�
	float Sinc(const float& i);

	// �n�j���O���֐�
	double Haninng(const unsigned int& i, const size_t& num);

	// ���U�t�[���G�ϊ�
	void DFT(const std::vector<double>& input, std::vector<double>& real, std::vector<double>& imag);
	void DFT(const std::vector<float>& input, std::vector<float>& real, std::vector<float>& imag);

	// �t���U�t�[���G�ϊ�
	std::vector<double> IDFT(const std::vector<double>& real, const std::vector<double>& imag);
	std::vector<float> IDFT(const std::vector<float>& real, const std::vector<float>& imag);

	// �����t�[���G�ϊ�
	void FFT(const std::vector<double>& input, std::vector<double>& real, std::vector<double>& imag);
	void FFT(const std::vector<float>& input, std::vector<float>& real, std::vector<float>& imag);

	// �t�����t�[���G�ϊ�
	std::vector<double> IFFT(const std::vector<double>& real, const std::vector<double>& imag, const unsigned int& outNum);
	std::vector<float> IFFT(const std::vector<float>& real, const std::vector<float>& imag, const unsigned int& outNum);

	// �U���̎擾
	std::vector<double> Amplitude(const std::vector<double>& real, const std::vector<double>& imag);
	std::vector<float> Amplitude(const std::vector<float>& real, const std::vector<float>& imag);

	// �ʑ��̎擾
	std::vector<double> Phase(const std::vector<double>& real, const std::vector<double>& imag);
	std::vector<float> Phase(const std::vector<float>& real, const std::vector<float>& imag);
}