#pragma once
#include "PrimitiveInfo.h"

struct ID3D12Resource;

class Primitive
{
public:
	// コンストラクタ
	Primitive(const prm::PrimitiveType& type);
	// デストラクタ
	~Primitive();

	// リソースの取得
	ID3D12Resource* GetRsc(void);

protected:
	// 頂点バッファの生成
	void VertexBuffer(void);


	// プリミティブタイプ
	prm::PrimitiveType type;

	// リソースID
	int rsc;
};
