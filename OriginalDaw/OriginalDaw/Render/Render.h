#pragma once
#include <memory>


class Swap;

class Render
{
public:
	// �R���X�g���N�^
	Render(std::weak_ptr<Swap>swap);
	// �f�X�g���N�^
	~Render();

private:
	// �����_�[�̐���
	void Create(void);


	// �X���b�v
	std::weak_ptr<Swap>swap;

	// �q�[�vID
	int heap;
};
