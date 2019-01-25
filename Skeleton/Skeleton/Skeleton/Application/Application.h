#pragma once
#include "../Useful/Useful.h"
#include <memory>

class Window;
class Queue;
class List;
class Fence;
class Swap;

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

	// ���b�Z�[�W�̊m�F
	bool CheckMag(void) const;

	// �N���A
	void Clear(void) const;

	// ���s
	void Execution(void) const;

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
};
