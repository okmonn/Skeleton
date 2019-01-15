#include "Effector.h"
#include "../Root/Root.h"
#include "../Root/RootMane.h"
#include "../Pipe/Pipe.h"
#include "../Pipe/PipeMane.h"
#include "../Queue/Queue.h"
#include "../List/List.h"
#include "../Fence/Fence.h"
#include <d3d12.h>

// コンストラクタ
Effector::Effector(const std::wstring & fileName)
{
	Load("effect", fileName);
}

// デストラクタ
Effector::~Effector()
{
	End();
}

// 実行
void Effector::Execution(void)
{
	list->Reset();

	list->SetComputeRoot(RootMane::Get().GetRoot("effect")->Get());
	list->SetPipe(PipeMane::Get().GetPipe("effect")->Get());

	list->Dispatch(1, 1, 1);

	list->Close();

	ID3D12CommandList* com[] = {
		list->GetList(),
	};
	queue->Execution(com, _countof(com));

	fence->Wait();
}
