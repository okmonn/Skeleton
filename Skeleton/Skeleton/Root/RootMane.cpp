#include "RootMane.h"
#include "Root.h"
#include "../Useful/Useful.h"
#include "../Release.h"

#pragma comment(lib, "d3dcompiler.lib")

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
void RootMane::Create(const std::string & name, const std::string & fileName)
{
	if (root.find(name) != root.end())
	{
		OutputDebugString(_T("\nルート生成済み\n"));
		return;
	}

	root[name] = std::make_shared<Root>(use::ChangeCode(fileName));
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

// ルートの削除
void RootMane::Delete(const std::string & name)
{
	if (root.find(name) != root.end())
	{
		root.erase(root.find(name));
	}
}
