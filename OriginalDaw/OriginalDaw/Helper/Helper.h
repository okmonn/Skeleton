#pragma once
#include "../etc/Vector2.h"
#include <string>
#include <vector>

namespace help {
	// バイト変換
	int Byte(const int& i);

	// ラジアン変換
	float Radian(const float& i);

	// 任意の桁から切り捨て
	float Floor(const float& i, const int& num);

	// 任意の桁から四捨五入
	float Round(const float& i, const int& num);

	// ハニング窓関数
	double Haninng(const unsigned int& i, const size_t& num);

	// 離散フーリエ変換
	void DFT(const std::vector<double>& input, std::vector<double>& real, std::vector<double>& imag);

	// 逆離散フーリエ変換
	std::vector<double> IDFT(const std::vector<double>& real, const std::vector<double>& imag);

	// 高速フーリエ変換
	void FFT(const std::vector<double>& input, std::vector<double>& real, std::vector<double>& imag);

	// 逆高速フーリエ変換
	std::vector<double> IFFT(const std::vector<double>& real, std::vector<double>& imag, const unsigned int& outNum);

	// ユニコード文字に変換
	std::wstring ChangeWString(const std::string& st);

	// マルチバイト文字に変換
	std::string ChangeString(const std::wstring& st);

	// 文字列の検索
	bool CheckChar(const std::string& find, const unsigned char* data, const size_t& num);

	// メインディスプレイの解像度の取得
	Vec2 GetDisplayResolution(void);

	// キー入力
	int CheckKey(const int& key);

	// 全部のキー情報を格納
	void GetKeys(int* key);

	// マウス座標の取得
	Vec2 GetMousePos(void);

	// マウス座標のセット
	void SetMousePos(const Vec2& pos);
}
