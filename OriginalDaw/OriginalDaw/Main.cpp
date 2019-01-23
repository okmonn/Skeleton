#include "Mixer/Mixer.h"

// エントリーポイント
#ifdef _DEBUG
int main()
#else
int __stdcall WinMain(void* hInstance, void* hPrevInstance, char* lpCmdLine, int nShowCmd)
#endif
{
	std::unique_ptr<Mixer>mix(std::make_unique<Mixer>());
	mix->Run();

	return 0;
}