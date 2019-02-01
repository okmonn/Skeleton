#pragma once
#include <vector>
#include <memory>
#include <thread>

class Application;
class Sound;
class Wave;

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

	// �Đ��Ǘ�
	void Play(void);

	// ����
	void UpData(void);

	// �g�`�̕`��
	void DrawWave(void);


	// �A�v���P�[�V����
	std::shared_ptr<Application>app;

	//�T�E���h
	std::shared_ptr<Sound>sound;

	// �g�`
	std::unique_ptr<Wave>wave;

	// �Đ��t���O
	bool play;

	// �X���b�h�t���O
	bool threadFlag;

	// �X���b�h
	std::thread th;
};
