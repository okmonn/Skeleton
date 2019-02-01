#pragma once
#include "../Vector2.h"
#include <string>
#include <vector>

namespace use
{
	// ユニコード変換
	std::wstring ChangeCode(const std::string& code);

	// マルチバイトに変換
	std::string ChangeCode(const std::wstring& code);

	// ラジアン変換
	float Rad(const float& i);

	// バイト変換
	int Byte(const int& bit);

	// 任意の桁から切り捨て
	float Floor(const float& i, const int& num);

	// 任意の桁から四捨五入
	float Round(const float& i, const int& num);

	// 文字の確認
	bool CheckChar(const std::string& find, const unsigned char* data, const size_t& num);

	// キーの入力の確認
	unsigned int CheckKey(const unsigned int& key);

	// メインディスプレイの解像度の取得
	Vec2 GetDisplayResolution(void);

	// 画面上のマウス座標の取得
	Vec2 GetMousePos(void);

	// シンク窓関数
	float Sinc(const float& i);

	// ハニング窓関数
	float Haninng(const unsigned int& i, const size_t& num);

	// 離散フーリエ変換
	void DFT(const std::vector<double>& input, std::vector<double>& real, std::vector<double>& imag);
	//離散フーリエ変換・精度低下
	void DFT(const std::vector<float>& input, std::vector<float>& real, std::vector<float>& imag);

	// 逆離散フーリエ変換
	std::vector<double> IDFT(const std::vector<double>& real, const std::vector<double>& imag);
	// 逆離散フーリエ変換・精度低下
	std::vector<float> IDFT(const std::vector<float>& real, const std::vector<float>& imag);

	// 高速フーリエ変換
	void FFT(const std::vector<double>& input, std::vector<double>& real, std::vector<double>& imag);
	// 高速フーリエ変換・精度低下
	void FFT(const std::vector<float>& input, std::vector<float>& real, std::vector<float>& imag);

	// 逆高速フーリエ変換
	std::vector<double> IFFT(const std::vector<double>& real, const std::vector<double>& imag, const unsigned int& outNum);
	// 逆高速フーリエ変換・精度低下
	std::vector<float> IFFT(const std::vector<float>& real, const std::vector<float>& imag, const unsigned int& outNum);
}
