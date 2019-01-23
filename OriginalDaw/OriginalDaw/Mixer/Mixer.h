#pragma once
#include <vector>
#include <memory>
#include <thread>

class Application;
class Sound;

class Mixer
{
public:
	// �R���X�g���N�^
	Mixer();
	// �f�X�g���N�^
	~Mixer();

	// ����
	void Run(void);

private:
	// ������
	void Init(void);

	// �`��
	void Draw(void);

	// ����
	void UpData(void);


	// �A�v���P�[�V����
	std::shared_ptr<Application>app;
	
	// �T�E���h
	std::shared_ptr<Sound>sound;

	// �Đ��t���O
	bool playFlag;

	// �X���b�h�t���O
	bool threadFlag;

	// �X���b�h
	std::vector<std::thread>th;
};
