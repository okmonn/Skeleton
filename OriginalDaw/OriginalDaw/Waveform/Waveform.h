#pragma once
#include <memory>

class Application;
class Sound;

class Waveform
{
public:
	// �R���X�g���N�^
	Waveform(std::weak_ptr<Application>app, std::weak_ptr<Sound>sound);
	// �f�X�g���N�^
	~Waveform();

	// �`��
	void Draw();

	// ����
	void UpData(void);

private:
	// �T�E���h
	std::weak_ptr<Sound>sound;

	// �A�v���P�[�V����
	std::unique_ptr<Application>app;
};
