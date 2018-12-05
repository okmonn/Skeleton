#include "Pmd.h"
#include "../Device/Device.h"
#include "../List/List.h"
#include "../Root/Root.h"
#include "../Pipe/Pipe.h"
#include "../etc/Release.h"

// コンストラクタ
Pmd::Pmd(std::weak_ptr<Device> dev, std::weak_ptr<Root> root, std::weak_ptr<Pipe> pipe) : 
	dev(dev), root(root), pipe(pipe)
{
	list = std::make_unique<List>(dev, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_BUNDLE);
}

// デストラクタ
Pmd::~Pmd()
{
}

// 読み込み
void Pmd::Load(const std::string & fileName, int & i)
{
}
