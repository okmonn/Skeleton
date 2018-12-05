#pragma once
#include <string>
#include <memory>
#include <unordered_map>

class Device;
class List;
class Root;
class Pipe;

class Pmd
{
public:
	// コンストラクタ
	Pmd(std::weak_ptr<Device>dev, std::weak_ptr<Root>root, std::weak_ptr<Pipe>pipe);
	// デストラクタ
	~Pmd();

	// 読み込み
	void Load(const std::string& fileName, int& i);

private:
	// デバイス
	std::weak_ptr<Device>dev;

	// バンドル用リスト
	std::unique_ptr<List>list;

	// ルート
	std::weak_ptr<Root>root;

	// パイプ
	std::weak_ptr<Pipe>pipe;

	// Pmdデータ

};
