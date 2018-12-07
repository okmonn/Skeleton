#pragma once
#include "PmdData.h"
#include <string>
#include <vector>
#include <array>
#include <memory>
#include <unordered_map>

class TextureLoader;
class DescriptorMane;
class Device;

class PmdLoader
{
	// 読み込みデータ
	struct Data {
		//頂点情報
		std::vector<pmd::Vertex>vertex;
		//インデックス情報
		std::vector<unsigned short>index;
		//マテリアル情報
		std::vector<pmd::Material>material;
		//ボーン情報
		std::vector<pmd::Born>born;

		//頂点リソース
		int vRsc;
		void* vertexData;
		//インデックスリソース
		int iRsc;
		void* indexData;

		//テクスチャID
		std::unordered_map<int, std::string>tex;
		//加算テクスチャID
		std::unordered_map<int, std::string>spa;
		//乗算テクスチャID
		std::unordered_map<int, std::string>sph;
		//トゥーンテクスチャID
		std::unordered_map<int, std::string>toon;
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

	// 頂点情報の取得
	std::vector<pmd::Vertex> GetVertex(const std::string& fileName) {
		return data[fileName].vertex;
	}
	// インデックス情報の取得
	std::vector<unsigned short> GetIndex(const std::string& fileName) {
		return data[fileName].index;
	}
	// マテリアル情報の取得
	std::vector<pmd::Material> GetMaterial(const std::string& fileName) {
		return data[fileName].material;
	}
	// ボーン情報の取得
	std::vector<pmd::Born> GetBorn(const std::string& fileName) {
		return data[fileName].born;
	}
	// 頂点リソースIDの取得
	int& GetVertexRsc(const std::string& fileName) {
		return data[fileName].vRsc;
	}
	// インデックスリソースIDの取得
	int& GetIndexRsc(const std::string& fileName) {
		return data[fileName].iRsc;
	}
	// テクスチャIDの取得
	std::unordered_map<int, std::string>& GetTexture(const std::string& fileName) {
		return data[fileName].tex;
	}
	// 加算テクスチャIDの取得
	std::unordered_map<int, std::string>& GetSpa(const std::string& fileName) {
		return data[fileName].spa;
	}
	// 乗算テクスチャIDの取得
	std::unordered_map<int, std::string>& GetSph(const std::string& fileName) {
		return data[fileName].sph;
	}
	// トゥーンテクスチャIDの取得
	std::unordered_map<int, std::string>& GetToon(const std::string& fileName) {
		return data[fileName].toon;
	}

private:
	// コンストラクタ
	PmdLoader();
	PmdLoader(const PmdLoader&)      = delete;
	void operator=(const PmdLoader&) = delete;

	// テクスチャの読み込み
	void LoadTex(std::weak_ptr<Device>dev, const std::string& fileName);

	// 加算テクスチャの読み込み
	void LoadSpa(std::weak_ptr<Device>dev, const std::string& fileName, const std::string& pass, const unsigned int& index);

	// 乗算テクスチャの読み込み
	void LoadSph(std::weak_ptr<Device>dev, const std::string& fileName, const std::string& pass, const unsigned int& index);

	// トゥーンテクスチャの読み込み
	void LoadToon(std::weak_ptr<Device>dev, const std::string& fileName);

	// 頂点リソースの生成
	long CreateRsc(std::weak_ptr<Device>dev, int* i, const unsigned long long& size);

	// マップ
	template<typename T>
	long Map(int* i, T* data, const unsigned int& size, void** rscData);


	// テクスチャローダー
	TextureLoader& tex;

	// ディスクリプターマネージャー
	DescriptorMane& descMane;

	// トゥーンテクスチャ名
	std::array<char[100], 10>toonName;

	// 読み込みデータ
	std::unordered_map<std::string, Data>data;
};
