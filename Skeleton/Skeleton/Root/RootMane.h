#pragma once
#include "../etc/tString.h"
#include <map>
#include <memory>

class Device;
class Root;
class RootCompute;

class RootMane
{
public:
	// デストラクタ
	~RootMane();

	// インスタンス変数の取得
	static RootMane& Get(void) {
		static RootMane instance;
		return instance;
	}

	// ルートシグネチャクラスの生成
	void CreateRoot(int& i, std::weak_ptr<Device>dev, const std::tstring& fileName);

	// ルートシグネチャクラスの取得
	std::shared_ptr<Root>Get(int& i) {
		return root[&i];
	}

private:
	// コンストラクタ
	RootMane();
	RootMane(const RootMane&) {
	}
	void operator=(const RootMane&) {
	}

	// ルートシグネチャ
	std::map<int*, std::shared_ptr<Root>>root;
};