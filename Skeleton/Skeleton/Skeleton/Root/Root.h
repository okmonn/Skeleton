#pragma once
#include <string>

struct ID3D10Blob;
typedef ID3D10Blob ID3DBlob;
struct ID3D12RootSignature;

class Root
{
public:
	// �R���X�g���N�^
	Root(const std::wstring& fileName);
	// �f�X�g���N�^
	~Root();

	// ���[�g�̎擾
	ID3D12RootSignature* Get(void) const {
		return root;
	}
	// �V�O�l�`���̎擾
	ID3DBlob* GetSig(void) const {
		return sig;
	}
	// ���_�V�F�[�_�̎擾
	ID3DBlob* GetVertex(void) const {
		return vertex;
	}
	// �W�I���g���[�V�F�[�_�̎擾
	ID3DBlob* GetGeometry(void) const {
		return geometry;
	}
	// �s�N�Z���V�F�[�_�̎擾
	ID3DBlob* GetPixel(void) const {
		return pixel;
	}
	// �R���s���[�g�V�F�[�_�̎擾
	ID3DBlob* GetCompute(void) const {
		return compute;
	}

private:
	// �V�F�[�_�̃R���p�C��
	long Compile(const std::wstring& fileName);

	// ���[�g���̎擾
	long GetRootInfo(void);

	// ���[�g�̐���
	long CreateRoot(void);


	// ���[�g
	ID3D12RootSignature* root;

	// �V�O�l�`��
	ID3DBlob* sig;

	// ���_�V�F�[�_
	ID3DBlob* vertex;

	// �W�I���g���[�V�F�[�_�[
	ID3DBlob* geometry;

	// �s�N�Z���V�F�[�_
	ID3DBlob* pixel;

	// �R���s���[�g�V�F�[�_
	ID3DBlob* compute;
};
