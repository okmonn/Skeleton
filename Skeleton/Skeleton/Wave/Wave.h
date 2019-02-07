#pragma once
#include <memory>

class Application;
class Sound;

class Wave
{
public:
	// �R���X�g���N�^
	Wave(std::weak_ptr<Application>app, std::weak_ptr<Sound>sound);
	// �f�X�g���N�^
	~Wave();

	// �`��
	void Draw(void);

private:
	// �A�v���P�[�V����
	std::unique_ptr<Application>app;

	// �T�E���h
	std::weak_ptr<Sound>sound;
};
