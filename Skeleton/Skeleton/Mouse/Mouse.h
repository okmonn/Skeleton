#pragma once
#include "../Vector2.h"
#include <memory>

class Application;

class Mouse
{
public:
	// �R���X�g���N�^
	Mouse(std::weak_ptr<Application>app);
	// �f�X�g���N�^
	~Mouse();

	// ����
	void UpData(void);

	// �N���b�N���W�̎擾
	Vec2 GetClick(void) const {
		return click;
	}
	// ���݂̍��W�̎擾
	Vec2 GetPos(void) const {
		return pos;
	}

private:
	// �A�v���P�[�V����
	std::weak_ptr<Application>app;

	// �N���b�N���W
	Vec2 click;

	// ���݂̍��W
	Vec2 pos;
};
