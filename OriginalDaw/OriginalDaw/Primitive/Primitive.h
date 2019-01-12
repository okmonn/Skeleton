#pragma once
#include "PrimitiveInfo.h"

struct ID3D12Resource;

class Primitive
{
public:
	// �R���X�g���N�^
	Primitive(const prm::PrimitiveType& type);
	// �f�X�g���N�^
	~Primitive();

	// ���\�[�X�̎擾
	ID3D12Resource* GetRsc(void);

protected:
	// ���_�o�b�t�@�̐���
	void VertexBuffer(void);


	// �v���~�e�B�u�^�C�v
	prm::PrimitiveType type;

	// ���\�[�XID
	int rsc;
};
