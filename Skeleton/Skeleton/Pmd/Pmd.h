#pragma once
#include "PmdData.h"
#include "../Vmd/VmdData.h"
#include <map>
#include <string>
#include <memory>
#include <unordered_map>

class PmdLoader;
class TextureLoader;
class Vmd;
class DescriptorMane;
class Device;
class List;
class Camera;
class Root;
class Pipe;
class CurlNoise;

class Pmd
{
	struct WVP {
		DirectX::XMFLOAT4X4 world;
		DirectX::XMFLOAT4X4 view;
		DirectX::XMFLOAT4X4 projection;
		DirectX::XMFLOAT4X4 lightView;
		DirectX::XMFLOAT4X4 lightProjection;
		DirectX::XMFLOAT3 eyePos;
		DirectX::XMFLOAT3 lightPos;
	};

	// データ
	struct Data {
		//モデルファイル名
		std::string model;

		//バンドル用リスト
		std::unique_ptr<List>list;
		std::unique_ptr<List>sList;

		//WVP用リソース
		int cRsc;
		WVP* wvp;

		//ボーン用リソース
		int bRsc;
		std::vector<DirectX::XMMATRIX>mtx;
		void* bornData;

		//マテリアル用リソース
		int mRsc;
		pmd::Mat mat;
		unsigned __int8* materialData;

		//モーション
		std::weak_ptr<std::map<std::string, std::vector<vmd::Motion>>>motion;
		float flam;
		float animTime;
	};

public:
	// コンストラクタ
	Pmd(std::weak_ptr<Device>dev, std::weak_ptr<Camera>cam, std::weak_ptr<Root>root, std::weak_ptr<Pipe>pipe,
		std::weak_ptr<Root>sRoot, std::weak_ptr<Pipe>sPipe);
	// デストラクタ
	~Pmd();

	// 読み込み
	void Load(const std::string& fileName, int& i);

	// モーションの適応
	void Attach(const std::string& fileName, int& i);

	// 回転
	void Rotate(int& i, const float& angle);
	
	// 描画
	void Draw(std::weak_ptr<List>list, int& i);

	// 影描画
	void DrawShadow(std::weak_ptr<List>list, int& i);

	// アニメーション
	void Animation(int& i, const bool& loop, const float& animSpeed);

	// アニメーション終了確認
	bool CheckAnimEnd(int& i);

	// モデルの削除
	void Delete(int& i);

private:
	// シェーダービューの生成
	void CreateShaderView(const std::string& fileName, int* i, const int& index = 0);

	// サブリソースに書き込み
	long WriteSub(const std::string& fileName);

	// 定数リソースの生成
	long CreateConRsc(int* i, int& rsc, const unsigned __int64& size);

	// 定数ビューの生成
	void CreateConView(int* i, int& rsc, const unsigned int& size, const int& index = 0);

	// マップ
	long Map(int& rsc, void** data);

	// バンドルのセット
	void Bundle(const std::string& fileName, int* i);

	// 影用バンドルのセット
	void ShadowBundle(const std::string& fileName, int* i);

	// ボーンの回転
	void RotateBorn(int* i, const std::string& name, const DirectX::XMMATRIX& mtx);

	// ボーンの再帰処理
	void RecursiveBorn(int* i, pmd::BornNode& node, const DirectX::XMMATRIX& mtx);

	// アニメーションのリセット
	void ResetAnim(int* i);


	// PMDローダー
	PmdLoader& loader;

	// テクスチャローダー
	TextureLoader& tex;

	// VMD
	Vmd& motion;

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

	// カールノイズ
	std::unique_ptr<CurlNoise>curl;

	// 影用ルート
	std::weak_ptr<Root>sRoot;

	// 影用パイプ
	std::weak_ptr<Pipe>sPipe;

	// リソース作成番号
	int index;

	// Pmdデータ
	std::unordered_map<int*, Data>data;
};
