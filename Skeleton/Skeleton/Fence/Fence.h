#pragma once
#include <memory>

struct ID3D12Fence;
class Queue;

class Fence
{
public:
	// �R���X�g���N�^
	Fence(std::weak_ptr<Queue>queue);
	// �f�X�g���N�^
	~Fence();

	// �ҋ@
	void Wait(void);

private:
	// �t�F���X�̐���
	long CreateFence(void);


	// �L���[
	std::weak_ptr<Queue>queue;

	// �t�F���X
	ID3D12Fence* fence;

	// �C�x���g
	void* handle;

	// �t�F���X�J�E��
	long long cnt;
};
