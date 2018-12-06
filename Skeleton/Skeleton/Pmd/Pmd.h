#pragma once
#include "PmdData.h"
#include <string>
#include <memory>
#include <unordered_map>

class PmdLoader;
class DescriptorMane;
class Device;
class List;
class Camera;
class Root;
class Pipe;

class Pmd
{
	struct WVP {
		DirectX::XMFLOAT4X4 world;
		DirectX::XMFLOAT4X4 view;
		DirectX::XMFLOAT4X4 projection;
	};

	// データ
	struct Data {
		//バンドル用リスト
		std::unique_ptr<List>list;

		//WVP用リソース
		int cRsc;
		WVP* wvp;

		//マテリアル用リソース
		int mRsc;
		pmd::Mat mat;
		unsigned __int8* materialData;
	};

public:
	// コンストラクタ
	Pmd(std::weak_ptr<Device>dev, std::weak_ptr<Camera>cam, std::weak_ptr<Root>root, std::weak_ptr<Pipe>pipe);
	// デストラクタ
	~Pmd();

	// 読み込み
	void Load(const std::string& fileName, int& i);

	// 回転
	void Rotate(int& i, const float& angle);
	
	// 描画
	void Draw(std::weak_ptr<List>list, int& i);

private:
	// 定数リソースの生成
	long CreateConRsc(int* i, int& rsc, const unsigned __int64& size);

	// 定数ビューの生成
	void CreateConView(int* i, int& rsc, const unsigned int& size, const int& index = 0);

	// マップ
	long Map(int& rsc, void** data);

	// バンドルのセット
	void Bundle(const std::string& fileName, int* i);


	// PMDローダー
	PmdLoader& loader;

	// ディスクリプターマネージャー
	DescriptorMane& descMane;

	// デバイス
	std::weak_ptr<Device>dev;

	// カメラ
	std::weak_ptr<Camera>cam;

	// ルート
	std::weak_ptr<Root>root;

	// パイプ
	std::weak_ptr<Pipe>pipe;

	// リソース作成番号
	int index;

	// Pmdデータ
	std::unordered_map<int*, Data>data;
};
