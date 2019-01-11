#pragma once

struct ID3D12Device;

class Device
{
public:
	// デストラクタ
	~Device();

	// インスタンス変数の取得
	static Device& Get(void) {
		static Device instance;
		return instance;
	}

	// デバイスの取得
	ID3D12Device* GetDev(void) const {
		return dev;
	}

private:
	// コンストラクタ
	Device();
	Device(const Device&) = delete;
	void operator=(const Device&) = delete;

	// デバイスの生成
	long Create(void);


	// デバイス
	ID3D12Device* dev;
};
