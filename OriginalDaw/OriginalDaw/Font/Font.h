#pragma once
#include "FontInfo.h"
#include <string>
#include <vector>
#include <memory>

struct tagTEXTMETRICW;
typedef tagTEXTMETRICW TEXTMETRIC;
struct _GLYPHMETRICS;
typedef _GLYPHMETRICS GLYPHMETRICS;
class Window;
class List;
class Root;
class Pipe;

class Font
{
public:
	// コンストラクタ
	Font(std::weak_ptr<Window>win, void* handle, const std::string& code);
	// デストラクタ
	~Font();

	// 描画
	void Draw(std::weak_ptr<List>list, const DirectX::XMFLOAT2& pos, const DirectX::XMFLOAT2& size);

private:
	// SRVの生成
	void SRV(const TEXTMETRIC& tm, const GLYPHMETRICS& gm);

	// サブリソースに書き込み
	long WriteSub(const std::vector<unsigned char>& data);

	// 頂点バッファの生成
	void VertexBuffer(void);

	// フォント情報の取得
	void GetInfo(void* handle, const std::string& code);

	// マップ
	long Map(void);


	// ウィンドウ
	std::weak_ptr<Window>win;

	// シェーダリソースID
	int srv;

	// 頂点リソース
	int rsc;

	// 頂点
	std::vector<font::Vertex>vertex;
};
