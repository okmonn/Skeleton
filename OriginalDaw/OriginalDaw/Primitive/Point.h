#pragma once
#include "Primitive.h"

class Point :
	public Primitive
{
public:
	// �R���X�g���N�^
	Point(std::weak_ptr<Window>win, std::weak_ptr<Root>root, std::weak_ptr<Pipe>pipe);
	// �f�X�g���N�^
	~Point();

	// ���_�̃Z�b�g
	void SetVertex(const DirectX::XMFLOAT2& pos, const DirectX::XMFLOAT4& color);

private:

};
