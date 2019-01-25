#pragma once
#include "../etc/Vector2.h"
#include <string>
#include <vector>

namespace help {
	// �o�C�g�ϊ�
	int Byte(const int& i);

	// ���W�A���ϊ�
	float Radian(const float& i);

	// �C�ӂ̌�����؂�̂�
	float Floor(const float& i, const int& num);

	// �C�ӂ̌�����l�̌ܓ�
	float Round(const float& i, const int& num);

	// �n�j���O���֐�
	double Haninng(const unsigned int& i, const size_t& num);

	// ���U�t�[���G�ϊ�
	void DFT(const std::vector<double>& input, std::vector<double>& real, std::vector<double>& imag);

	// �t���U�t�[���G�ϊ�
	std::vector<double> IDFT(const std::vector<double>& real, const std::vector<double>& imag);

	// �����t�[���G�ϊ�
	void FFT(const std::vector<double>& input, std::vector<double>& real, std::vector<double>& imag);

	// �t�����t�[���G�ϊ�
	std::vector<double> IFFT(const std::vector<double>& real, std::vector<double>& imag, const unsigned int& outNum);

	// ���j�R�[�h�����ɕϊ�
	std::wstring ChangeWString(const std::string& st);

	// �}���`�o�C�g�����ɕϊ�
	std::string ChangeString(const std::wstring& st);

	// ������̌���
	bool CheckChar(const std::string& find, const unsigned char* data, const size_t& num);

	// ���C���f�B�X�v���C�̉𑜓x�̎擾
	Vec2 GetDisplayResolution(void);

	// �L�[����
	int CheckKey(const int& key);

	// �S���̃L�[�����i�[
	void GetKeys(int* key);

	// �}�E�X���W�̎擾
	Vec2 GetMousePos(void);

	// �}�E�X���W�̃Z�b�g
	void SetMousePos(const Vec2& pos);
}
