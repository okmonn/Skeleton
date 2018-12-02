#pragma once

struct ID3D12Device;

class Device
{
public:
	// コンストラクタ
	Device();
	// デストラクタ
	~Device();

	// デバイスの取得
	ID3D12Device* Get(void) const {
		return dev;
	}

private:
	// デバイスの生成
	long CreateDev(void);


	// デバイス
	ID3D12Device* dev;
};
