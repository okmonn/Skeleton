#include "RootMane.h"
#include "../Device/Device.h"
#include "Root.h"

#pragma comment (lib, "d3dcompiler.lib")

// コンストラクタ
RootMane::RootMane()
{
	root.clear();
}

// デストラクタ
RootMane::~RootMane()
{
}

// ルートシグネチャクラスの生成
void RootMane::CreateRoot(int& i, std::weak_ptr<Device>dev, const std::tstring & fileName)
{
	root[&i] = std::make_shared<Root>(dev, fileName);
}
