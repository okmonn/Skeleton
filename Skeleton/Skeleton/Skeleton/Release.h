#pragma once
#include "Device/Device.h"
#include <tchar.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <DirectXMath.h>

// マップの解除
#define UnMap(X) { if((X) != nullptr) (X)->Unmap(0, nullptr); }

// メモリ解放
#define Release(X) { if((X) != nullptr) (X)->Release(); (X) = nullptr; }

// デバイスの取得
#define Dev Device::Get().GetDev()
