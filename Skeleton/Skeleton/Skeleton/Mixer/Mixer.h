#pragma once
#include <memory>

class Application;
class Sound;

class Mixer
{
public:
	// �R���X�g���N�^
	Mixer();
	// �f�X�g���N�^
	~Mixer();

	// ���s
	void Execution(void);

private:
	// �`��
	void Draw(void);

	// ����
	void UpData(void);


	// �A�v���P�[�V����
	std::shared_ptr<Application>app;

	//�T�E���h
	std::shared_ptr<Sound>sound;
};
