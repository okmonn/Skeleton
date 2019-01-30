#pragma once
#include "../Input/Input.h"
#include "../Sound/Sound.h"
#include "../Useful/Useful.h"
#include "../Color.h"

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
	// �R���X�g���N�^
	Application(const Vec2& size);
	Application(const Application& app, const Vec2& size);
	Application(std::weak_ptr<Application>app, const Vec2& size);
	// �f�X�g���N�^
	~Application();

	// �^�C�g���̕ύX
	void ChangeTitle(const std::string& title) const;

	// �E�B���h�E�T�C�Y�̎擾
	Vec2 GetWinSize(void) const;

	// �}�E�X���W�̎擾
	Vec2 GetMousePos(void) const ;

	// �摜�̓ǂݍ���
	void LoadTex(int& addr, const std::string& fileName);

	// ���b�Z�[�W�̊m�F
	bool CheckMag(void) const;

	// �N���A
	void Clear(void) const;

	// �_�̕`��
	void DrawPoint(const Vec2f& pos, const Color& color);

	// �_�̕����`��
	void DrawMultiPoint(const std::vector<Vec2f>& pos, const Color& color);

	// ���̕`��
	void DrawLine(const Vec2f& pos1, const Vec2f& pos2, const Color& color);

	// ���̕����`��
	void DrawMultiLine(const std::vector<Vec2f>& pos1, const std::vector<Vec2f>& pos2, const Color& color);

	// �O�p�`�̕`��
	void DrawTriangle(const Vec2f& pos1, const Vec2f& pos2, const Vec2f& pos3, const Color& color);

	// �O�p�`�̕����`��
	void DrawMultiTriangle(const std::vector<Vec2f>& pos1, const std::vector<Vec2f>& pos2, const std::vector<Vec2f>& pos3, const Color& color);

	// �l�p�`�̕`��
	void DrawBox(const Vec2f& pos, const Vec2f& size, const Color& color);

	// �摜�̕`��
	void DrawTex(int& addr, const Vec2f& pos, const Vec2f& size, const Vec2f& uvPos, const Vec2f& uvSize,
		const float& alpha = 1.0f, const float& angle = 0.0f, const bool& turnX = false, const bool& turnY = false);

	// ���s
	void Execution(void) const;

	// �摜�̍폜
	void DeleteTex(int& addr);

private:
	void operator=(const Application&) = delete;

	// �N���X�̃C���X�^���X
	void Instance(const Vec2& size, void* parent = nullptr);


	// �E�B���h�E
	std::shared_ptr<Window>win;

	// �L���[
	std::shared_ptr<Queue>queue;

	// ���X�g
	std::shared_ptr<List>list;

	// �t�F���X
	std::unique_ptr<Fence>fence;

	// �X���b�v
	std::shared_ptr<Swap>swap;

	// �����_�[
	std::unique_ptr<Render>render;

	// �f�v�X
	std::shared_ptr<Depth>depth;

	// �v���~�e�B�u
	std::unique_ptr<PrimitiveMane>primitive;

	// �e�N�X�`��
	std::unique_ptr<TexMane>texture;
};
