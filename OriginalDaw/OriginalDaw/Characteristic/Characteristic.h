#pragma once
#include <memory>

class Application;
class Sound;

class Characteristic
{
public:
	// �R���X�g���N�^
	Characteristic(std::weak_ptr<Application>app, std::weak_ptr<Sound>sound);
	// �f�X�g���N�^
	Characteristic();

	// �`��
	void Draw(void);

	// ����
	void UpData(void);

private:
	// �A�v���P�[�V����
	std::unique_ptr<Application>app;

	// �T�E���h
	std::weak_ptr<Sound>sound;
};
