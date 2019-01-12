#pragma once
#include <string>
#include <memory>
#include <unordered_map>

class Root;

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

	// ルートの生成
	void Create(const std::string& name, const std::wstring& fileName);

	// ルートの取得
	std::shared_ptr<Root> GetRoot(const std::string& name) {
		return root[name];
	}

private:
	// コンストラクタ
	RootMane();
	RootMane(const Root&) = delete;
	void operator=(const Root&) = delete;


	// ルート
	std::unordered_map<std::string, std::shared_ptr<Root>>root;
};
