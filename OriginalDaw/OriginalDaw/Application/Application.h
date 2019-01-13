#pragma once
#include "../etc/Vector2.h"
#include "../etc/Color.h"
#include <memory>

class Window;
class Queue;
class List;
class Fence;
class Swap;
class Render;
class Depth;
class PrimitiveMane;
class TexMane;

class Application
{
public:
	// �f�X�g���N�^
	~Application();

	// �C���X�^���X�ϐ��̎擾
	static Application& Get(void) {
		static Application instance;
		return instance;
	}

	// ���b�Z�[�W�̊m�F
	bool CheckMsg(void);

	// �L�[����
	bool CheckKey(const int& key);

	// �|�C���g�̕`��
	void DrawPoint(const Vec2f& pos, const Color& color);

	// ���C���̕`��
	void DrawLine(const Vec2f& pos1, const Vec2f& pos2, const Color& color);

	// �g���C�A���O���̕`��
	void DrawTriangle(const Vec2f& pos1, const Vec2f& pos2, const Vec2f& pos3, const Color& color);

	// �{�b�N�X�̕`��
	void DrawBox(const Vec2f& pos, const Vec2f& size, const Color& color);

	// �摜�̓ǂݍ���
	void LoadTex(int& addr, const std::string& fileName);

	// �摜�̕`��
	void DrawTex(int& addr, const Vec2f& pos, const Vec2f& size, const Vec2f& uvPos, const Vec2f& uvSize,
		const float& alpha = 1.0f, const bool& turnX = false, const bool& turnY = false);

	// ��ʃN���A
	void Clear(void);

	// �R�}���h�̎��s
	void Execution(void);

	// �摜�̍폜
	void DeleteTex(int& addr);

private:
	// �R���X�g���N�^
	Application();
	Application(const Application&) = delete;
	void operator=(const Application&) = delete;

	// �N���X�̐���
	void Create(void);

	// ���[�g�̐���
	void CreateRoot(void);


	// �E�B���h�E
	std::shared_ptr<Window>win;

	// �L���[
	std::shared_ptr<Queue>queue;

	// ���X�g
	std::shared_ptr<List>list;

	// �t�F���X
	std::shared_ptr<Fence>fence;

	// �X���b�v
	std::shared_ptr<Swap>swap;

	// �����_�[
	std::shared_ptr<Render>render;

	// �f�v�X
	std::shared_ptr<Depth>depth;

	// �v���~�e�B�u
	std::unique_ptr<PrimitiveMane>primitive;

	// �e�N�X�`��
	std::unique_ptr<TexMane>texture;
};
