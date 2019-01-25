#pragma once
#include "Device/Device.h"
#include <tchar.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <DirectXMath.h>

// �}�b�v�̉���
#define UnMap(X) { if((X) != nullptr) (X)->Unmap(0, nullptr); }

// ���������
#define Release(X) { if((X) != nullptr) (X)->Release(); (X) = nullptr; }

// �f�o�C�X�̎擾
#define Dev Device::Get().GetDev()
