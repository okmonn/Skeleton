#pragma once
#include "PmdData.h"
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

class DescriptorMane;
class Device;

class PmdLoader
{
	// 読み込みデータ
	struct Data {
		//頂点情報
		std::shared_ptr<std::vector<pmd::Vertex>>vertex;
		//インデックス情報
		std::shared_ptr<std::vector<unsigned short>>index;
		//マテリアル情報
		std::shared_ptr<std::vector<pmd::Material>>material;
		//ボーン情報
		std::shared_ptr<std::vector<pmd::Born>>born;

		//頂点リソース
		int vRsc;
		//インデックスリソース
		int iRsc;
		//マテリアル用リソース
		int mRsc;
		//ボーン用リソース
		int bRsc;
	};

public:
	// デストラクタ
	~PmdLoader();

	// インスタンス変数の取得
	static PmdLoader& Get(void) {
		static PmdLoader instance;
		return instance;
	}

	// 読み込み
	int Load(std::weak_ptr<Device>dev, const std::string& fileName);

private:
	// コンストラクタ
	PmdLoader();
	PmdLoader(const PmdLoader&)      = delete;
	void operator=(const PmdLoader&) = delete;

	// 頂点リソースの生成
	long CreateVertexRsc(std::weak_ptr<Device>dev, const std::string& fileName);


	// ディスクリプターマネージャー
	DescriptorMane& descMane;

	// 読み込みデータ
	std::unordered_map<std::string, Data>data;
};
