#include "Mixer/Mixer.h"

// エントリーポイント
#ifdef _DEBUG
int main()
#else
int __stdcall WinMain(void* hInstance, void* hPrevInstance, char* lpCmdLine, int nShowCmd)
#endif
{
	Mixer mix;
	mix.Execution();

	return 0;
}