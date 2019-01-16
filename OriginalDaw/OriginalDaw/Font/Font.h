#pragma once
#include "FontInfo.h"
#include <string>
#include <vector>
#include <memory>

struct tagTEXTMETRICW;
typedef tagTEXTMETRICW TEXTMETRIC;
struct _GLYPHMETRICS;
typedef _GLYPHMETRICS GLYPHMETRICS;
class Window;
class List;
class Root;
class Pipe;

class Font
{
public:
	// �R���X�g���N�^
	Font(std::weak_ptr<Window>win, void* handle, const std::string& code);
	// �f�X�g���N�^
	~Font();

	// �`��
	void Draw(std::weak_ptr<List>list, const DirectX::XMFLOAT2& pos, const DirectX::XMFLOAT2& size);

private:
	// SRV�̐���
	void SRV(const TEXTMETRIC& tm, const GLYPHMETRICS& gm);

	// �T�u���\�[�X�ɏ�������
	long WriteSub(const std::vector<unsigned char>& data);

	// ���_�o�b�t�@�̐���
	void VertexBuffer(void);

	// �t�H���g���̎擾
	void GetInfo(void* handle, const std::string& code);

	// �}�b�v
	long Map(void);


	// �E�B���h�E
	std::weak_ptr<Window>win;

	// �V�F�[�_���\�[�XID
	int srv;

	// ���_���\�[�X
	int rsc;

	// ���_
	std::vector<font::Vertex>vertex;
};
