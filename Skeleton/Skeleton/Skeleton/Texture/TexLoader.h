#pragma once
#include "TexInfo.h"
#include "../Vector2.h"
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

	// リソースの取得
	ID3D12Resource* GetRsc(const std::string& fileName);

	// デコードの取得
	unsigned char* GetDecode(const std::string& fileName);

	// サブデータの取得
	std::shared_ptr<D3D12_SUBRESOURCE_DATA> GetSub(const std::string& fileName);

	// 画像サイズの取得
	Vec2f GetSize(const std::string& fileName);

	// 画像の削除
	void Delete(const std::string& fileName);

private:
	// コンストラクタ
	TexLoader();
	TexLoader(const TexLoader&) = delete;
	void operator=(const TexLoader&) = delete;


	// 読み込みデータ
	std::unordered_map<std::string, tex::Info>info;
};
