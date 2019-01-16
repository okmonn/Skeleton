#pragma once
#include "../etc/tstring.h"
#include <memory>

class Root;
class Pipe;

class Font
{
public:
	// コンストラクタ
	Font(std::weak_ptr<Root>root, std::weak_ptr<Pipe>pipe, const std::tstring& code);
	// デストラクタ
	~Font();

private:
	// フォント情報の取得
	void GetInfo(void* font, const std::tstring& code);

	// SRVの生成
	void SRV(void);

	// シェーダリソースID
	int srv;

	// 頂点リソース
	int rsc;
};
