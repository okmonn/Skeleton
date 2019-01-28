#pragma once
#include "TexInfo.h"
#include <string>
#include <vector>

struct ID3D12DescriptorHeap;
class Window;
class List;
class Root;
class Pipe;

class Texture
{
public:
	// �R���X�g���N�^
	Texture(std::weak_ptr<Window>win, std::weak_ptr<Root>root, std::weak_ptr<Pipe>pipe);
	// �f�X�g���N�^
	~Texture();

	// �ǂݍ���
	void Load(const std::string& fileName);

	// �`��̏���
	void SetDraw(const DirectX::XMFLOAT2& pos, const DirectX::XMFLOAT2& size, const DirectX::XMFLOAT2& uvPos, const DirectX::XMFLOAT2& uvSize,
		const float& alpha, const float& angle, const bool& turnX, const bool& turnY);

	// �`��
	void Draw(std::weak_ptr<List>list);

private:
	// ���_���\�[�X�̐���
	long CreateVRsc(void);

	// �萔�p�q�[�v�̐���
	long CreateHeap(void);

	// �萔���\�[�X�̐���
	long CreateCRsc(void);

	// CBV�̐���
	void CBV(void);

	// �}�b�v
	long Map(void);

	// ������
	void Init(void);

	// SRV�̐���
	void SRV(void);

	// �T�u���\�[�X�ɏ�����
	long WriteSub(const std::string& fileName);


	// �E�B���h�E
	std::weak_ptr<Window>win;

	// ���[�g
	std::weak_ptr<Root>root;

	// �p�C�v
	std::weak_ptr<Pipe>pipe;

	// �q�[�v
	ID3D12DescriptorHeap* heap;

	// �萔�p���\�[�X
	ID3D12Resource* cRsc;

	// �V�F�[�_���\�[�X
	ID3D12Resource* sRsc;

	// ���_�p���\�[�X
	ID3D12Resource* vRsc;

	// ���M�p�f�[�^
	tex::CbvInfo* info;

	// �C���f�b�N�X
	unsigned int index;
};
