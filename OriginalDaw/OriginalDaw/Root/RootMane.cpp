#include "RootMane.h"
#include "Root.h"

// コンストラクタ
RootMane::RootMane()
{
	root.clear();
}

// デストラクタ
RootMane::~RootMane()
{
}

// ルートの生成
void RootMane::Create(const std::string & name, const std::wstring & fileName)
{
	if (root.find(name) != root.end())
	{
		return;
	}

	root[name] = std::make_shared<Root>(fileName);
}

// ルートの削除
void RootMane::Delete(const std::string & name)
{
	if (root.find(name) != root.end())
	{
		root.erase(root.find(name));
	}
}

// ルートの取得
std::shared_ptr<Root> RootMane::GetRoot(const std::string & name)
{
	if (root.find(name) == root.end())
	{
		return nullptr;
	}

	return root[name];
}
