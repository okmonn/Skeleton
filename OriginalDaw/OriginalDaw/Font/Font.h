#pragma once
#include "../etc/tstring.h"
#include <memory>

class Root;
class Pipe;

class Font
{
public:
	// �R���X�g���N�^
	Font(std::weak_ptr<Root>root, std::weak_ptr<Pipe>pipe, const std::tstring& code);
	// �f�X�g���N�^
	~Font();

private:
	// �t�H���g���̎擾
	void GetInfo(void* font, const std::tstring& code);

	// SRV�̐���
	void SRV(void);

	// �V�F�[�_���\�[�XID
	int srv;

	// ���_���\�[�X
	int rsc;
};
