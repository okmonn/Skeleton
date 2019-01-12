#pragma once
#include <memory>

class Primitive;

class PrimitiveMane
{
public:
	// �f�X�g���N�^
	~PrimitiveMane();

	// �C���X�^���X�ϐ��̎擾
	static PrimitiveMane& Get(void) {
		static PrimitiveMane instance;
		return instance;
	}

private:
	// �R���X�g���N�^
	PrimitiveMane();
	PrimitiveMane(const PrimitiveMane&) = delete;
	void operator=(const Primitive&) = delete;
};
