#pragma once
#include <d3d12.h>
#include <tchar.h>

#define Release(X) { if((X) != nullptr) (X)->Release(); (X) = nullptr; }
