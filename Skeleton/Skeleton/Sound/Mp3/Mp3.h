#pragma once
#include <string>
#include <vector>

namespace mp3 
{
#pragma pack(1)
	// ID3v2ヘッダー
	struct ID3v2Header {
		//識別子
		char id[3];
		//バージョン
		short ver;
		//フラグ
		char flag;
		//サイズ
		long size;
	};
#pragma pack()
	// ID3v2フレーム
	struct ID3v2Fream {
		//フレームID
		char id[4];
		//サイズ
		long size;
		//フラグ
		short flag;
		//内容
		std::vector<char>contents;
	};
	
	// ヘッダーの読み込み
	long LoadHeader(ID3v2Header& header, FILE* file);

	// フレームの読み込み
	void LoadFream(std::vector<ID3v2Fream>& freams, FILE* file);
	
	// 読み込み
	int Load(const std::string& fileName);
}
