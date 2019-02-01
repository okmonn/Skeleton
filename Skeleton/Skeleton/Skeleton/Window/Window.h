#pragma once
#include "../Vector2.h"
#include "../tstring.h"

struct ID3D12DescriptorHeap;
struct ID3D12Resource;

class Window
{
public:
	// �R���X�g���N�^
	Window(const Vec2& size, void* parent);
	// �f�X�g���N�^
	~Window();

	// �h���b�v���ꂽ�t�@�C���p�X�̎擾
	std::string GetDropPass(void);

	// �E�B���h�E�n���h���̎擾
	void* Get(void) const {
		return handle;
	}
	// �T�C�Y�̎擾
	Vec2 GetSize(void) const {
		return size;
	}
	// �q�[�v�̎擾
	ID3D12DescriptorHeap* GetHeap(void) const {
		return cHeap;
	}
	// ���\�[�X�̎擾
	ID3D12Resource* GetRsc(void) const {
		return cRsc;
	}

private:
	// �E�B���h�E�̃R�[���o�b�N
#ifdef _WIN64
	static long __stdcall WindowProc(void* hWnd, unsigned int message, unsigned __int64 wParam, __int64 lParam);
#else
	static long __stdcall WindowProc(void* hWnd, unsigned int message, long wParam, long lParam);
#endif

	// �E�B���h�E�̐���
	int CreateWnd(void* parent);

	// �萔�p�q�[�v�̐���
	long CreateCHeap(void);

	// �萔���\�[�X�̐���
	long CreateCRsc(void);

	// CBV�̐���
	void CBV(void);

	// �}�b�v
	long Map(void);

	// ������
	void Init(void);


	// �E�B���h�E�n���h��
	void* handle;

	// �E�B���h�E�T�C�Y
	Vec2 size;

	// �E�B���h�E�C���X�^���X
	void* instance;

	// �E�B���h�E��
	const wchar_t* name;

	// �h���b�v���ꂽ�t�@�C���p�X
	static std::tstring dropFilePass;

	// �萔�p�q�[�v
	ID3D12DescriptorHeap* cHeap;

	// ���\�[�X
	ID3D12Resource* cRsc;
};
