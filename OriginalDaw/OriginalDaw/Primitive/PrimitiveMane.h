#pragma once
#include <memory>

class Primitive;

class PrimitiveMane
{
public:
	// デストラクタ
	~PrimitiveMane();

	// インスタンス変数の取得
	static PrimitiveMane& Get(void) {
		static PrimitiveMane instance;
		return instance;
	}

private:
	// コンストラクタ
	PrimitiveMane();
	PrimitiveMane(const PrimitiveMane&) = delete;
	void operator=(const Primitive&) = delete;
};
