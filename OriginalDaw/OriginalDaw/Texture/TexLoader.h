#pragma once
#include "TextureInfo.h"
#include <string>
#include <unordered_map>

class TexLoader
{
public:
	// デストラクタ
	~TexLoader();

	// インスタンス変数の取得
	static TexLoader& Get(void) {
		static TexLoader instance;
		return instance;
	}

	// 読み込み
	long Load(const std::string& fileName);

	// 削除
	void Delete(const std::string& fileName);

	// リソースの取得
	ID3D12Resource* GetRsc(const std::string& fileName) {
		return info[fileName].rsc;
	}
	// デコードの取得
	unsigned char* GetDecode(const std::string& fileName) {
		return info[fileName].decode.get();
	}
	// サブデータの取得
	std::shared_ptr<D3D12_SUBRESOURCE_DATA>GetSub(const std::string& fileName) {
		return info[fileName].sub;
	}
	// 画像の横幅の取得
	unsigned int GetWidth(const std::string& fileName);
	// 画像の横幅の取得
	unsigned int GetHeight(const std::string& fileName);

private:
	// コンストラクタ
	TexLoader();
	TexLoader(const TexLoader&) = delete;
	void operator=(const TexLoader&) = delete;


	// 元データ
	std::unordered_map<std::string, tex::Info>info;
};
